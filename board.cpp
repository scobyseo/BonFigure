/* board.cpp 
 *  (C) Seo Bon Keun, 2017
 */
#include <wx/listimpl.cpp>

WX_DEFINE_LIST(KeyCodeRow);
WX_DEFINE_LIST(KeyCodeLayout);
WX_DEFINE_LIST(KbdConfigs);
WX_DEFINE_LIST(KbdLayers);


void Controller::LoadLayers(wxXmlNode *node)
{
	wxXmlNode *child = node->GetChildren();
	while(child){
		struct KbdLayer *layer = new struct KbdLayer;
		layer->id      = child->GetAttribute("id", "-1");
		layer->name    = child->GetAttribute("name", "");
		layer->comment = child->GetAttribute("comment", "");
		layers.Append(layer);
		child->GetNext();
	}
}

wxString Controller::GetLayer(int id)
{
	return layers;
}

void Controller::LoadKeyCodes(wxXmlNode *node)
{
	wxXmlNode *child;

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
			keycodes.Append(key);
		} else if (sName == "newline"){
			keycodes.Append(row);
			row = new KeyRow;
		}
	}
}

void Controller::LoadConfigs(wxXmlNode *node)
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

void Controller::LoadBoards(wxXmlNode *node)
{
	wxXmlNode *child;
	struct Board *board;

	for (child = node->GetChildren(); child; child = child->GetNext()){
		if (child->GetName() != "board") continue;
		wxString name = child->GetAttribute("name", ""); /* NO USE */
		wxString path = child->GetAttribute("path", "");
		board_names.Add(name);
		board_paths.Add(path);
	}
}

bool Controller::LoadFile(wxString fname)
{
	wxXmlDocument doc;
	if (!doc.Load(fname))
		return false;

	wxXmlNode *root = doc.GetRoot();
	if (root->GetName() != "controller")
		return false;

	wxXmlNode *child = root->GetChildren();
	while (child){
		if (child->GetName() == "name") {
			this->name = child->GetNodeContent();
		} else if (child->GetName() == "author") {
			this->author = child->GetNodeContent();
		} else if (child->GetName() == "url") {
			this->url = child->GetNodeContent();
		} else if (child->GetName() == "matrix") {
			child->GetAttribute("rows", "0").ToLong(&this->rows);
			child->GetAttribute("cols", "0").ToLong(&this->cols);
		} else if (child->GetName() == "layers") {
			wxString sSize = child->GetAttribute("size", "0");
			sSize.ToLong(&this->nr_layers);
			LoadLayers(child);
		} else if (child->GetName() == "keycodes") {
			LoadKeyCodes(child);
		} else if (child->GetName() == "configurations") {
			LoadConfigs(child);
		} else if (child->GetName() == "functions") {
			LoadFunctions(child);
		} else if (child->GetName() == "boards") {
			LoadBoards(child);
		} else { /* DO NOTHING */ }
		child = child->GetNext();
	}
}

Board *Controller::GetBoard(wxString name)
{
	for (int i = 0; i < board_names.GetCount(); i ++){
		if (name != board_names[i]) continue;
		Board *board = new Board(this);
		if ( board->LoadFile(board_paths[i]) ){
			return board;
		} else {
			delete(board);
			return NULL;
		}
	}
	return NULL;
}

wxArrayString Controller::GetBoardList()
{
	return board_names;
}

Configs Controller::GetConfigs()
{
	return configs;
}

KbdConfig *Controller::GetConfig(wxString option)
{
	for (Configs::iterator *iter = configs.begin();
			iter != configs.end(); iter++){
		KbdConfig *config = *iter;
		if (config->name == option){
			return config;
		}
	}
	return NULL;
}

void Controller::SetConfig(wxString option, wxString value)
{
	for (Configs::iterator *iter = configs.begin();
			iter != configs.end(); iter++){
		KbdConfig *config = *iter;
		if (config->name != option) continue;
		if (config->type == OptNumber){
			long v;
			value.ToInt(&v);
			config->value = v;
		} else if (config->type == OptSelection){
		} else if (config->type == OptColor){
		} else if (config->type == OptArray){
		}
	}
}

