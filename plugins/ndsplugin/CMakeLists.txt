cmake_minimum_required(VERSION 2.8.4)
project(cpp-mods.musicplayer.ndsplugin)

set(SOURCE_FILES
	NDSPlugin.cpp
	nds/vio2sf/vio2sf.c
	nds/vio2sf/desmume/arm_instructions.c
	nds/vio2sf/desmume/armcpu.c
	nds/vio2sf/desmume/bios.c
	nds/vio2sf/desmume/cp15.c
	nds/vio2sf/desmume/FIFO.c
	nds/vio2sf/desmume/GPU.c
	nds/vio2sf/desmume/matrix.c
	nds/vio2sf/desmume/mc.c
	nds/vio2sf/desmume/MMU.c
	nds/vio2sf/desmume/NDSSystem.c
	nds/vio2sf/desmume/SPU.c
	nds/vio2sf/desmume/thumb_instructions.c
)

#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-implicit-function-declaration -Wno-incompatible-pointer-types -Wno-parentheses-equality -Wno-bitwise-op-parentheses -Wno-parentheses -Wno-pointer-sign  -Wno-dangling-else")

add_library(ndsplugin ${SOURCE_FILES})
target_include_directories(ndsplugin PRIVATE nds/vio2sf nds/vio2sf/desmume nds)
target_compile_definitions(ndsplugin PRIVATE -DLSB_FIRST -DHAVE_STDINT_H -D_strnicmp=strncasecmp)

