<!--
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
-->

# memdump

![Screenshot](screenshot.png)

**memdump** is a program for Macintosh computers to let you save the contents of memory to a file.

## Usage

When you launch **memdump**, it comes preset to dump the entire contents of RAM
to a file. This may be larger than the amount of space available on your disk or
larger than you want. You can dump a smaller portion of memory by entering a
smaller dump size and/or adjusting the first and last address. Changing one
field will dynamically update the others accordingly.

**memdump** comes preset to display and accept hexadecimal numbers. To use
decimal numbers instead, click the Decimal radio button or press Command-D. To
switch back to hexadecimal, click the Hexadecimal radio button or press
Command-H.

Any part of the processor's address space can be dumped; you are not limited to
the range assigned to RAM. You could dump the ROM if you wish, if you know its
start address and size; it varies by Macintosh model.

There are portions of the address space dedicated to hardware I/O, also varying
by Macintosh model. Accessing these addresses could cause undesired hardware
operation including crashes and data corruption. Consult the address maps in
Guide to Macintosh Family Hardware and other Apple documentation to learn which
address ranges contain RAM and ROM which are safe to access and which ranges are
for hardware I/O and should not be accessed with **memdump**.

Once you've chosen which part of the address space to dump, click the Save
button or press Return or Enter or Command-S. You'll get a chance to name the
file and choose where to save it. Once the file is saved, **memdump** will
remain open in case you want to save a different range of addresses to another
file.

When you're done, click the Quit button or press Command-Q or Command-Period or
Escape to quit **memdump**.

## Compatibility

**memdump** is intended to be compatible with any Macintosh with a 680x0 processor.

## Download

Get a compiled version of **memdump** that's ready to run by downloading
memdump.bin from [the latest release on the Releases
page](https://github.com/ryandesign/memdump/releases/latest). It's encoded in
MacBinary format. Use a program like [StuffIt
Expander](https://www.gryphel.com/c/sw/archive/stuffexp/) or
[binUnpk](https://www.gryphel.com/c/minivmac/extras/binunpk/) to decode it.

## Source code

**memdump** was created using THINK C 4 and ResEdit.

Like other Macintosh programs, THINK C writes text files that use MacRoman
encoding and carriage return line endings, but in git repositories text files
are stored with UTF-8 encoding and linefeed line endings. And resource files
like the THINK C project file and files created by ResEdit can't be represented
in git repositories or on non-Mac filesystems, therefore in this repository
resource files have been converted to rez format.

If you wish to compile the code yourself, the `./export.sh` script will perform
all the necessary conversions and place the converted files on a new or existing
floppy disk image that can be loaded into an emulator like [Mini
vMac](https://www.gryphel.com/c/minivmac/). The compiled program and any changes
you make can be brought back from the disk image using the `./import.sh` script.

These scripts work only on macOS because they rely on programs that come with
macOS, like `macbinary` and `Rez`, and the fact that macOS filesystems can still
represent resource files. They also require
[hfsutils](https://ports.macports.org/port/hfsutils).

## License

MIT
