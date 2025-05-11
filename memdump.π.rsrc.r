/*
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
*/

resource 'ALRT' (129, purgeable) {
	{80, 104, 208, 408},
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
	{80, 104, 208, 408},
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

resource 'DITL' (128, purgeable) {
	{	/* array DITLarray: 10 elements */
		/* [1] */
		{150, 180, 170, 238},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{150, 110, 170, 168},
		Button {
			enabled,
			"Cancel"
		},
		/* [3] */
		{146, 176, 174, 242},
		UserItem {
			disabled
		},
		/* [4] */
		{10, 10, 26, 234},
		StaticText {
			disabled,
			"Save contents of memory to disk"
		},
		/* [5] */
		{50, 10, 66, 138},
		StaticText {
			disabled,
			"RAM size:"
		},
		/* [6] */
		{50, 160, 66, 240},
		StaticText {
			disabled,
			""
		},
		/* [7] */
		{80, 10, 96, 138},
		StaticText {
			disabled,
			"Starting address:"
		},
		/* [8] */
		{80, 160, 96, 240},
		EditText {
			enabled,
			""
		},
		/* [9] */
		{110, 10, 126, 138},
		StaticText {
			disabled,
			"Ending address:"
		},
		/* [10] */
		{110, 160, 126, 240},
		EditText {
			enabled,
			""
		}
	}
};

resource 'DITL' (129) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{90, 230, 110, 288},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{10, 70, 26, 294},
		StaticText {
			disabled,
			"Error ^0"
		}
	}
};

resource 'DITL' (130) {
	{	/* array DITLarray: 2 elements */
		/* [1] */
		{90, 230, 110, 288},
		Button {
			enabled,
			"OK"
		},
		/* [2] */
		{10, 70, 42, 294},
		StaticText {
			disabled,
			"^0"
		}
	}
};

resource 'DLOG' (128, purgeable) {
	{40, 130, 224, 382},
	dBoxProc,
	invisible,
	noGoAway,
	0x0,
	128,
	""
};

resource 'STR#' (128, purgeable) {
	{	/* array StringArray: 2 elements */
		/* [1] */
		"Save as:",
		/* [2] */
		"Dump"
	}
};

resource 'STR#' (129, purgeable) {
	{	/* array StringArray: 1 elements */
		/* [1] */
		"Start address must be less than end addr"
		"ess."
	}
};

resource 'vers' (1, purgeable) {
	0x0,
	0x0,
	development,
	0x1,
	0,
	"0.0.0d1",
	"0.0.0d1 © 2025 Ryan Carsten Schmidt"
};

