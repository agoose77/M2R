#!/bin/sh
filename="$1"
while read -r line
do
    echo "$line"
    root -q -l M2RV2.C++ <<EOF
      $line
EOF
echo "It is out now"
done < file1.txt
