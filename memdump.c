/*
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
*/

#include <MultiFinder.h>
#include <OSUtil.h>

#ifndef nil
#define nil ((void *)0)
#endif

#define r_file_strings 128
#define i_prompt 1
#define i_default_filename_start 2
#define i_default_filename_middle 3
#define i_default_filename_end 4

#define r_error_strings 129
#define e_bad_count 1
#define e_bad_start 2

#define r_dialog 128
#define i_ok 1
#define i_cancel 2
#define i_default_button_outline 3
#define i_decimal 5
#define i_hexadecimal 6
#define i_ram_size 8
#define i_dump_size 10
#define i_first 12
#define i_last 14

#define r_error_number_alert 129
#define r_error_string_alert 130
#define r_bad_key_alert 131

#define k_enter 0x03
#define k_escape 0x1b
#define k_return 0x0d

#define k_hilite 1
#define k_unhilite 0

#define k_visual_delay 8

#define k_default_button_outline_thickness 3
#define k_default_button_outline_curve 16

#define k_min_selection 0
#define k_max_selection 32767

#define k_in_front ((WindowPtr)-1)

#define k_sfputfile_left 104
#define k_sfputfile_top 80

#define k_creator_code '????'
#define k_type_code '????'

#ifndef _InitGraf
#define _InitGraf 0xA86E
#define _OSDispatch 0xA88F
#define _Unimplemented 0xA89F
typedef SignedByte TrapType;
#endif

#define has_64k_rom() (ROM85 < 0)

struct data {
	long ram_size;
	long count;
	long first;
	SFReply reply;
	Boolean keypress;
	Boolean hex;
	Boolean old_hex;
	Boolean ram_size_dirty;
	Boolean count_dirty;
	Boolean first_dirty;
	Boolean last_dirty;
};

static short get_num_toolbox_traps(void) {
	return (NGetTrapAddress(_InitGraf, ToolTrap) == NGetTrapAddress(0xAA6E, ToolTrap)) ? 0x200 : 0x400;
}

static TrapType get_trap_type(unsigned short trap) {
	return (trap & 0x0800) ? ToolTrap : OSTrap;
}

static has_trap(unsigned short trap) {
	TrapType trap_type;
	Boolean has_trap;

	if (has_64k_rom()) {
		has_trap = false;
	} else {
		trap_type = get_trap_type(trap);
		if (ToolTrap == trap_type) {
			trap &= 0x03FF;
			if (trap >= get_num_toolbox_traps()) {
				trap = _Unimplemented;
			}
		}
		has_trap = NGetTrapAddress(trap, trap_type) != NGetTrapAddress(_Unimplemented, ToolTrap);
	}
	return has_trap;
}

/* Tech Note 158: Frequently Asked MultiFinder Questions */
#define switch_ptr ((long *)0x282)
static Boolean has_switcher(void) {
	return (0 != *switch_ptr) && (-1 != *switch_ptr);
}

static void append_string(Str255 string, StringPtr append) {
	Size count;

	count = (append[0] + string[0] > 255) ? (255 - string[0]) : append[0];
	BlockMove(&append[1], &string[string[0] + 1], count);
	string[0] += count;
}

static void show_err(OSErr err) {
	Str255 string;
	short id;

	if (err < 0) {
		id = r_error_number_alert;
		NumToString(err, string);
	} else {
		id = r_error_string_alert;
		GetIndString(string, r_error_strings, err);
	}
	ParamText(string, "\p", "\p", "\p");
	SetCursor(&arrow);
	StopAlert(id, nil);
}

static void save_data(struct data *data) {
	OSErr err;
	short refnum;
	long count;
	CursHandle watch;

	watch = GetCursor(watchCursor);
	if (watch) SetCursor(*watch);

	err = FSDelete(data->reply.fName, data->reply.vRefNum);
	if (fnfErr != err && noErr != err) {
		show_err(err);
		return;
	}
	err = Create(data->reply.fName, data->reply.vRefNum, k_creator_code, k_type_code);
	if (noErr != err) {
		show_err(err);
		return;
	}
	err = FSOpen(data->reply.fName, data->reply.vRefNum, &refnum);
	if (noErr != err) {
		show_err(err);
		return;
	}
	count = data->count;
	err = FSWrite(refnum, &count, (void *)data->first);
	if (noErr != err) {
		show_err(err);
		goto close;
	}
close:
	err = FSClose(refnum);
	if (noErr != err) {
		show_err(err);
		return;
	}
	SetCursor(&arrow);
}

static void flash_dialog_button(DialogPtr dialog, short item) {
	Handle handle;
	short item_type;
	Rect rect;
	long ignore;

	GetDItem(dialog, item, &item_type, &handle, &rect);
	if (item_type == ctrlItem | btnCtrl) {
		HiliteControl((ControlHandle)handle, k_hilite);
		Delay(k_visual_delay, &ignore);
		HiliteControl((ControlHandle)handle, k_unhilite);
	}
}

