#!/bin/bash

# SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
# SPDX-License-Identifier: MIT

# Import files from an HFS disk image.

set -uo pipefail

: "${SDK=/opt/local/libexec/Retro68/universal}"
: "${TMPDIR=/tmp}"

proj=memdump
dsk="$proj.dsk"

cd -- "$(dirname -- "${BASH_SOURCE[0]}")" &>/dev/null || exit $?

cleanup() {
    [ -d "${tmpdir-}" ] && rm -rf "$tmpdir"
    printf "Unmount %s\n" "$dsk"
    humount
}

trap cleanup EXIT

printf "Mount %s\n" "$dsk"
hmount "$dsk" || exit $?

# The macbinary tool won't write to a file that exists, so we must make a
# temporary directory for it to write into. Its manpage documents a --force flag
# that can be used to make it write to an existing file, but the tool doesn't
# actually have any such option. Bug report filed with Apple as FB17654870.
tmpdir=$(mktemp -d "$TMPDIR/import.XXXXXXXX")
tmpfile="$tmpdir/tmp"

import_file() {
    local method="$1"
    local outfile="$2"
    local infile
    infile=$(printf %s "$outfile" | iconv -f utf-8 -t macroman)
    printf "Import %s\n" "$outfile"
    if [ "$method" = "macbinary" ]; then
        hcopy -m ":$infile" "$outfile.bin" || return $?
    else
        hcopy -m ":$infile" - \
            | macbinary decode --pipe -o "$tmpfile" || return $?
        case $method in
            rez)
                DeRez "$tmpfile" "$SDK/RIncludes/Types.r" \
                    -d ALRT_RezTemplateVersion=0 \
                    -d DLOG_RezTemplateVersion=0 \
                    | LC_CTYPE=C sed -E 's/^(\t\$"[^"]*").*/\1/' \
                    | iconv -f macroman -t utf-8 \
                    | ./sortrez.pl > "$outfile.r" || return $?
                ;;
            text)
                iconv -f macroman -t utf-8 < "$tmpfile" \
                    | tr '\r' '\n' > "$outfile" || return $?
                ;;
        esac
        rm -f "$tmpfile"
    fi
}

import_file rez "$proj.π" || exit $?
import_file rez "$proj.π.rsrc" || exit $?
import_file text "$proj.c" || exit $?
[ -n "$(hls "$proj" 2>/dev/null)" ] && import_file macbinary "$proj" || exit $?
