#include <wx/list.h>

struct KeyCode {
	int      code;
	wxString label;
	wxString comment;
};

WX_DECLARE_LIST(struct KeyCode, KeyCodeRow);
WX_DECLARE_LIST(KeyCodeRow, KeyCodeLayout);

struct KbdConfig {
	wxString name;
	enum { OptNumber, OptSelection, OptColor, OptArray } type;
	int min, max;
	int value;
};

WX_DECLARE_LIST(struct KbdConfig, KbdConfigs);

struct KbdLayer {
	int id;
	wxString name;
	wxString comment;
};

WX_DECLARE_LIST(struct KbdLayer, KbdLayers);

/* ------------------- .cpp --------------------- */
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(KeyCodeRow);
WX_DEFINE_LIST(KeyCodeLayout);
WX_DEFINE_LIST(KbdConfigs);
WX_DEFINE_LIST(KbdLayers);

class Controller {
private:
	int cols, rows;
	wxString name;
	wxString author;
	wxString url;
	// Layers
	int nr_layers, max_layers;
	struct Layer *layer;
	// KeyCodes
	int nr_codes, max_codes;
	int nr_code_rows;
	struct KeyCode *codes;
	// Configurations
	int nr_confs, max_confs;
	struct Config *conf;
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

Controller::AddLayer(wxXmlNode *node)
{
	if (nr_layers == max_layers){
		max_layers *= 2;
		struct Layer *layers = new struct Layer[max_layers];
		memcpy(layers, this->layers, nr_layers * sizeof(struct Layer));
		delete(this->layers);
		this->layers = layers;
	}

	layers[nr_layers].id      = node->GetAttribute("id", "-1");
	layers[nr_layers].name    = node->GetAttribute("name", "");
	layers[nr_layers].comment = node->GetAttribute("comment", "");

	nr_layers ++;
}

Controller::GetLayer(int id)
{
	return layers[id];
}

Controller::LoadKeyCodes(wxXmlNode *node)
{
	wxXmlNode *child;
	KeyCodeRow *row = new KeyCodeRow;

	for (child = node->GetChildren(); child; child = child->GetNext()){
		wxString sName = child->GetName();
		if (sName == "key"){
			struct KeyCode *key = new struct KeyCode;
			child->GetAttribute("code", "255").ToLong(&key->code);
			key->label   = child->GetAttribute("label", "");
			key->comment = child->GetNodeContent();
			row->Append(key);
		} else if (sName == "newline"){
			codeLayout.Append(row);
			row = new KeyCodeRow;
		}
	}
	codeLayout.Append(row);
}

Controller::LoadConfig(wxXmlNode *node)
{
	wxXmlNode *child;
	struct KbdConfig *config;

	for (child = node->GetChildren(); child; child = child->GetNext()){
		wxString sName = child->GetName();
		if (sName != "option")
			continue;

		config = new struct KbdConfig;
		config->name   = child->GetAttribute("name", "");
		wxString sType = child->GetAttribute("type", "");
		if (sType == "number"){
			config->type = OptNumber;
			child->GetAttribute("min",   "0").ToLong(&config->min);
			child->GetAttribute("max", "255").ToLong(&config->max);
		} else if (sType == "color"){
			config->type = OptColor;
		} else if (sType == "selection"){
			config->type = OptSelection;
			wxXmlNode *opt = child->GetChildren();
			while(opt){
				opt = opt->GetNext();
			}
		} else if (sType == "array"){
			config->type = OptArray;
			config->min  = 0;
			child->GetAttribute("size", "0").ToLong(&config->max);
		}
		row->Append(key);
	}
	codeLayout.Append(row);
}

Controller::LoadFile(wxString filename)
{
	wxXmlDocument doc;
	if (!doc.Load(filename))
		return false;

	if (doc.GetRoot()->GetName() != "controller")
		return false;

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	while (child){
		if (child->GetName() == "name") {
			this->name = child->GetNodeContent();
		} else if (child->GetName() == "author") {
			this->author = child->GetNodeContent();
		} else if (child->GetName() == "url") {
			this->url = child->GetNodeContent();
		} else if (child->GetName() == "matrix") {
			wxString sRows = child->GetAttribute("rows", "0");
			wxString sCols = child->GetAttribute("cols", "0");
			sRows.ToLong(&this->rows);
			sCols.ToLong(&this->cols);
		} else if (child->GetName() == "layers") {
			wxString sSize = child->GetAttribute("size", "0");
			int size;
			sSize.ToLong(&size);
			wxXmlNode *node = child->GetChildren();
			while(node){
				AddLayer(node);
				node->GetNext();
			}
		} else if (child->GetName() == "keycodes") {
			LoadKeyCodes(child);
		} else if (child->GetName() == "configurations") {
			LoadConfigurations(child);
		} else if (child->GetName() == "functions") {
			LoadFunctions(child);
		} else { /* DO NOTHING */ }
		child = child->GetNext();
	}
}

class Board {
private:
	wxString name;
	wxString controller;
	wxList<
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

Board::AddKey(wxXmlNode *node)
{
	wxString sSize, sRow, sCol, sCode;
	long size, row, col;
	sSize = node->GetAttribute("size", "1");
	sRow  = node->GetAttribute("col",  "-1");
	sCol  = node->GetAttribute("row",  "-1");
	sCode = node->GetAttribute("keycode", " "); // unmapped key code
	sSize.ToLong(&size);
	sRow.ToLong(&row);
	sCol.ToLong(&col);

	layout.
}

void Board::AddSpace(wxXmlNode *node)
{
	wxString sSize;
	long size;
	sSize = node->GetAttribute("size", "1");
	sSize.ToLong(&size);
}

bool Board::LoadRows(wxXmlNode *parent)
{
	wxXmlNode *keyNode;
	for (keyNode = parent->GetChildren(); keyNode; keyNode = keyNode->GetNext()){
		if (keyNode->GetName == "key"){
			AddKey(keyNode);
		} else if (keyNode->GetName == "space"){
			AddSpace(keyNode);
		} else { /* DO NOTHING */ }
	}
	return true;
}

Board::LoadFile(wxString filename)
{
	wxXmlDocument doc;
	if (!doc.Load(filename))
		return false;

	if (doc.GetRoot()->GetName() != "board")
		return false;

	wxXmlNode *child = doc.GetRoot()->GetChildren();
	while (child){
		if (child->GetName() == "name") {
			this->name = child->GetNodeContent();
		} else if (child->GetName() == "controller") {
			this->controller = child->GetNodeContent();
		} else if (child->GetName() == "row") {
			loadRows(child);
		} else { /* DO NOTHING */ }
		child = child->GetNext();
	}
}