static pascal void draw_default_button_outline(WindowPtr window, short item) {
	GrafPtr port;
	Handle handle;
	short item_type;
	PenState pen_state;
	Rect rect;

	GetDItem(window, item, &item_type, &handle, &rect);
	GetPort(&port);
	SetPort(window);
	GetPenState(&pen_state);
	PenNormal();
	PenSize(k_default_button_outline_thickness, k_default_button_outline_thickness);
	FrameRoundRect(&rect, k_default_button_outline_curve, k_default_button_outline_curve);
	SetPenState(&pen_state);
	SetPort(port);
}

static void num_to_hex_string(long number, Str255 string) {
	short i;
	short j;
	short len;
	char remainder;
	Boolean negative;

	/* Build the string in reverse order in an otherwise unused later section of the string. */
	i = 10;
	j = 0;
	len = 0;
	negative = number < 0;
	if (negative) number *= -1;
	do {
		++i;
		++len;
		remainder = number % 16;
		if (remainder < 10) {
			remainder += '0';
		} else {
			remainder += 'A' - 10;
		}
		string[i] = remainder;
		number >>= 4;
	} while (number > 0);
	if (negative) {
		++i;
		++len;
		string[i] = '-';
	}

	/* Reverse the string into the correct order. */
	while (j < len) {
		string[++j] = string[i--];
	}
	string[0] = len;
}

static void hex_string_to_num(Str255 string, long *number) {
	short i;
	short len;
	long result;
	char digit;
	Boolean negative;

	result = 0;
	len = string[0];
	negative = len && '-' == string[1];
	for (i = 1; i <= len; ++i) {
		digit = string[i];
		if (digit >= '0' && digit <= '9') {
			digit -= '0';
		} else if (digit >= 'A' && digit <= 'F') {
			digit -= 'A' - 10;
		} else if (digit >= 'a' && digit <= 'f') {
			digit -= 'a' - 10;
		} else {
			continue;
		}
		result <<= 4;
		result += digit;
	}
	if (negative) result *= -1;
	if (number) *number = result;
}

static void set_dialog_item_text_to_number(DialogPtr dialog, short item, long number) {
	struct data *data;
	short item_type;
	Handle handle;
	Rect rect;
	Str255 string;

	data = (struct data *)GetWRefCon(dialog);
	GetDItem(dialog, item, &item_type, &handle, &rect);
	item_type &= ~itemDisable;
	if (statText == item_type || editText == item_type) {
		if (data->hex) {
			num_to_hex_string(number, string);
		} else {
			NumToString(number, string);
		}
		SetIText(handle, string);
	}
}

static long get_dialog_item_text_as_number(DialogPtr dialog, short item) {
	struct data *data;
	short item_type;
	Handle handle;
	Rect rect;
	Str255 string;
	long number;

	data = (struct data *)GetWRefCon(dialog);
	GetDItem(dialog, item, &item_type, &handle, &rect);
	item_type &= ~itemDisable;
	if (statText == item_type || editText == item_type) {
		GetIText(handle, string);
		if (data->hex) {
			hex_string_to_num(string, &number);
		} else {
			StringToNum(string, &number);
		}
	} else {
		number = 0;
	}

	return number;
}

static void update_controls(DialogPtr dialog) {
	struct data *data;
	short item_type;
	Handle handle;
	Rect rect;
	short focused_text_item;

	data = (struct data *)GetWRefCon(dialog);

	if (data->hex != data->old_hex) {
		GetDItem(dialog, i_decimal, &item_type, &handle, &rect);
		SetCtlValue((ControlHandle)handle, !data->hex);
		GetDItem(dialog, i_hexadecimal, &item_type, &handle, &rect);
		SetCtlValue((ControlHandle)handle, data->hex);
		data->ram_size_dirty = data->count_dirty = data->first_dirty = data->last_dirty = true;
	}
	if (data->ram_size_dirty) {
		set_dialog_item_text_to_number(dialog, i_ram_size, data->ram_size);
		data->ram_size_dirty = false;
	}
	if (data->count_dirty) {
		set_dialog_item_text_to_number(dialog, i_dump_size, data->count);
		data->count_dirty = false;
	}
	if (data->first_dirty) {
		set_dialog_item_text_to_number(dialog, i_first, data->first);
		data->first_dirty = false;
	}
	if (data->last_dirty) {
		set_dialog_item_text_to_number(dialog, i_last, data->first + data->count - 1);
		data->last_dirty = false;
	}
	if (data->hex != data->old_hex) {
		focused_text_item = ((DialogPeek)dialog)->editField + 1;
		if (focused_text_item > 0) {
			SelIText(dialog, focused_text_item, k_min_selection, k_max_selection);
		}
		data->old_hex = data->hex;
	}
}

