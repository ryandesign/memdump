<!--
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
-->

# Changelog

All notable changes to this project will be documented in this file.

The format is based on ideas from [Keep a
Changelog](https://keepachangelog.com/en/1.1.0/) and [Common
Changelog](https://common-changelog.org/). Because of limitations imposed on the
format of version numbers by the Macintosh `vers` resource, this project does
not currently use [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [0.0.0d4] - 2025-05-24

_Measure twice, cut once_

### Changed

- **UI**: Dump size is now displayed in an editable text field, in addition to
  the existing first and last fields. Whichever field you change, the others
  update accordingly. ([#6](https://github.com/ryandesign/memdump/issues/6))
- **UI**: Invalid input now elicits a beep. Repeated invalid input will summon
  an alert box.

### Fixed

- **UI**: No longer crashes under Switcher.
  ([#9](https://github.com/ryandesign/memdump/issues/9))

## [0.0.0d3] - 2025-05-22

_What's in a name?_

### Changed

- **UI**: The selected range of addresses is included in the default filename.
  ([#7](https://github.com/ryandesign/memdump/issues/7))
- **UI**: Start and End + 1 labels have been replaced by First and Last, and the
  last address now represents the last memory address that will be dumped, not
  one past the end. ([#8](https://github.com/ryandesign/memdump/issues/8))
- **UI**: Addresses are now shown in hexadecimal by default instead of decimal.

## [0.0.0d2] - 2025-05-21

_Now with hex!_

### Changed

- **UI**: Addresses can be displayed and entered in hexadecimal or decimal.
  ([#2](https://github.com/ryandesign/memdump/issues/2))

### Added

- **Code**: THINK C 4 project file now included.
- **Code**: Import and export scripts facilitate moving source code between a
  modern Mac and an emulated Macintosh disk image.
- **Documentation**: README.md explains how to download the latest version from
  the Releases page and how to work with the source code.
- **Documentation**: CHANGELOG.md shows what changed between versions.

### Removed

- **Code**: Unused `debug_num` function removed.

### Fixed

- **UI**: RAM size is now determined correctly under MultiFinder.
  ([#1](https://github.com/ryandesign/memdump/issues/1))
- **UI**: Non-digit characters can no longer be entered.
  ([#4](https://github.com/ryandesign/memdump/issues/4))
- **UI**: Clarified that the end address is the one _after_ the last one that
  will be dumped.
- **UI**: Dialog and alert box layouts adjusted to more closely conform to human
  interface guidelines. ([#5](https://github.com/ryandesign/memdump/issues/5))

## [0.0.0d1] - 2025-05-11

_Initial release_

### Added

- **UI**: A dialog box shows the size of RAM and allows input of a range of
  memory to dump to a file.

[0.0.0d4]: https://github.com/ryandesign/memdump/releases/tag/v0.0.0d4
[0.0.0d3]: https://github.com/ryandesign/memdump/releases/tag/v0.0.0d3
[0.0.0d2]: https://github.com/ryandesign/memdump/releases/tag/v0.0.0d2
[0.0.0d1]: https://github.com/ryandesign/memdump/releases/tag/v0.0.0d1
