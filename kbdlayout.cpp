/* kbdlayout.cpp
 *
 * (C) Seo Bon Keun, 2015
 *
 * vim: set ts=8 sts=8 sw=8: syn on 
 */

#include "kbdlayout.h"

struct KeyItem {
	wxString label;
	float width;
	float height;
};

struct KeyItem sample_layout[5][16] = {
	{ { wxT("ESC"), 1.0, 1.0 }, { wxT("1"), 1.0, 1.0}, { wxT("2"), 1.0, 1.0},
	  { wxT("3"), 1.0, 1.0}, { wxT("4"), 1.0, 1.0}, { wxT("5"), 1.0, 1.0},
	  { wxT("6"), 1.0, 1.0}, { wxT("7"), 1.0, 1.0}, { wxT("8"), 1.0, 1.0},
	  { wxT("9"), 1.0, 1.0}, { wxT("0"), 1.0, 1.0}, { wxT("-"), 1.0, 1.0},
	  { wxT("="), 1.0, 1.0}, { wxT("`"), 1.0, 1.0}, { wxT("\\"), 1.0, 1.0}, 
	  { wxT(""), 0, 0} },
	{ { wxT("Tab"), 1.5, 1.0 }, { wxT("Q"), 1.0, 1.0}, { wxT("W"), 1.0, 1.0},
	  { wxT("E"), 1.0, 1.0}, { wxT("R"), 1.0, 1.0}, { wxT("T"), 1.0, 1.0},
	  { wxT("Y"), 1.0, 1.0}, { wxT("U"), 1.0, 1.0}, { wxT("I"), 1.0, 1.0},
	  { wxT("O"), 1.0, 1.0}, { wxT("P"), 1.0, 1.0}, { wxT("["), 1.0, 1.0},
	  { wxT("]"), 1.0, 1.0}, { wxT("Backspace"), 1.5, 1.0 }, { wxT(""), 0, 0} },
	{ { wxT("CapsLock"), 1.75, 1.0 }, { wxT("A"), 1.0, 1.0}, { wxT("S"), 1.0, 1.0},
	  { wxT("D"), 1.0, 1.0}, { wxT("F"), 1.0, 1.0}, { wxT("G"), 1.0, 1.0},
	  { wxT("H"), 1.0, 1.0}, { wxT("J"), 1.0, 1.0}, { wxT("K"), 1.0, 1.0},
	  { wxT("L"), 1.0, 1.0}, { wxT(";"), 1.0, 1.0}, { wxT("'"), 1.0, 1.0},
	  { wxT("Enter"), 2.25, 1.0 }, { wxT(""), 0, 0} },
	{ { wxT("LShift"), 2.25, 1.0 }, { wxT("Z"), 1.0, 1.0}, { wxT("X"), 1.0, 1.0},
	  { wxT("C"), 1.0, 1.0}, { wxT("V"), 1.0, 1.0}, { wxT("B"), 1.0, 1.0},
	  { wxT("N"), 1.0, 1.0}, { wxT("M"), 1.0, 1.0}, { wxT("<"), 1.0, 1.0},
	  { wxT(">"), 1.0, 1.0}, { wxT("/"), 1.0, 1.0}, { wxT("RShift"), 1.75, 1.0},
	  { wxT("Fn"), 1.0, 1.0}, { wxT(""), 0, 0} },
	{ { wxT("LCtrl"), 1.25, 1.0 }, { wxT("LGUI"), 1.25, 1.0}, { wxT("LAlt"), 1.25, 1.0},
	  { wxT("Spacebar"), 6.25, 1.0}, { wxT("RAlt"), 1.25, 1.0}, { wxT("RGUI"), 1.25, 1.0},
	  { wxT("Apps"), 1.25, 1.0}, { wxT("RCtrl"), 1.25, 1.0}, { wxT(""), 0, 0} }
};

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
			struct KeyItem key = sample_layout[i][j];
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
			struct KeyItem key = sample_layout[i][j];
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
