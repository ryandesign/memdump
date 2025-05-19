#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
# SPDX-License-Identifier: MIT

# Import files from an HFS disk image.

set -uo pipefail

: "${SDK=/opt/local/libexec/Retro68/universal}"
: "${TMPDIR=/tmp}"

proj=memdump
dsk="$proj.dsk"

scriptdir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &> /dev/null && pwd)

cleanup() {
    [ -d "${tmpdir-}" ] && rm -rf "$tmpdir"
    printf "Unmount %s\n" "$dsk"
}

trap cleanup EXIT

printf "Mount %s\n" "$dsk"
hmount "$dsk" || exit $?

tmpdir=$(mktemp -d "$TMPDIR/import.XXXXXXXX")
binfile="$tmpdir/tmp.bin"

import_file() {
    local method="$1"
    local outfile="$2"
    local infile
    infile=$(printf %s "$outfile" | iconv -f utf-8 -t macroman)
    printf "Import %s\n" "$outfile"
    hcopy -m ":$infile" - | macbinary decode --pipe -o "$binfile" || return $?
    case $method in
        rez)
            DeRez "$binfile" "$SDK/RIncludes/Types.r" \
                -d ALRT_RezTemplateVersion=0 \
                -d DLOG_RezTemplateVersion=0 \
                | LC_CTYPE=C sed -E 's/^(\t\$"[^"]*").*/\1/' \
                | iconv -f macroman -t utf-8 \
                | "$scriptdir/sortrez.pl" > "$outfile.r" || return $?
            ;;
        text)
            iconv -f macroman -t utf-8 < "$binfile" \
                | tr '\r' '\n' > "$outfile" || return $?
            ;;
    esac
    rm -f "$binfile"
}

import_file rez "$proj.π" || exit $?
import_file rez "$proj.π.rsrc" || exit $?
import_file text "$proj.c" || exit $?
