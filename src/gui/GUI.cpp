/*
 * GUI.cpp
 *
 *  Created on: Jan 25, 2017
 *      Author: nullifiedcat
 */

#include "GUI.h"
#include "IWidget.h"
#include "RootWindow.h"
#include "CTooltip.h"

#include "../common.h"
#include "../sdk.h"

void GUIVisibleCallback(IConVar* var, const char* pOldValue, float flOldValue) {
	g_IInputSystem->SetCursorPosition(draw::width / 2, draw::height / 2);
	g_ISurface->SetCursor(vgui::CursorCode::dc_none);
	g_IMatSystemSurface->SetCursorAlwaysVisible(false);
	if (gui_visible) {
		g_IMatSystemSurface->UnlockCursor();
	} else {
		g_IMatSystemSurface->LockCursor();
	}
}

extern CatVar gui_visible(CV_SWITCH, "gui_visible", "0", "GUI Active", "GUI switch (bind it to a key!)");
extern CatVar gui_draw_bounds(CV_SWITCH, "gui_bounds", "0", "Draw Bounds", "Draw GUI elements' bounding boxes");

CatGUI::CatGUI() {
	m_pRootWindow = 0;
}

CatGUI::~CatGUI() {
	delete m_pRootWindow;
}

void CatGUI::Setup() {
	m_pRootWindow = new RootWindow();
	m_pRootWindow->Setup();
	gui_visible.OnRegister([](CatVar* var) {
		var->convar->InstallChangeCallback(GUIVisibleCallback);
	});
}

void CatGUI::ShowTooltip(std::string text) {
	m_pTooltip->SetText(text);
	m_pTooltip->SetOffset(m_iMouseX + 5, m_iMouseY + 5);
	m_pTooltip->Show();
	m_bShowTooltip = true;
}

void CatGUI::Update() {
	m_bShowTooltip = false;
	for (int i = 0; i < ButtonCode_t::BUTTON_CODE_COUNT; i++) {
		bool down = g_IInputSystem->IsButtonDown((ButtonCode_t)(i));
		bool changed = m_bPressedState[i] != down;
		if (changed && down) m_iPressedFrame[i] = g_GlobalVars->framecount;
		m_bPressedState[i] = down;
		if (m_bKeysInit) {
			if (changed) {
				//logging::Info("Key %i changed! Now %i.", i, down);
				if (i == ButtonCode_t::MOUSE_LEFT) {
					if (down) m_pRootWindow->OnMousePress();
					else m_pRootWindow->OnMouseRelease();
				} else {
					if (i == ButtonCode_t::KEY_INSERT && down) {
						gui_visible = !gui_visible;
					}
					if (down) m_pRootWindow->OnKeyPress((ButtonCode_t)i, false);
					else m_pRootWindow->OnKeyRelease((ButtonCode_t)i);
				}
			} else {
				if (down) {
					int frame = g_GlobalVars->framecount - m_iPressedFrame[i];
					bool shouldrepeat = false;
					if (frame) {
						if (frame > 150) {
							if (frame > 400) {
								if (frame % 30 == 0) shouldrepeat = true;
							} else {
								if (frame % 80 == 0) shouldrepeat = true;
							}
						}
					}
					if (shouldrepeat) m_pRootWindow->OnKeyPress((ButtonCode_t)i, true);
				}
			}
		}
	}


	m_iMouseX = g_IInputSystem->GetAnalogValue(AnalogCode_t::MOUSE_X);
	m_iMouseY = g_IInputSystem->GetAnalogValue(AnalogCode_t::MOUSE_Y);

	if (!m_bKeysInit) m_bKeysInit = 1;
	if (gui_visible) {
		if (!m_pRootWindow->IsVisible())
			m_pRootWindow->Show();
		m_pRootWindow->Update();
		if (!m_bShowTooltip && m_pTooltip->IsVisible()) m_pTooltip->Hide();
		m_pRootWindow->Draw(0, 0);
		draw::DrawRect(m_iMouseX - 5, m_iMouseY - 5, 10, 10, colors::Transparent(colors::white));
		draw::OutlineRect(m_iMouseX - 5, m_iMouseY - 5, 10, 10, colors::pink);
		if (gui_draw_bounds) {
			m_pRootWindow->DrawBounds(0, 0);
		}
	} else {
		if (m_pRootWindow->IsVisible())
			m_pRootWindow->Hide();
	}


}

bool CatGUI::ConsumesKey(ButtonCode_t key) {
	if (m_pRootWindow->IsVisible())
		return m_pRootWindow->ConsumesKey(key);
	else return false;
}

RootWindow* CatGUI::GetRootWindow() {
	return m_pRootWindow;
}

CatGUI* g_pGUI = 0;
