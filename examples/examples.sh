#!/bin/sh

build() {
   [ -f "$1" ] && return
   printf "building %s.c\n" "$1"
   gcc -O2 -Wall -Wextra -o "$1" "${1}.c" || exit
   command -v strip >/dev/null && strip "$1"
}

if [ "$1" = 'clean' ]; then
   rm -f checkstr isutf8
   exit
fi

build checkstr
build isutf8

STR="Hi€rögl𓁠f $(printf 'fo\n\tdfö\360\223\045\200ha\1Faa\360')"
printf "checkstr %s:\n" "$STR"
./checkstr "$STR"

echo

printf "isutf8 %s: " "$0"
./isutf8 "$0" && echo 'yes' || echo 'no'
printf "isutf8 /bin/sh: "
./isutf8 /bin/sh && echo 'yes' || echo 'no'
