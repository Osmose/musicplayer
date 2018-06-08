cmake_minimum_required(VERSION 3.12)
project(musicplayer)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # not necessary, but encouraged
set(APPLE 1)
#project(cpp-mods.musicplayer)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(CMAKE_BUILD_TYPE Debug)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

set(APONE ../apone/mods)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MUSICPLAYER_PLUGINS
	# adplugin
	aoplugin
	ayflyplugin
	# ffmpegplugin
	gmeplugin
	gsfplugin
	heplugin
	hivelyplugin
	htplugin
	mdxplugin
	# mp3plugin
	ndsplugin
	# openmptplugin
	rsnplugin
	s98plugin
	sc68plugin
	stsoundplugin
	tedplugin
	uadeplugin
	usfplugin
	viceplugin
    v2plugin
    # Obsolete
    # sexypsfplugin
    # modplugin
)
add_subdirectory(plugins)

add_subdirectory(${APONE}/coreutils coreutils)
add_subdirectory(${APONE}/crypto crypto)
add_subdirectory(${APONE}/audioplayer audioplayer)
add_subdirectory(${APONE}/archive archive)

#add_executable(play main.cpp)
#target_link_libraries(play plugin_register ${MUSICPLAYER_PLUGINS} coreutils audioplayer crypto archive pthread)

add_library(musicplayer SHARED wrapper.cpp ${PLUGIN_EXTRA_FILES})
target_link_libraries(musicplayer ${MUSICPLAYER_PLUGINS} coreutils audioplayer crypto archive pthread)
set_property(TARGET musicplayer PROPERTY CXX_STANDARD 14)
set_property(TARGET musicplayer PROPERTY CXX_STANDARD_REQUIRED ON)
