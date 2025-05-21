#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
# SPDX-License-Identifier: MIT

# Export files onto an HFS disk image.

set -uo pipefail

: "${SDK=/opt/local/libexec/Retro68/universal}"
: "${TMPDIR=/tmp}"

proj=memdump
dsk="$proj.dsk"

err() {
    local msg="$1"
    local code="${2-1}"

    printf "%s: %s\n" "$(basename "$0")" "$msg" 1>&2
    exit "$code"
}

[ "$(uname -s)" = "Darwin" ] || err "this script only works on macOS"

cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null || exit $?

cleanup() {
    [ -n "${tmpfile-}" ] && rm -f "$tmpfile"
    [ -n "${tmpfile2-}" ] && rm -f "$tmpfile2"
    printf "Unmount %s\n" "$dsk"
    humount
}

trap cleanup EXIT

if [ -f "$dsk" ]; then
    printf "Mount %s\n" "$dsk"
    hmount "$dsk" || exit $?
else
    printf "Create %s\n" "$dsk"
    dd if=/dev/zero of="$dsk" bs=1k count=800 status=none || exit $?
    hformat -l "$proj" "$dsk" || exit $?
fi

export_file() {
    local method="$1"
    local infile="$2"
    local type="$3"
    local creator="$4"
    local outfile
    outfile=$(printf %s "$infile" | iconv -f utf-8 -t macroman)
    printf "Export %s\n" "$infile"
    tmpfile=$(mktemp "$TMPDIR/export.XXXXXXXX")
    case $method in
        rez)
            tmpfile2=$(mktemp "$TMPDIR/export.XXXXXXXX")
            iconv -f utf-8 -t macroman < "$infile.r" > "$tmpfile2" || return $?
            Rez "$SDK/RIncludes/Types.r" "$tmpfile2" \
                -d ALRT_RezTemplateVersion=0 \
                -d DLOG_RezTemplateVersion=0 \
                -o "$tmpfile" || return $?
            rm -f "$tmpfile2"
            tmpfile2=
            ;;
        text)
            tr '\n' '\r' < "$infile" \
                | iconv -f utf-8 -t macroman > "$tmpfile" || return $?
            ;;
    esac
    SetFile -t "$type" -c "$creator" "$tmpfile" || return $?
    macbinary encode --pipe "$tmpfile" | hcopy -m - ":$outfile" || return $?
    rm -f "$tmpfile"
    tmpfile=
}

export_file rez "$proj.π" PROJ KAHL || exit $?
export_file rez "$proj.π.rsrc" rsrc RSED || exit $?
export_file text "$proj.c" TEXT KAHL || exit $?
