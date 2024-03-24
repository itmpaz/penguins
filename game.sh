#!/bin/sh


cd bin

echo ---

./master -n 10 10 2 3 
rc=$?

echo ---

while [ $rc -ge 10 ]
do

./player
rc=$?
if [ $rc -ne 0 ]
then
    echo "Player stopped. Return code " $rc 
    exit
fi

./master -c 
rc=$?
if [ $rc -lt 10 ]
then
    break
fi


done

echo ---