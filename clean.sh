#!/bin/bash

AGFILES="aclocal.m4 config.h.in configure Makefile.in compile depcomp install-sh missing"
AGDIRS="autom4te.cache"

for F in ${AGFILES}; do
	find ./ -type f -name "${F}" -print -delete;
done;

for D in ${AGDIRS}; do
	if [ -d "./${D}" ]; then
		find "./${D}" -type f -print -delete;
		find "./${D}" -type d -empty -print -delete;
	fi;
done;

