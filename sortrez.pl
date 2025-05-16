#!/usr/bin/env perl -00

# SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
# SPDX-License-Identifier: MIT

# Sort resources in a rez file by types (alphabetically) and IDs (numerically).

while (<>) {
    ($type, $id) = m/^\S+\s+(\S+)\s+\((-?\d+)/;
    push @resources, $_;
    push @types, $type;
    push @ids, $id;
}

print @resources[ sort {
    $types[$a] cmp $types[$b] || $ids[$a] <=> $ids[$b];
} 0..$#resources ];
