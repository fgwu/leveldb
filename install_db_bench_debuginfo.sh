#!/bin/bash

cd out-static
file=`pwd`/db_bench
buildid=`eu-readelf -n $file| grep Build.ID: | awk '{print $3}'`
dir=`echo $buildid | cut -c1-2`
fn=`echo $buildid | cut -c3-`


echo mkdir -p /usr/lib/debug/.build-id/$dir
mkdir -p /usr/lib/debug/.build-id/$dir
echo ln -s $file /usr/lib/debug/.build-id/$dir/$fn
ln -s $file /usr/lib/debug/.build-id/$dir/$fn
echo ln -s $file /usr/lib/debug/.build-id/$dir/${fn}.debug
ln -s $file /usr/lib/debug/.build-id/$dir/${fn}.debug
