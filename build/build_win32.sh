#! /bin/sh

if [ ! -x configure ]; then cd ../; fi
WIN32="${HOME}/root/windows-i386"
CFLAGS="-I${WIN32}/include -march=i386"
CPPFLAGS="${CFLAGS}"
LDFLAGS="-L${WIN32}/lib"
DATE="`date +%Y%m%d%H%M`"
CROSS=i586-mingw32msvc
if [ ! -z "${CROSS}" ]; then
	CROSSCMD="${CROSS}-"
fi
export CFLAGS LDFLAGS CPPFLAGS
make distclean
./configure --host=${CROSS} --prefix=${HOME}/root/windows-i386/ --with-libopennet=${HOME}/root/windows-i386/ && \
make || exit 1
