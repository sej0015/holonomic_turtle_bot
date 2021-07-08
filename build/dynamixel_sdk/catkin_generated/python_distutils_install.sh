#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/smart5/dynamixel-workbench/src/dynamixel_sdk"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/smart5/dynamixel-workbench/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/smart5/dynamixel-workbench/install/lib/python2.7/dist-packages:/home/smart5/dynamixel-workbench/build/dynamixel_sdk/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/smart5/dynamixel-workbench/build/dynamixel_sdk" \
    "/usr/bin/python2" \
    "/home/smart5/dynamixel-workbench/src/dynamixel_sdk/setup.py" \
     \
    build --build-base "/home/smart5/dynamixel-workbench/build/dynamixel_sdk" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/smart5/dynamixel-workbench/install" --install-scripts="/home/smart5/dynamixel-workbench/install/bin"
