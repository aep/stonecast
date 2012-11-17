PLATINUM := 3rdparty/PlatinumKit-1-0-3-0_a71192_66de97/
TARGET   := dlnarender
SOURCES  := main.cpp

SOURCES  += $(filter-out %gstreamer.c %xine.c %vlc.c, $(wildcard 3rdparty/libplayer/src/*.c))
SOURCES  += $(wildcard $(PLATINUM)/Neptune/Source/Core/*.cpp)
SOURCES  += $(filter-out %NptPosixDynamicLibraries.cpp, $(wildcard $(PLATINUM)/Neptune/Source/System/Posix/*.cpp))
SOURCES  += $(wildcard $(PLATINUM)/Neptune/Source/System/StdC/*.cpp)
SOURCES  += $(wildcard $(PLATINUM)/Neptune/Source/System/Bsd/*.cpp)
SOURCES  += $(wildcard $(PLATINUM)/Platinum/Source/Devices/MediaRenderer/*.cpp)
SOURCES  += $(wildcard $(PLATINUM)/Platinum/Source/Devices/MediaServer/*.cpp)
SOURCES  += $(wildcard $(PLATINUM)/Platinum/Source/Core/*.cpp)

OBJS := $(patsubst %.c,%.o,${SOURCES:%.cpp=%.o})

CXXFLAGS := \
	-I$(PLATINUM)/Platinum/Source/Core/ \
	-I$(PLATINUM)/Neptune/Source/Core/ \
	-I$(PLATINUM)/Platinum/Source/Platinum/  \
	-I$(PLATINUM)/Platinum/Source/Devices/MediaRenderer/  \
	-I$(PLATINUM)/Platinum/Source/Devices/MediaServer/ \
	-DNPT_CONFIG_ENABLE_LOGGING \
	-I3rdparty/libplayer/src \

CFLAGS := \
	-DHAVE_MPLAYER -DHAVE_WIN_XCB \


LDFLAGS := \
	-lpthread -lxcb\

$(TARGET): ${OBJS}
	g++ $^ ${LDFLAGS} -o $@

clean:
	rm -f $(TARGET)
	rm -f ${OBJS}