int Controller::LoadKeyMap(wxString fname)
{
	//  TODO: add base_address / size
	HexFile f = new HexFile(0);
	if (!this->keymap)
		this->keymap = new (int**)[this->nr_layers];

	f.FirstChar();
	for (l = 0; l < this->nr_layers; l ++){
		if (this->keymap[l] == NULL)
			this->keymap[l] = new (int*)[this->rows];
		for (r = 0; r < this->rows; r ++){
			if (this->keymap[l][r] == NULL)
				this->keymap[l][r] = new int[this->cols];
			for (c = 0; c < this->cols; c ++){
				this->keymap[l][r][c] = f.GetNextChar();
			}	
		}	
	}
}

int Controller::SaveKeyMap(wxString fname)
{
}

/************************* Board Class Implementation ************************/

bool Board::AddKey(keytype_t type, wxXmlNode *node)
{
	struct Key *key = new struct Key;

	key->type    = type;
	key->label   = node->GetAttribute("keycode", " "); // unmapped key code
	// key->code = ctrl->LookupKeycode(key->label);
	key->comment = node->GetNodeContent();
	key->height  = 1.0;
	node->GetAttribute("size", "1").ToDouble(&key->width);
	if (type == KEY_NORMAL){
		node->GetAttribute("col", "-1").ToLong(&key->col);
		node->GetAttribute("row", "-1").ToLong(&key->row);
	} else {
		key->col     = -1;
		key->row     = -1;
	}
	layout.Append(key);
}

bool Board::LoadLayout(wxXmlNode *parent)
{
	wxXmlNode *keyNode;
	for (keyNode = parent->GetChildren(); keyNode; keyNode = keyNode->GetNext()){
		if (keyNode->GetName == "key"){
			AddKey(KEY_NORMAL, keyNode);
		} else if (keyNode->GetName == "space"){
			AddKey(KEY_BLANK, keyNode);
		} else if (keyNode->GetName == "newline"){
			AddKey(KEY_NEWLINE, keyNode);
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
		} else if (child->GetName() == "layout") {
			loadLayout(child);
		} else { /* DO NOTHING */ }
		child = child->GetNext();
	}

	return this->controller;
}

bool HexFile::Read(wxString fname)
{
	wxTextFile f;
	if (!f.Open(fname, wxFile::read))
		return false;

	for (wxString s = f.GetFirstLine();
			!f.Eof(); s = f.GetNextLine()){
		if ( s(0,1) != ":" ) break;
		long addr, len, type, checksum;
		s(1,2).ToLong(&len,  16);
		s(3,4).ToLong(&addr, 16);
		s(7,2).ToLong(&type, 16);
		checksum = len + type + (addr >> 8) + (addr & 0xFF);
		for (int i = 0; i < len; i ++){
			long v;
			s(9+i, 2).ToLong(&v, 16);
			checksum += v;
		}
		/* TODO: check CS */
	}

	return true;
}

/*********************** BoardPool Class Implementation **********************/

int BoardPool::LoadControllers()
{
	int i, nr_ctrls = sizeof(ctrls) / sizeof(Controller);
	for (i = 0; i < nr_ctrls; i ++)
		ctrls[i].LoadFile();
	return 0;
}

Controller *BoardPool::GetController(int mid, int pid)
{
	int i, nr_ctrls = sizeof(ctrls) / sizeof(Controller);
	for (i = 0; i < nr_ctrls; i ++){
		if (ctrls[i].IsA(mid, pid))
			return &ctrls[i];
	}
	return NULL;
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

wxArrayString BoardPool::GetBoardList(int mid, int pid)
{
	BoardList::iterator iter;
	wxArrayString blist;
	for (iter = boards.begin(); iter != boards.end(); ++iter){
		Boards *brd = iter;
		if (brd->GetController()->IsA(mid, pid))
			blist->Add(brd->GetName());
	}
	return blist;
}
