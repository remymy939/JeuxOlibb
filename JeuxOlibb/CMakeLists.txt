cmake_minimum_required(VERSION 3.3)
project(TP4)

SET(SDL2_SEARCH_PATHS
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local
        /usr
        /sw
        /opt/local
        /opt/csw
        /opt
        ${SDL2_PATH}
        )

FIND_PATH(SDL2_INCLUDE_DIR SDL.h
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PATH(SDL2_IMAGE_INCLUDE_DIR SDL_image.h
        HINTS
        $ENV {SDL2IMAGEDIR}
        $ENV {SDL2DIR}
        PATH_SUFFIXES include/SDL2 include
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_LIBRARY_TEMP
        NAMES SDL2
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_TTF_LIBRARY
        NAMES SDL2_ttf
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2_IMAGE_LIBRARY
        NAMES SDL2_image
        HINTS
        $ENV{SDL2TTFDIR}
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_LIBRARY(SDL2MAIN_LIBRARY
        NAMES SDL2main
        HINTS
        $ENV{SDL2DIR}
        PATH_SUFFIXES lib64 lib
        PATHS ${SDL2_SEARCH_PATHS}
        )

FIND_PACKAGE(Threads)
FIND_PACKAGE(OpenGL REQUIRED)
INCLUDE_DIRECTORIES(${OPENGL_INCLUDE_DIR})

SET(MINGW32_LIBRARY mingw32 CACHE STRING "mwindows for MinGW")
SET(SDL2_LIBRARY_TEMP ${MINGW32_LIBRARY} ${SDL2MAIN_LIBRARY} ${SDL2_LIBRARY_TEMP} ${CMAKE_THREAD_LIBS_INIT})

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2 REQUIRED_VARS SDL2_LIBRARY SDL2_INCLUDE_DIR)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(SOURCE_FILES main.cpp)


set(SOURCE_FILES main.cpp Folder.h TextFile.h Projectile.h Position.h WaveStone.h Monsters/MinouRapide.h Wave.h Monsters/SnowSprites.h Monsters/Monster.h Monsters/FlyingCabbage.h Towers/Tower.h Towers/FlameThrower.h)
add_executable(TP4 ${SOURCE_FILES})
target_link_libraries(TP4 ${OPENGL_LIBRARIES})
target_link_libraries(TP4 ${SDL2_LIBRARY_TEMP})
target_link_libraries(TP4 ${SDL2_TTF_LIBRARY})
target_link_libraries(TP4 ${SDL2_IMAGE_LIBRARY})