/*
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
*/

resource 'ALRT' (129, purgeable) {
	{80, 104, 193, 408},
	129,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	}
};

resource 'ALRT' (130, purgeable) {
	{80, 104, 193, 408},
	130,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, visible, sound1,
		/* [3] */
		OK, visible, sound1,
		/* [4] */
		OK, visible, sound1
	}
};

resource 'ALRT' (131, purgeable) {
	{80, 104, 193, 408},
	131,
	{	/* array: 4 elements */
		/* [1] */
		OK, visible, sound1,
		/* [2] */
		OK, invisible, sound1,
		/* [3] */
		OK, invisible, sound1,
		/* [4] */
		OK, invisible, sound1
	}
};

resource 'DITL' (128, purgeable) {
	{	/* array DITLarray: 14 elements */
		/* [1] */
		{184, 184, 204, 242},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{184, 113, 204, 171},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{180, 180, 208, 246},
		UserItem {
			disabled
		},
		/* [4] */
		{7, 8, 23, 244},
		StaticText {
			disabled,
			"Save contents of memory to disk"
		},
		/* [5] */
		{41, 8, 59, 98},
		RadioButton {
			enabled,
			"Decimal"
		},
		/* [6] */
		{41, 108, 59, 238},
		RadioButton {
			enabled,
			"Hexadecimal"
		},
		/* [7] */
		{77, 8, 93, 148},
		StaticText {
			disabled,
			"RAM size in bytes:"
		},
		/* [8] */
		{77, 160, 93, 239},
		StaticText {
			disabled,
			""
		},
		/* [9] */
		{102, 8, 118, 148},
		StaticText {
			disabled,
			"Dump size:"
		},
		/* [10] */
		{102, 160, 118, 239},
		EditText {
			enabled,
			""
		},
		/* [11] */
		{127, 8, 143, 148},
		StaticText {
			disabled,
			"First address:"
		},
		/* [12] */
		{127, 160, 143, 239},
		EditText {
			enabled,
			""
		},
		/* [13] */
		{152, 8, 168, 148},
		StaticText {
			disabled,
			"Last address:"
		},
		/* [14] */
		{152, 160, 168, 239},
		EditText {
			enabled,
			""
		}
	}
};

resource 'DITL' (129, purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{83, 236, 103, 294},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{7, 73, 71, 294},
		StaticText {
			disabled,
			"Error ^0"
		}
	}
};

resource 'DITL' (130, purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{83, 236, 103, 294},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{7, 73, 71, 294},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'DITL' (131, purgeable) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{83, 236, 103, 294},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{7, 73, 71, 294},
		StaticText {
			disabled,
			"Only digits can be typed into this text "
			"field."
		}
	}
};

resource 'DLOG' (128, purgeable) {
	{40, 130, 254, 382},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	128,
	""
};

resource 'STR#' (128, purgeable) {
	{	/* array StringArray: 4 elements */
		/* [1] */
		"Save as:",
		/* [2] */
		"Dump ",
		/* [3] */
		"-",
		/* [4] */
		""
	}
};

resource 'STR#' (129, purgeable) {
	{	/* array StringArray: 2 elements */
		/* [1] */
		"Dump size must be greater than zero.",
		/* [2] */
		"First address must not be less than zero"
		"."
	}
};

resource 'vers' (1, purgeable) {
	0x0,
	0x0,
	development,
	0x4,
	0,
	"0.0.0d4",
	"0.0.0d4 © 2025 Ryan Carsten Schmidt"
};