static pascal Boolean dialog_filter(DialogPtr dialog, EventRecord *event, short *item) {
	struct data *data;
	short focused_text_item;
	Boolean handled;
	unsigned char key;

	data = (struct data *)GetWRefCon(dialog);

	if (data->keypress) {
		focused_text_item = ((DialogPeek)dialog)->editField + 1;
		switch (focused_text_item) {
			case i_dump_size:
				data->count = get_dialog_item_text_as_number(dialog, i_dump_size);
				data->last_dirty = true;
				break;
			case i_first:
				data->first = get_dialog_item_text_as_number(dialog, i_first);
				data->last_dirty = true;
				break;
			case i_last:
				data->count = get_dialog_item_text_as_number(dialog, i_last) - data->first + 1;
				data->count_dirty = true;
				break;
		}
		update_controls(dialog);
		data->keypress = false;
	}

	handled = false;
	switch (event->what) {
		case autoKey:
		case keyDown:
			key = event->message & charCodeMask;
			if (k_return == key || k_enter == key) {
				if (item) *item = i_ok;
				flash_dialog_button(dialog, i_ok);
				handled = true;
			} else if (k_escape == key || ('.' == key && (event->modifiers & cmdKey))) {
				if (item) *item = i_cancel;
				flash_dialog_button(dialog, i_cancel);
				handled = true;
			} else if (key < 32) {
				/* allow control characters (tab, delete, arrow keys, etc.) */
			} else if (data->hex && (key >= 'A' && key <= 'F')) {
				/* allow hex letters in hex mode */
			} else if (data->hex && (key >= 'a' && key <= 'f')) {
				/* convert lowercase to uppercase */
				event->message = (event->message & ~charCodeMask) | (key - ('a' - 'A'));
			} else if (key < '0' || key > '9') {
				/* pretend other keystrokes didn't happen */
				NoteAlert(r_bad_key_alert, nil);
				event->what = nullEvent;
			}
			data->keypress = keyDown == event->what && !handled;
			break;
	}
	return handled;
}

static Ptr get_top_mem(void) {
	return (has_trap(_OSDispatch) && !has_switcher()) ? MFTopMem() : TopMem();
}

static void make_default_filename(struct data *data, Str255 filename) {
	Str255 string;

	GetIndString(filename, r_file_strings, i_default_filename_start);
	num_to_hex_string(data->first, string);
	append_string(filename, string);
	GetIndString(string, r_file_strings, i_default_filename_middle);
	append_string(filename, string);
	num_to_hex_string(data->first + data->count - 1, string);
	append_string(filename, string);
	GetIndString(string, r_file_strings, i_default_filename_end);
	append_string(filename, string);
}

static Boolean get_user_input(struct data *data) {
	DialogPtr dialog;
	Boolean hex;
	short item;
	short item_type;
	Handle handle;
	long max_ram;
	Rect rect;
	Point top_left;
	Str255 prompt;
	Str255 default_filename;

	dialog = GetNewDialog(r_dialog, nil, k_in_front);
	if (!dialog) return;
	SetWRefCon(dialog, data);

	data->keypress = false;
	data->first = 0;
	data->count = data->ram_size = (long)get_top_mem();
	data->hex = true;
	data->old_hex = !data->hex;

	GetDItem(dialog, i_default_button_outline, &item_type, &handle, &rect);
	SetDItem(dialog, i_default_button_outline, item_type, (Handle)&draw_default_button_outline, &rect);
	update_controls(dialog);
	ShowWindow(dialog);

	GetIndString(prompt, r_file_strings, i_prompt);
	do {
		ModalDialog(&dialog_filter, &item);
		switch (item) {
			case i_ok:
				if (data->count <= 0) {
					show_err(e_bad_count);
					item = 0;
				} else if (data->first < 0) {
					show_err(e_bad_start);
					item = 0;
				} else {
					top_left.h = k_sfputfile_left;
					top_left.v = k_sfputfile_top;
					make_default_filename(data, default_filename);
					SFPutFile(top_left, prompt, default_filename, nil, &data->reply);
					if (!data->reply.good) item = 0;
				}
				break;
			case i_decimal:
			case i_hexadecimal:
				data->hex = i_hexadecimal == item;
				update_controls(dialog);
				break;
		}
	} while (i_ok != item && i_cancel != item);

	DisposDialog(dialog);

	return i_ok == item;
}

static void init(void) {
	MaxApplZone();
	InitGraf(&thePort);
	InitFonts();
	InitWindows();
	InitMenus();
	TEInit();
	InitDialogs(nil);
	FlushEvents(everyEvent, 0);
	InitCursor();
}

void main(void) {
	struct data data;

	init();
	if (get_user_input(&data)) {
		save_data(&data);
	}
}
