#include "wx/wxprec.h"

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/dcbuffer.h"
#include "wx/graphics.h"

class KBDLayoutCanvas: public wxPanel
{
private:
	int scale = 40;
	int act_col = -1, act_row = -1;
	wxPanel *parent;

public:
	KBDLayoutCanvas( wxPanel *parent );

	void OnPaint(wxPaintEvent &event);
	void OnSize (wxSizeEvent  &event);
	void OnMouseDown(wxMouseEvent &event);
	void OnMouseUp  (wxMouseEvent &event);

	void DrawKey(wxDC &dc, wxString label, int x, int y, int w, int h);
	void SetScale(int s) { scale = s; }
};

