#! /bin/sh

for file in *.[ch]; do
	if [ "${file}" = "compat.h" ]; then continue; fi
	if [ "${file}" = "config.h" ]; then continue; fi
	if [ "${file}" = "win32.h" ]; then continue; fi
	filelist="${filelist} ${file}"
done

(
	grep ' HAVE_.*_H$' ${filelist} | sed 's@.*HAVE_\(.*_H\)@\1@;s@_H$@.h@;s@_@/@g' | dd conv=lcase 2>/dev/null | grep -v '^config.h$'
	echo 'time.h'
	echo 'sys/time.h'
) | sort -u
