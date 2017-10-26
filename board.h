/* board.h
 *  (C) Seo Bon Keun, 2017
 */
#include <wx/list.h>

struct Key {
	enum KeyType { KEY_NORMAL, KEY_BLANK, KEY_LED } type;
	wxString label;
	wxString comment;
	float width;
	float height;
	int   code;     /* valid for keycode view */
	int   row, col;	/* matrix related : valid for layer view */
};

WX_DECLARE_LIST(struct Key, KeyRow);
WX_DECLARE_LIST(KeyRow, KbdLayout);

struct KbdConfig {
	wxString name;
	enum { OptNumber, OptSelection, OptColor, OptArray } type;
	int min, max;
	int value;
	wxString *items;
	wxColor   color;
};

WX_DECLARE_LIST(struct KbdConfig, KbdConfigs);

struct KbdFunction {
	wxString name;
	wxString comment;
}

WX_DECLARE_LIST(struct KbdFunction, KbdFuncs);

struct KbdLayer {
	int id;
	wxString name;
	wxString comment;
};

WX_DECLARE_LIST(struct KbdLayer, KbdLayers);

class Controller {
private:
	int cols, rows;
	wxString  name;
	wxString  author;
	wxString  url;
	KbdLayers layers;
	KbdLayout keycodes;
	Configs   configs;
	Funcs     functions;
public:
	Controller(){
	}

	AddLayer(wxXmlNode *node);
	GetLayers();

	AddKeyCode(wxXmlNode *node);
	GetKeyCodes();
	GetKeyCode(int code);
	GetKeyCode(wxString label);

	AddConfig(wxXmlNode *node);
	GetConfigs();				     // for display
	GetConfig(wxString option);
	SetConfig(wxString option, wxString value);  // set option
	DownloadConfig();
	UploadConfig();
};

class Board {
private:
	wxString name;
	wxString controller;
	Controller *ctrl;
	int nr_rows, max_rows;
	int nr_cols, max_cols;
	struct kbdRowList *layout;
	// row - keys
public:
	Board(){
		int i;
		nr_rows = 0;
		nr_cols = 0;
		max_rows = 8;
		max_cols = 25;
		layout = new struct KeyItem[max_rows][max_cols];
	}
	bool LoadFile(wxString filename);
	bool LoadRows(wxXmlNode *parent);
	void AddKey(int size, int row, int col, wxString sCode);
	void AddSapce(int size);
};
