#!/usr/bin/env bash

NAME=JV8
V8_SRC_ROOT_DEFAULT=./support/v8
NUM_CPUS=1
PLATFORM_VERSION=android-8
ANT_TARGET=debug
ARCHITECTURES="arm,x86"
INSTALL=false
DEBUG=false

usage()
{
cat <<EOF
Usage: $0 options...

This script builds v8 against the Android NDK and a sample project skeleton that uses it.
Options:
  -h                  Show this help message and exit
  -s <v8_src>         The path to v8's project sourcetree's root. (default $V8_SRC_ROOT_DEFAULT)
  -a <archs>          Comma-separated build architectures.
                        Available architectures: arm, x86. (default $ARCHITECTURES)
  -n <ndk_dir>        The path to the Android NDK. (default \$ANDROID_NDK_ROOT)
  -t <toolchain_dir>  The path to the Android's toolchain binaries. (default \$ANDROID_TOOLCHAIN)
  -p <platform>       The Android SDK version to support (default $PLATFORM_VERSION)
  -j <num-cpus>       The number of processors to use in building (default $NUM_CPUS)
  -i                  Install resulting example APK onto default device.
  -d                  Install resulting example APK and begin debugging via ndk-gdb. (implicitly sets -i)
EOF
}

red='\e[31;1m'
green='\e[32;3m'

function msg() {
    echo -n -e "$green"
    echo -n \-\>\  
    tput sgr0
    echo $@ >&1
}

function error() {
    echo -n -e "$red"
    echo -n \-\> ERROR:\  
    tput sgr0
    echo $@ >&2
}

function checkForErrors() {
    ret=$?
    if [ $ret -ne 0 ]
    then
        if [ -n "$1" ]
        then
            error "$@"
        else
            error "Unexpected error. Aborting."
        fi
        exit $ret
    fi
}

while getopts "hs:a:n:t:p:j:id" OPTION; do
  case $OPTION in
    h)
      usage
      exit
      ;;
    s)
      V8_SRC_ROOT=$OPTARG
      ;;
    a)
      ARCHITECTURES=$OPTARG
      ;;
    n)
      ANDROID_NDK_ROOT=$OPTARG
      ;;
    t)
      ANDROID_TOOLCHAIN=$OPTARG
      ;;
    j)
      NUM_CPUS=$OPTARG
      ;;
    p)
      PLATFORM_VERSION=$OPTARG
      ;;
    i)
      INSTALL=true
      ;;
    d)
      DEBUG=true
      ;;
    ?)
      usage
      exit
      ;;
  esac
done

if [[ -z "$V8_SRC_ROOT" ]]
then
  V8_SRC_ROOT=$V8_SRC_ROOT_DEFAULT
fi

if [[ -z "$ANDROID_NDK_ROOT" ]]
then
  msg Please set \$ANDROID_NDK_ROOT or use the -n option.
  usage
  exit
fi

if [[ -z "$ANDROID_TOOLCHAIN" ]]
then
  msg Please set \$ANDROID_TOOLCHAIN or use the -t option.
  usage
  exit
fi

msg "Building v8..."
pushd $V8_SRC_ROOT

if [ ! -d "./build/gyp" ]
then
  msg "Building GYP first"
  make dependencies -j$NUM_CPUS
fi

# Iterate over archs and build v8
for i in $(echo $ARCHITECTURES | tr "," "\n")
do
  TARGET=""
  if [ "$i" == "arm" ]; then
    TARGET="android_arm.release"
  elif [ "$i" == "x86" ]; then
    TARGET="android_ia32.release"
  fi
  make $TARGET -j$NUM_CPUS
  checkForErrors "Error building v8 (target=$TARGET)"
done

popd


msg "Copying static library files..."
mkdir -p support/android/libs
for i in $(echo $ARCHITECTURES | tr "," "\n")
do
  ARCH=""
  if [ "$i" == "arm" ]; then
    ARCH="android_arm.release"
  elif [ "$i" == "x86" ]; then
    ARCH="android_ia32.release"
  fi
  rsync -tr $V8_SRC_ROOT/out/$ARCH/obj.target/tools/gyp/*.a support/android/libs/.
  checkForErrors "Error copying v8 static library (arch=$ARCH)"
done


msg "Copying v8 header files..."
mkdir -p support/include
rsync -tr $V8_SRC_ROOT/include/* support/include/.
checkForErrors "Error copying v8 header files"


msg "Building NDK libraries..."
ABI=""
for i in $(echo $ARCHITECTURES | tr "," "\n")
do
  if [ "$i" == "arm" ]; then
    ABI="$ABI armeabi armeabi-v7a"
  elif [ "$i" == "x86" ]; then
    ABI="$ABI x86"
  fi
  NDK_DEBUG=1 $ANDROID_NDK_ROOT/ndk-build -j$NUM_CPUS V=1 "APP_ABI=$ABI"
  checkForErrors "Error building NDK libraries (ABI=$ABI)"
done

msg "Creating jv8-native.jar..."
rm -fr lib
mv libs lib
mkdir -p dist/android/libs
rm dist/android/libs/jv8-native.jar
zip -r dist/android/libs/jv8-native.jar lib
checkForErrors "Error creating jv8-native.jar"

msg "Creating jv8.jar"
rm -fr lib
ant dist
checkForErrors "Error creating jv8.jar"

msg "Libs are in dist/android/libs"
