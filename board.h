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
	KbdFuncs  functions;
public:
	Controller(){
		cols = 0;
		rows = 0;
	}

	bool	  LoadFile();

	void      AddLayer(wxXmlNode *node);
	KbdLayers GetLayers();

	void      LoadKeyCodes(wxXmlNode *node);
	KbdLayout GetKeyCodes();
	int	  GetKeyCode(int code);
	int	  GetKeyCode(wxString label);

	void      AddConfig(wxXmlNode *node);
	Configs   GetConfigs();			     // for display
	KbdConfig GetConfig(wxString option);
	void      SetConfig(wxString option, wxString value);  // set option
	int	  DownloadConfig();
	int	  UploadConfig();
};

class Board {
private:
	wxString name;
	wxString controller;
	Controller *ctrl;
	int nr_rows;
	int nr_cols;
	KbdLayout layout;
public:
	Board(){
		nr_rows = 0;
		nr_cols = 0;
	}
	bool	   LoadFile(wxString filename);
	bool	   LoadRows(wxXmlNode *parent);
	void	   AddKey(int size, int row, int col, wxString sCode);
	void	   AddSapce(int size);
	KbdLayout *GetLayout(){ return layout; }
};

class BoardPool {
};
