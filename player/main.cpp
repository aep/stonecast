#include "PltUPnP.h"
#include "PltMediaRenderer.h"

#include "player.h"

#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include <iostream>

static int event_cb (player_event_t e, pl_unused void *data);
class MyRenderer : public PLT_MediaRenderer
{
    player_t *player;
    NPT_String uri, meta;

public:
    static MyRenderer *instance;
    MyRenderer(const char*  friendly_name,
            bool         show_ip = false,
            const char*  uuid = NULL,
            unsigned int port = 0)
        : PLT_MediaRenderer(friendly_name, show_ip, uuid, port)
    {
        instance = this;

        player_init_param_t param;
        memset (&param, 0, sizeof (player_init_param_t));
        param.ao       = PLAYER_AO_AUTO;
        param.vo       = PLAYER_VO_AUTO;
        param.event_cb = ::event_cb;
        param.quality  = PLAYER_QUALITY_NORMAL;

        player = player_init (PLAYER_TYPE_MPLAYER, PLAYER_MSG_WARNING , &param);

        if (!player) {
            fprintf(stderr, "player setup failed\n");
            exit (8);
        }
    }

    void updateTotalTrackTime()
    {
        PLT_Service* avt;
        FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", avt);
        if (!avt) return;

        char buf[201];
        long int total = mrl_get_property(player, NULL,  MRL_PROPERTY_LENGTH) / 1000;
        struct tm * tmp = gmtime(&total);
        strftime(buf, 200, "%H:%M:%S", tmp);
        avt->SetStateVariable("CurrentMediaDuration", buf);
        avt->SetStateVariable("CurrentTrackDuration", buf);

        avt->SetStateVariable("TransportStatus", "OK");
        avt->SetStateVariable("TransportPlaySpeed", "1");
        avt->SetStateVariable("NumberOfTracks", "1");
        avt->SetStateVariable("CurrentTrack", "1");
    }

    void updateCurrentTime()
    {
        PLT_Service* avt;
        FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", avt);
        if (!avt) return;

        char buf[201];
        long int secpos = player_get_time_pos (player) / 1000;
        struct tm *tmp = gmtime(&secpos);
        strftime(buf, 200, "%H:%M:%S", tmp);
        avt->SetStateVariable("RelativeTimePosition", buf);
        avt->SetStateVariable("AbsoluteTimePosition", buf);
    }

