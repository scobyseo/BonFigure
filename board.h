/* board.h
 *  (C) Seo Bon Keun, 2017
 */

#include <wx/list.h>

//namespace bonkb;

typedef enum { KEY_NORMAL, KEY_BLANK, KEY_NEWLINE } keytype_t;
struct Key {
	keytype_t type;
	wxString  label;
	wxString  comment;
	float width;
	float height;
	int   code;     /* valid for keycode view */
	int   row, col;	/* matrix related : valid for layer view */
};

WX_DECLARE_LIST(struct Key, KbdLayout);

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

typedef enum {
	KBD_STATE_NORMAL, KBD_STATE_MAPPER, KBD_STATE_LOADER
} KbdState_t;

class Controller {
private:
	wxString  name;
	wxString  author;
	wxString  url;
	KbdLayers layers;
	KbdLayout keycodes;
	Configs   configs;
	KbdFuncs  functions;
	wxArrayString board_names;
	wxArrayString board_paths;
	int	  cols, rows, nr_layers;
	int	  ***keymap;

public:
	Controller(){ }

	bool	  LoadFile(wxString fname);
	virtual bool LoadFile() { /* Inherit this for group loading */ }

	void      LoadLayers(wxXmlNode *node);
	KbdLayers GetLayer();

	void      LoadKeyCodes(wxXmlNode *node);
	KbdLayout GetKeyCodes();
	int	  GetKeyCode(wxString label);
	wxString  GetKeyLabel(int code);

	int	  SetKeyMap(int row, int col, int code);
	int	  SetKeyMap(int row, int col, wxString label);

	void      LoadConfig(wxXmlNode *node);
	Configs   GetConfigs();			     // for display
	KbdConfig GetConfig(wxString option);
	void      SetConfig(wxString option, wxString value);  // set option

	void	  LoadBoards(wxXmlNode *node);
	Board	 *GetBoard(wxString name);
	wxArrayString  GetBoardList();

	bool  IsA(int vid, int pid) {
		return (this->vid == vid) && (this->pid == pid);
	}

	virtual int  SetKdbState(KbdState_t &state);
	virtual int  LoadKeyMap(wxString fname);
	virtual int  SaveKeyMap(wxString fname);
	virtual int  RetrieveKeyMap() {}
	virtual int  ApplyKeyMap() {}
	virtual int  RetriveConfig() {}
	virtual int  ApplyConfig() {}
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
	Board(Controller *ctrl){ this->ctrl = ctrl; }

	bool	    LoadFile(wxString filename);
	bool	    LoadLayout(wxXmlNode *parent);
	void	    AddKey(keytype_t type, wxXmlNode *node);
	KbdLayout  *GetLayout(){ return &layout; }
	wxString    GetName();
};

WX_DECLARE_LIST(Board, BoardList);
WX_DECLARE_LIST(Controller, ControllerList);

class HexFile {
private:
	char *buf;
	int base_addr;
	int length;
	int current;

public:
	HexFile(int base_addr) { this->base_addr = base_addr; }
	bool Read(wxString fname);
	//bool Write(wxString fname);
	char FirstChar() { current = 0; return buf[0]; }
	char GetNextChar() { return (current < length) ? buf[current++] : 0; }
};

class BoardPool {
private:
	BoardList   boards;
	Controller  ctrls[] = { ps2avrGB };

public:
	int	    LoadControllers();
	Controller *GetController(wxString name);
	Controller *GetController(int mid, int pid);

	int	    LoadBoards();
	Board       GetBoard(wxString name);
	wxArrayString  GetBoardList(int mid, int pid);
};

extern BoardPool boards;	/* global board pool */
