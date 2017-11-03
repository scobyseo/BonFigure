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
	int	  cols, rows, nr_layers;
	int	  **keymap;

public:
	Controller(){
		cols = 0;
		rows = 0;
	}

	bool	  LoadFile();

	void      LoadLayer(wxXmlNode *node);
	KbdLayers GetLayers();

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

	bool	  IsA(wxString name);
	bool	  IsA(int mid, int pid);

	virtual int  SetKdbState(KbdState_t &state);
	virtual int  LoadKeyMap();
	virtual int  SaveKeyMap();
	virtual int  RetrieveKeyMap();
	virtual int  ApplyKeyMap();
	virtual int  RetriveConfig();
	virtual int  ApplyConfig();
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
	bool	    LoadFile(wxString filename);
	bool	    LoadRows(wxXmlNode *parent);
	void	    AddKey(int size, int row, int col, wxString sCode);
	void	    AddSapce(int size);
	KbdLayout  *GetLayout(){ return layout; }
	Controller *GetController(){ return ctrl; }
	wxString    GetName();
	bool	    IsA(wxString name);
};

WX_DECLARE_LIST(Board, BoardList);
WX_DECLARE_LIST(Controller, ControllerList);

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


