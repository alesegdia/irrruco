#!/usr/bin/env bash

if [ "$#" -ne 2 ]; then
	echo "Usage: calib.sh <folder-with-jpgs> <yml-out-file>"
	exit 0
fi

if [ -d $1 ]; then
	rm $2
	touch $2
	echo "<?xml version=\"1.0\"?>"\
		 "<opencv_storage>"\
		 "<images>" >> $2
	a=1
	for i in $1/*.jpg; do
		new=$(printf "$1/xx%02d.jpg" ${a}) #04 pad to length of 4
		cp ${i} ${new}
		echo $new >> $2
		let a=a+1
	done
	echo "</images>"\
		 "</opencv_storage>" >> $2
else
	echo "$1 is not a folder!"
fi
