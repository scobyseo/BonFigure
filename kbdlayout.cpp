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
		int x, int y, int w, int h)
{
	if (x == act_col && y == act_row){
		printf("Active key: %d %d\n", x, y);
		dc.SetBrush(wxBrush(*wxYELLOW_BRUSH));
		dc.SetPen(*wxMEDIUM_GREY_PEN);

	} else {
		dc.SetBrush(wxBrush(*wxWHITE_BRUSH));
		dc.SetPen(*wxMEDIUM_GREY_PEN);
	}
	dc.DrawRoundedRectangle(x * scale + 1, y * scale + 1,
			w * scale - 2, h * scale - 2, scale / 10);
	dc.SetBrush(wxBrush(*wxBLUE));
	wxCoord length, height, descent;
	dc.GetTextExtent(label, &length, &height, &descent );
	dc.DrawText(label, x * scale + scale / 2 - length / 2,
						  y * scale + scale / 2 - height / 2);
}

void KBDLayoutCanvas::OnPaint(wxPaintEvent &WXUNUSED(event))
{
	int i, j;
	wxPaintDC bpdc(this);
	PrepareDC(bpdc);
	for (i = 0; i < 4; i ++){
		for (j = 0; j < 15; j ++){
			DrawKey(bpdc, wxString("Ctrl"), j, i, 1, 1);
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

	printf("Mouse Down : %d %d\n", p.x, p.y);
	if (p.x < 15 * scale && p.y < 4 * scale){
		act_col = p.x / scale;
		act_row = p.y / scale;
	}
	Refresh();
}

void KBDLayoutCanvas::OnMouseUp(wxMouseEvent &event)
{
}
