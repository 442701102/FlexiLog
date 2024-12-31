#!/bin/sh

find ./  -name "*.o" -exec basename {} \; | while read i

do

echo "$i";

mv $i  .$i

done
