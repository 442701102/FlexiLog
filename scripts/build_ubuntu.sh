#!/bin/bash
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
PLATFORM=ubuntu
PROJECT_DIR=$SCRIPT_DIR/..
CMAKE_DIR=$PROJECT_DIR/cmake
BUILD_DIR=$PROJECT_DIR/build/${PLATFORM}
mkdir -p $BUILD_DIR
cd $BUILD_DIR
cp $SCRIPT_DIR/build.properties.${PLATFORM} $BUILD_DIR/build.properties.local
cmake -DCMAKE_INSTALL_PREFIX:PATH=$BUILD_DIR/output/ $PROJECT_DIR 
cmake --build .
make -j4 install
