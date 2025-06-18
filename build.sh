#!/bin/sh

set -xe

outdir="bin"
cflags="-Wall -Wextra -ggdb -static"

if [ -d $outdir ]; then 
  rm -rf $outdir
fi

mkdir -p $outdir

gcc -o ${outdir}/main main.c render.c wordlist.c $cflags