    virtual NPT_Result OnAction(PLT_ActionReference & action,  const PLT_HttpRequestContext& context)
    {
        if (player_playback_get_state(player) != PLAYER_PB_STATE_PAUSE) {
            updateCurrentTime();
        }
        return PLT_MediaRenderer::OnAction(action, context);
    }
    NPT_Result SetupServices()
    {
        fprintf(stderr, "SetupServices\n");
        PLT_MediaRenderer::SetupServices();
        PLT_Service* service;
        FindServiceByType("urn:schemas-upnp-org:service:ConnectionManager:1", service);
        service->SetStateVariable("SinkProtocolInfo" ,
                "http-get:*:*:*"
                ",http-get:*:audio/mpegurl:*"
                ",http-get:*:audio/mpeg:*"
                ",http-get:*:audio/mpeg3:*"
                ",http-get:*:audio/mp3:*"
                ",http-get:*:audio/mp4:*"
                ",http-get:*:audio/basic:*"
                ",http-get:*:audio/midi:*"
                ",http-get:*:audio/ulaw:*"
                ",http-get:*:audio/ogg:*"
                ",http-get:*:audio/DVI4:*"
                ",http-get:*:audio/G722:*"
                ",http-get:*:audio/G723:*"
                ",http-get:*:audio/G726-16:*"
                ",http-get:*:audio/G726-24:*"
                ",http-get:*:audio/G726-32:*"
                ",http-get:*:audio/G726-40:*"
                ",http-get:*:audio/G728:*"
                ",http-get:*:audio/G729:*"
                ",http-get:*:audio/G729D:*"
                ",http-get:*:audio/G729E:*"
                ",http-get:*:audio/GSM:*"
                ",http-get:*:audio/GSM-EFR:*"
                ",http-get:*:audio/L8:*"
                ",http-get:*:audio/L16:*"
                ",http-get:*:audio/LPC:*"
                ",http-get:*:audio/MPA:*"
                ",http-get:*:audio/PCMA:*"
                ",http-get:*:audio/PCMU:*"
                ",http-get:*:audio/QCELP:*"
                ",http-get:*:audio/RED:*"
                ",http-get:*:audio/VDVI:*"
                ",http-get:*:audio/ac3:*"
                ",http-get:*:audio/vorbis:*"
                ",http-get:*:audio/speex:*"
                ",http-get:*:audio/x-aiff:*"
                ",http-get:*:audio/x-pn-realaudio:*"
                ",http-get:*:audio/x-realaudio:*"
                ",http-get:*:audio/x-wav:*"
                ",http-get:*:audio/x-ms-wma:*"
                ",http-get:*:audio/x-mpegurl:*"
                ",http-get:*:application/x-shockwave-flash:*"
                ",http-get:*:application/ogg:*"
                ",http-get:*:application/sdp:*"
                ",http-get:*:image/gif:*"
                ",http-get:*:image/jpeg:*"
                ",http-get:*:image/ief:*"
                ",http-get:*:image/png:*"
                ",http-get:*:image/tiff:*"
                ",http-get:*:video/avi:*"
                ",http-get:*:video/mpeg:*"
                ",http-get:*:video/fli:*"
                ",http-get:*:video/flv:*"
                ",http-get:*:video/quicktime:*"
                ",http-get:*:video/vnd.vivo:*"
                ",http-get:*:video/vc1:*"
                ",http-get:*:video/ogg:*"
                ",http-get:*:video/mp4:*"
                ",http-get:*:video/BT656:*"
                ",http-get:*:video/CelB:*"
                ",http-get:*:video/JPEG:*"
                ",http-get:*:video/H261:*"
                ",http-get:*:video/H263:*"
                ",http-get:*:video/H263-1998:*"
                ",http-get:*:video/H263-2000:*"
                ",http-get:*:video/MPV:*"
                ",http-get:*:video/MP2T:*"
                ",http-get:*:video/MP1S:*"
                ",http-get:*:video/MP2P:*"
                ",http-get:*:video/BMPEG:*"
                ",http-get:*:video/x-ms-wmv:*"
                ",http-get:*:video/x-ms-avi:*"
                ",http-get:*:video/x-flv:*"
                ",http-get:*:video/x-fli:*"
                ",http-get:*:video/x-ms-asf:*"
                ",http-get:*:video/x-ms-asx:*"
                ",http-get:*:video/x-ms-wmx:*"
                ",http-get:*:video/x-ms-wvx:*"
                ",http-get:*:video/x-msvideo:*"
                );
        return NPT_SUCCESS;
    }


    virtual NPT_Result OnSetAVTransportURI(PLT_ActionReference& action)
    {
        PLT_Service* service;
        FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", service);

        action->GetArgumentValue("CurrentURI", uri);
        action->GetArgumentValue("CurrentURIMetaData", meta);

        const char *s = uri.GetChars();
        fprintf(stderr, "=> set url: %s\n", s);

        mrl_resource_network_args_t *args = (mrl_resource_network_args_t*)calloc (1, sizeof (mrl_resource_network_args_t));
        args->url = strdup (s);
        mrl_t *mrl = mrl_new (player, MRL_RESOURCE_HTTP , args);
        player_mrl_set(player, mrl);

        updateTotalTrackTime();

        service->SetStateVariable("AVTransportURI", uri);
        service->SetStateVariable("AVTransportURIMetaData", meta);
        service->SetStateVariable("CurrentTrackURI", uri);
        service->SetStateVariable("CurrentTrackMetadata", meta);

        return NPT_SUCCESS;
    }


