/*
SPDX-FileCopyrightText: © 2025 Ryan Carsten Schmidt <https://github.com/ryandesign>
SPDX-License-Identifier: MIT
*/

#ifndef nil
#define nil ((void *)0)
#endif

#define r_file_strings 128
#define i_prompt 1
#define i_default_filename 2

#define r_error_strings 129
#define e_range 1

#define r_dialog 128
#define i_ok 1
#define i_cancel 2
#define i_default_button_outline 3
#define i_ram_size 6
#define i_start 8
#define i_end 10

#define r_error_number_alert 129
#define r_error_string_alert 130

#define k_enter 0x03
#define k_escape 0x1b
#define k_return 0x0d

#define k_hilite 1
#define k_unhilite 0

#define k_visual_delay 8

#define k_default_button_outline_thickness 3
#define k_default_button_outline_curve 16

#define k_max_selection 32767

#define k_in_front ((WindowPtr)-1)

#define k_sfputfile_left 104
#define k_sfputfile_top 80

#define k_creator_code '????'
#define k_type_code '????'

struct data {
	long start;
	long end;
	SFReply reply;
};

static void debug_num(long number) {
	Str255 string;

	NumToString(number, string);
	DebugStr(string);
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
	count = data->end - data->start;
	err = FSWrite(refnum, &count, (void *)data->start);
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

static pascal Boolean dialog_filter(DialogPtr dialog, EventRecord *event, short *item) {
	Boolean handled;
	char key;

	handled = false;
	switch (event->what) {
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
				/* allow control characters like tab */
			} else if (key < '0' || key > '9') {
				event->what = nullEvent;
			}
			break;
	}
	return handled;
}

static void set_dialog_item_text_to_number(DialogPtr dialog, short item, long number) {
	short item_type;
	Handle handle;
	Rect rect;
	Str255 string;

	GetDItem(dialog, item, &item_type, &handle, &rect);
	item_type &= ~itemDisable;
	if (statText == item_type || editText == item_type) {
		NumToString(number, string);
		SetIText(handle, string);
	}
}

static long get_dialog_item_text_as_number(DialogPtr dialog, short item) {
	short item_type;
	Handle handle;
	Rect rect;
	Str255 string;
	long number;

	GetDItem(dialog, item, &item_type, &handle, &rect);
	item_type &= ~itemDisable;
	if (statText == item_type || editText == item_type) {
		GetIText(handle, string);
		StringToNum(string, &number);
	} else {
		number = 0;
	}

	return number;
}

static Boolean get_user_input(struct data *data) {
	DialogPtr dialog;
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

	GetDItem(dialog, i_default_button_outline, &item_type, &handle, &rect);
	SetDItem(dialog, i_default_button_outline, item_type, (Handle)&draw_default_button_outline, &rect);

	max_ram = (long)TopMem(); /* todo: wrong under multifinder */
	set_dialog_item_text_to_number(dialog, i_ram_size, max_ram);
	set_dialog_item_text_to_number(dialog, i_start, 0);
	set_dialog_item_text_to_number(dialog, i_end, max_ram);
	SelIText(dialog, i_start, 0, k_max_selection);
	ShowWindow(dialog);

	GetIndString(prompt, r_file_strings, i_prompt);
	GetIndString(default_filename, r_file_strings, i_default_filename);
	do {
		ModalDialog(&dialog_filter, &item);
		if (i_ok == item) {
			data->start = get_dialog_item_text_as_number(dialog, i_start);
			data->end = get_dialog_item_text_as_number(dialog, i_end);
			if (data->end <= data->start) {
				show_err(e_range);
				item = 0;
			} else {
				top_left.h = k_sfputfile_left;
				top_left.v = k_sfputfile_top;
				SFPutFile(top_left, prompt, default_filename, nil, &data->reply);
				if (!data->reply.good) item = 0;
			}
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
