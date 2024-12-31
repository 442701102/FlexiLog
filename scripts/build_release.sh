#!/bin/bash
SCRIPT_PATH=`realpath $0`
SCRIPT_DIR=`dirname $SCRIPT_PATH`
PROJECT_DIR=$SCRIPT_DIR/..
function build_lib()
{
  PLATFORM=$1
  BUILD_DIR=$PROJECT_DIR/build/${PLATFORM}
####  get clog file  
  if [[ "$CLOG_file" == "" ]] 
  then  
    CLOG_file=$BUILD_DIR/out/lib/libclog.a
    echo "clog file" $CLOG_file
  else  
    echo "clog file" $CLOG_file
  fi 
echo "build $PROJECT_DIR/Release/$PLATFORM/$Lib_type start"
  if [[ $PLATFORM == "ubuntu" ]]
  then
   `ar x $CLOG_file`
   `ar rc libclog.a *.o $PROJECT_DIR/Release/$PLATFORM/$Lib_type/.*.o `
   `rm *.o`
   `mv  libclog.a $PROJECT_DIR/Release/$PLATFORM/$Lib_type/lib/`
  else
   `/home/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ar x $CLOG_file`
   `/home/gcc-linaro-6.5.0-2018.12-x86_64_aarch64-linux-gnu/bin/aarch64-linux-gnu-ar rc libclog.a *.o $PROJECT_DIR/Release/$PLATFORM/$Lib_type/.*.o `
   `rm *.o`
   `mv  libclog.a $PROJECT_DIR/Release/$PLATFORM/$Lib_type/lib/`  
  fi
  echo "build $PROJECT_DIR/Release/$PLATFORM/$Lib_type finish"
  ls -l   $PROJECT_DIR/Release/$PLATFORM/$Lib_type/lib/libclog.a  
}
######## main
if [[ $#  -ge 2 ]]
then
############################################################
  Platform_type=$1
##### set PLATFORM #####
  if [[ $Platform_type == "linux" || $Platform_type == "Linux" || $Platform_type == "1" || $Platform_type == "ubuntu"  || $Platform_type == "x86" ]]
  then
  PLATFORM=ubuntu
  else
  PLATFORM=aarch64-journey
  fi
##### set Lib_type #####
  Lib_type=$2

  if [[ $Lib_type == "log4cplus" || $Lib_type == "log4" || $Lib_type == "1" || $Lib_type == "LOG4COLUS"  || $Lib_type == "ON" ]]
  then
    Lib_type="log4cplus"
  else
    Lib_type="glog"
  fi
##### set CLOG_file ##### 
  if [[ $#  -ge 3 ]]
  then
    CLOG_file=$3
  else
    CLOG_file=""
  fi
############################################################
  build_lib $PLATFORM $Lib_type

else
 echo "Too few parameters!!, please enter the build platform and lib type" 
fi


