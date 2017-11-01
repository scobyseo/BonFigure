/* board.cpp 
 *  (C) Seo Bon Keun, 2017
 */
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(KeyCodeRow);
WX_DEFINE_LIST(KeyCodeLayout);
WX_DEFINE_LIST(KbdConfigs);
WX_DEFINE_LIST(KbdLayers);


Controller::AddLayer(wxXmlNode *node)
{
	struct KbdLayer *layer = new struct KbdLayer;

	layer->id      = node->GetAttribute("id", "-1");
	layer->name    = node->GetAttribute("name", "");
	layer->comment = node->GetAttribute("comment", "");
	layers.Append(layer);
}

Controller::GetLayer(int id)
{
	return layers;
}

Controller::LoadKeyCodes(wxXmlNode *node)
{
	wxXmlNode *child;
	KeyRow *row = new KeyRow;

	for (child = node->GetChildren(); child; child = child->GetNext()){
		wxString sName = child->GetName();
		if (sName == "key"){
			struct Key *key = new struct Key;
			child->GetAttribute("code", "255").ToLong(&key->code);
			key->label   = child->GetAttribute("label", "");
			key->comment = child->GetNodeContent();
			key->type    = KEY_NORMAL;
			key->width   = 1.0;
			key->height  = 1.0;
			key->row     = -1;
			key->col     = -1;
			row->Append(key);
		} else if (sName == "newline"){
			keycodes.Append(row);
			row = new KeyRow;
		}
	}
	keycodes.Append(row);
}

Controller::LoadConfigs(wxXmlNode *node)
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
			int i = 0;
			config->type = OptSelection;
			config->min  = 0;
			child->GetAttribute("size", "0").ToLong(&config->max);
			config->items = new wxString[config->max];
			wxXmlNode *opt = child->GetChildren();
			while(opt){
				config->items[i++] = opt->GetNodeContent();
				opt = opt->GetNext();
			}
		} else if (sType == "array"){
			config->type = OptArray;
			config->min  = 0;
			child->GetAttribute("size", "0").ToLong(&config->max);
		} else { /* DO NOTHING */ }
		configs.Append(key);
	}
}

Controller::LoadFile()
{
	wxXmlDocument doc;
	if (!doc.Load("layouts/ps2avrGB.ctl"))
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
			LoadConfigs(child);
		} else if (child->GetName() == "functions") {
			LoadFunctions(child);
		} else { /* DO NOTHING */ }
		child = child->GetNext();
	}
}

/* ----------------------- BOARD ------------------------ */

Board::AddKey(wxXmlNode *node)
{
	struct Key *key = new struct Key;

	key->type    = KEY_NORMAL;
	key->label   = node->GetAttribute("keycode", " "); // unmapped key code
	// key->code = controller->LookupKeycode(key->label);
	key->comment = node->GetNodeContent();
	key->height  = 1.0;
	node->GetAttribute("size", "1").ToDouble(&key->width);
	node->GetAttribute("col", "-1").ToLong(&key->col);
	node->GetAttribute("row", "-1").ToLong(&key->row);
}

void Board::AddSpace(wxXmlNode *node)
{
	struct Key *key = new struct Key;

	key->type    = KEY_BLANK;
	key->label   = "";
	// key->code = -1;
	key->comment = node->GetNodeContent();
	key->col     = -1;
	key->row     = -1;
	key->height  = 1.0;
	node->GetAttribute("size", "1").ToDouble(&key->width);
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

wxString Board::LoadFile(wxString filename)
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

	return this->controller;
}

void Board::SetController(Controller *ctrl)
{
	this->ctrl = ctrl;
}

WX_DECLARE_LIST(Board, BoardList);
WX_DECLARE_LIST(Controller, ControllerList);

BoardPool {
private:
	BoardList      boards;
	Controller     ctrls[] = { ps2avrGB };
public:
	LoadControllers();
	LoadBoards();
};


int BoardPool::LoadControllers()
{
	int i, nr_ctrls = sizeof(ctrls) / sizeof(Controller);
	for (i = 0; i < nr_ctrls; i ++){
		ctrls[i].LoadFile();
	}
	return 0;
}

Controller *BoardPool::GetController(wxString name)
{
	ControllerList::iterator iter;
	for (iter = ctrls.begin(); iter != ctrls.end(); ++iter){
		Controller *ctl = iter;
		if (ctl->name == name){
			return ctl;
		}
	}
}

int BoardPool::LoadBoards()
{
	wxFileSystem fs;
	wsString pwd = fs.GetPath();
	fs.ChangePathTo("layouts", true);
	wxString layoutPath = fs.GetPath();
	wxString fname = fs.FindFirst("*.board", wxFILE);
	while (fname != ""){
		Board *brd = new Board();
		wxString controller = brd->LoadFile(fname);
		brd->SetController(GetController(controller));
		boards.Append(brd);
		fname = fs.FindNext();
	}
	return 0;
}

Board *BoardPool::GetBoard(wxString name)
{
	BoardList::iterator iter;
	for (iter = boards.begin(); iter != boards.end(); ++iter){
		Boards *brd = iter;
		if (brd->name == name){
			return brd;
		}
	}
}
