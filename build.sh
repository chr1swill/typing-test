#!/bin/sh

set -xe

src="src"
outdir="bin"
cflags="-Wall -Wextra -ggdb -static"

if [ -d $outdir ];
then 
  rm -rf $outdir
fi

mkdir -p $outdir

gcc -o ${outdir}/main ${src}/main.c ${src}/render.c ${src}/wordlist.c $cflags