    virtual NPT_Result OnPlay(PLT_ActionReference& action)
    {
        PLT_Service* avt;
        FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", avt);

        fprintf(stderr, "=> play\n");
        switch (player_playback_get_state (player))
        {
            case PLAYER_PB_STATE_PAUSE:
                player_playback_pause (player);
                updateTotalTrackTime();
                break;
            case PLAYER_PB_STATE_IDLE:
                avt->SetStateVariable("TransportState", "TRANSITIONING");
                player_playback_start (player);
                updateTotalTrackTime();
                break;
            default:
                break;
        };

        return NPT_SUCCESS;
    }


    virtual NPT_Result OnStop(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> stop\n");
        player_playback_stop (player);
        return NPT_SUCCESS;
    }
    virtual NPT_Result OnPause(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> pause\n");
        player_playback_pause (player);
        return NPT_SUCCESS;
    }

    virtual NPT_Result OnNext(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> next\n");
        return NPT_SUCCESS;

    }

    virtual NPT_Result OnPrevious(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> previous\n");
        return NPT_SUCCESS;
    }

    virtual NPT_Result OnSeek(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> seek\n");
        return NPT_SUCCESS;
    }

    virtual NPT_Result OnSetVolume(PLT_ActionReference& action)
    {
        PLT_Service* rct;
        FindServiceByType("urn:schemas-upnp-org:service:RenderingControl:1", rct);
        if (!rct)
            return NPT_SUCCESS;

        NPT_String volume;
        action->GetArgumentValue("DesiredVolume", volume);
        int vol = atoi((const char*)volume);
        player_audio_volume_set(player, vol);

        rct->SetStateVariable("Volume", volume);


        fprintf(stderr, "=> setVolume %i \n", vol);
        return NPT_SUCCESS;
    }

    virtual NPT_Result OnSetMute(PLT_ActionReference& action)
    {
        fprintf(stderr, "=> setMute \n");
        PLT_Service* rct;
        FindServiceByType("urn:schemas-upnp-org:service:RenderingControl:1", rct);
        if (!rct)
            return NPT_SUCCESS;

        NPT_String mute;
        action->GetArgumentValue("DesiredMute",mute);
        player_audio_mute_set (player, mute == "1" ? PLAYER_MUTE_ON :PLAYER_MUTE_OFF);

        rct->SetStateVariable("Mute", mute);
        return NPT_SUCCESS;
    }

    int event_cb (player_event_t e, pl_unused void *data)
    {
        PLT_Service* avt;
        FindServiceByType("urn:schemas-upnp-org:service:AVTransport:1", avt);
        if (!avt) return 0;

        switch(e) {
            case PLAYER_EVENT_PLAYBACK_START:
                fprintf(stderr, "player: started\n");
                avt->SetStateVariable("TransportState", "PLAYING");
                avt->SetStateVariable("TransportStatus", "OK");
                break;
            case PLAYER_EVENT_PLAYBACK_STOP:
                fprintf(stderr, "player: stoped\n");
                avt->SetStateVariable("TransportState", "STOPPED");
                break;
            case PLAYER_EVENT_PLAYLIST_FINISHED:
            case PLAYER_EVENT_PLAYBACK_FINISHED:
                fprintf(stderr, "player: finished\n");
                avt->SetStateVariable("TransportState", "STOPPED");
                break;

            case PLAYER_EVENT_PLAYBACK_PAUSE:
                fprintf(stderr, "player: paused\n");
                avt->SetStateVariable("TransportState", "PAUSED_PLAYBACK");
                break;
            case PLAYER_EVENT_PLAYBACK_UNPAUSE:
                fprintf(stderr, "player: unpaused\n");
                avt->SetStateVariable("TransportState", "PLAYING");
                break;
        }


        return 0;
    }
};

MyRenderer *MyRenderer::instance;

static int event_cb (player_event_t e, void *data)
{
    return MyRenderer::instance->event_cb(e, data);
}



int main(int argc, char** argv)
{
    PLT_UPnP upnp;

    PLT_DeviceHostReference device(new MyRenderer("Reave", true, "e6572b54-f3c7-2d91-2fb5-b747f2537e21"));
    upnp.AddDevice(device);

    upnp.Start();

    for (;;) {
        sleep (10000);
    }

    upnp.Stop();
    return 0;
}
