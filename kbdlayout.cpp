/* kbdlayout.cpp
 *
 * (C) Seo Bon Keun, 2015
 *
 * vim: set ts=8 sts=8 sw=8: syn on 
 */

#include "kbdlayout.h"

KBDLayoutCanvas::KBDLayoutCanvas( wxPanel* parent, KbdLayout *layout )
	: wxPanel(parent)
{
	this->parent = parent;
	this->layout = layout;
	Connect(wxEVT_LEFT_DOWN,
			wxMouseEventHandler(KBDLayoutCanvas::OnMouseDown));
	Connect(wxEVT_LEFT_UP,
			wxMouseEventHandler(KBDLayoutCanvas::OnMouseUp));
	Connect(wxEVT_PAINT,
			wxPaintEventHandler(KBDLayoutCanvas::OnPaint));
	Connect(wxEVT_SIZE,
			wxSizeEventHandler(KBDLayoutCanvas::OnSize));
}

void KBDLayoutCanvas::DrawKey(wxDC &dc, wxString label,
		float x, float y, float w, float h)
{
	if (x == act_col && y == act_row){
		dc.SetBrush(wxBrush(*wxYELLOW_BRUSH));
		dc.SetPen(*wxMEDIUM_GREY_PEN);

	} else {
		dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
		dc.SetPen(*wxWHITE_PEN);
	}
	dc.DrawRoundedRectangle(x * scale + 1, y * scale + 1,
				w * scale - 2, h * scale - 2, scale / 10);
	dc.SetBrush(wxBrush(*wxBLUE));
	wxCoord length, height, descent;
#if 1
	wxFont font;
	font.SetFaceName("Roboto");
	font.SetPointSize(9);
	dc.SetFont(font);
#endif
	dc.GetTextExtent(label, &length, &height, &descent );
	dc.DrawText(label, x * scale + w * scale / 2 - length / 2,
			   y * scale + h * scale / 2 - height / 2);
}

void KBDLayoutCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	wxPaintDC bpdc(this);
	//PrepareDC(bpdc);
	float r = 0.0, c = 0.0;
	for (KbdLayout::iterator iter = layout.begin(); 
			iter != layout.end(); ++iter){
		struct Key *key = *iter;
		if (key->type == KEY_NORMAL){
			DrawKey(bpdc, key->label, c, r, 
				key->width, key->height);
			c += key->width;
		} else if (key->type == KEY_BLANK){
			c += key->width;
		} else if (key->type == KEY_NEWLINE){
			c = 0.0;
			r += 1.0;
		}
	}
}

void KBDLayoutCanvas::OnSize(wxSizeEvent &WXUNUSED(event))
{
	Refresh();
}

void KBDLayoutCanvas::OnMouseDown(wxMouseEvent &event)
{
	wxClientDC dc(this);
	PrepareDC(dc);
	wxPoint p = event.GetLogicalPosition(dc);

	float r = 0.0, c = 0.0;
	for (KbdLayout::iterator iter = layout.begin(); 
			iter != layout.end(); ++iter){
		struct Key *key = *iter;
		if (key->type == KEY_NEWLINE){
			c = 0.0;
			r += 1.0;
			continue;
		} else if (p.x >= c*scale && p.y >= r*scale){
			act_col = c;
			act_row = r;
			c += key->width;
		} else {
			c += key->width;
		}
	}
	Refresh(false);
}

void KBDLayoutCanvas::OnMouseUp(wxMouseEvent &event)
{
}
