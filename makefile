CXX=g++-6
CXXFLAGS=-std=gnu++14 -D_GLIBCXX_USE_CXX11_ABI=0 -D_POSIX=1 -DRAD_TELEMETRY_DISABLED -DLINUX=1 -D_LINUX=1 -DPOSIX=1 -DGNUC=1 -D_DEVELOPER=1 -DNO_MALLOC_OVERRIDE -O3 -g3 -ggdb -w -shared -Wall -Wno-unknown-pragmas -fmessage-length=0 -m32 -fvisibility=hidden -fPIC
SDKFOLDER=$(realpath source-sdk-2013/mp/src)
SIMPLE_IPC_DIR = $(realpath simple-ipc/src/include)
INCLUDES=-I$(SIMPLE_IPC_DIR) -I$(SDKFOLDER)/public -I$(SDKFOLDER)/mathlib -I$(SDKFOLDER)/common -I$(SDKFOLDER)/public/tier1 -I$(SDKFOLDER)/public/tier0 -I$(SDKFOLDER)
CXXFLAGS += $(INCLUDES)
LIB_DIR=lib
LDFLAGS=-m32 -fno-gnu-unique -D_GLIBCXX_USE_CXX11_ABI=0 -shared -L$(realpath $(LIB_DIR))
LDLIBS=-static -lc -lstdc++ -ltier0 -lvstdlib
SRC_DIR = src
OUT_NAME = libcathook.so
TARGET_DIR = bin
TARGET = $(TARGET_DIR)/$(OUT_NAME)
SOURCES = $(shell find $(SRC_DIR) -name "*.cpp" -print)
SOURCES += $(shell find $(SIMPLE_IPC_DIR) -name "*.cpp" -print)
OBJECTS = $(SOURCES:.cpp=.o)
DEPENDS = $(SOURCES:.cpp=.d)
SRC_SUBDIRS=$(shell find $(SRC_DIR) -type d -print)

.PHONY: clean directories

all:
	mkdir -p $(TARGET_DIR)
	$(MAKE) $(TARGET)
	
.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@
	
%.d: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJECTS) $(LDLIBS) -o $(TARGET)

clean:
	find . -type f -name '*.o' -delete
	find . -type f -name '*.d' -delete
	rm -rf ./bin
	
ifneq ($(MAKECMDGOALS), clean)
-include $(DEPENDS)
endif