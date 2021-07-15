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

echo_and_run cd "/home/turtle/holonomic_turtle_bot/src/dynamixel_sdk"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/turtle/holonomic_turtle_bot/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/turtle/holonomic_turtle_bot/install/lib/python2.7/dist-packages:/home/turtle/holonomic_turtle_bot/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/turtle/holonomic_turtle_bot/build" \
    "/usr/bin/python2" \
    "/home/turtle/holonomic_turtle_bot/src/dynamixel_sdk/setup.py" \
     \
    build --build-base "/home/turtle/holonomic_turtle_bot/build/dynamixel_sdk" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/turtle/holonomic_turtle_bot/install" --install-scripts="/home/turtle/holonomic_turtle_bot/install/bin"
