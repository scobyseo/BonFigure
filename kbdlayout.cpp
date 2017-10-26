/* kbdlayout.cpp
 *
 * (C) Seo Bon Keun, 2015
 *
 * vim: set ts=8 sts=8 sw=8: syn on 
 */

#include "kbdlayout.h"

KBDLayoutCanvas::KBDLayoutCanvas( wxPanel* parent )
	: wxPanel(parent)
{
	this->parent = parent;
	Connect(wxEVT_LEFT_DOWN, wxMouseEventHandler(KBDLayoutCanvas::OnMouseDown));
	Connect(wxEVT_LEFT_UP,   wxMouseEventHandler(KBDLayoutCanvas::OnMouseUp));
	Connect(wxEVT_PAINT,     wxPaintEventHandler(KBDLayoutCanvas::OnPaint));
	Connect(wxEVT_SIZE,      wxSizeEventHandler(KBDLayoutCanvas::OnSize));
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
	int i, j;
	float r, c;
	wxPaintDC bpdc(this);
	//PrepareDC(bpdc);
	r = c = 0.0;
	for (i = 0; i < 5; i ++){
		for (j = 0; j < 16; j ++){
			struct Key key = sample_layout[i][j];
			if (key.width == 0.0){
				c = 0.0;
				r += 1.0;
				break;
			}
			DrawKey(bpdc, key.label, c, r, 
					 key.width, key.height);
			c += key.width;
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

	int i, j;
	float r = 0.0, c = 0.0;
	for (i = 0; i < 5; i ++){
		for (j = 0; j < 16; j ++){
			struct Key key = sample_layout[i][j];
			if (key.width == 0.0){
				c = 0.0;
				r += 1.0;
				break;
			}
			if (p.x >= c*scale && p.y >= r*scale){
				act_col = c;
				act_row = r;
			}
			c += key.width;
		}
	}
	Refresh(false);
}

void KBDLayoutCanvas::OnMouseUp(wxMouseEvent &event)
{
}
