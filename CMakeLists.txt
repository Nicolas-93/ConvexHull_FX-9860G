# Configure with [fxsdk build-fx] or [fxsdk build-cg], which provide the
# toolchain file and module path of the fxSDK

cmake_minimum_required(VERSION 3.15)
project(ConvexHull)

include(GenerateG1A)
include(GenerateG3A)
include(Fxconv)
include_directories(include)
find_package(Gint 2.9 REQUIRED)

set(SOURCES
  src/main.c
  src/convexhull.c
  src/generation.c
  src/graphics.c
  src/scenes.c
  include/convexhull.h
  include/generation.h
  include/graphics.h
  include/scenes.h
  include/args.h
  include/circleq.h
)
# Shared assets, fx-9860G-only assets and fx-CG-50-only assets
set(ASSETS
  # ...
)
set(ASSETS_fx
  assets-fx/example.png
  # ...
)
set(ASSETS_cg
  assets-cg/example.png
  # ...
)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wno-unused-parameter")

fxconv_declare_assets(${ASSETS} ${ASSETS_fx} ${ASSETS_cg} WITH_METADATA)

add_executable(convexhull ${SOURCES} ${ASSETS} ${ASSETS_${FXSDK_PLATFORM}})
target_compile_options(convexhull PRIVATE -Wall -Wextra -Os)
target_link_libraries(convexhull Gint::Gint)

if("${FXSDK_PLATFORM_LONG}" STREQUAL fx9860G)
  generate_g1a(TARGET convexhull OUTPUT "Convex.g1a"
    NAME "Convex" ICON assets-fx/icon.png)
elseif("${FXSDK_PLATFORM_LONG}" STREQUAL fxCG50)
  generate_g3a(TARGET convexhull OUTPUT "Convex.g3a"
    NAME "Convex" ICONS assets-cg/icon-uns.png assets-cg/icon-sel.png)
endif()
