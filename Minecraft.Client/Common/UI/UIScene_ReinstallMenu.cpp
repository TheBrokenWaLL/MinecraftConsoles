#include "stdafx.h"
#include "UI.h"
#include "UIScene_ReinstallMenu.h"

UIScene_ReinstallMenu::UIScene_ReinstallMenu(int iPad, void *initData, UILayer *parentLayer) : UIScene(iPad, parentLayer)
{
	// Setup all the Iggy references we need for this scene
	initialiseMovie();

	m_playerNick = ProfileManager.GetDisplayName(m_iPad);
	m_buttons[eControl_Theme].init(m_playerNick, eControl_Theme);

	removeControl( &m_buttons[eControl_Gamerpic1], false );
	removeControl( &m_buttons[eControl_Gamerpic2], false );
	removeControl( &m_buttons[eControl_Avatar1], false );
	removeControl( &m_buttons[eControl_Avatar2], false );
	removeControl( &m_buttons[eControl_Avatar3], false );

#if TO_BE_IMPLEMENTED
	XuiControlSetText(m_Buttons[eControl_Theme],app.GetString(IDS_REINSTALL_THEME));
	XuiControlSetText(m_Buttons[eControl_Gamerpic1],app.GetString(IDS_REINSTALL_GAMERPIC_1));
	XuiControlSetText(m_Buttons[eControl_Gamerpic2],app.GetString(IDS_REINSTALL_GAMERPIC_2));
	XuiControlSetText(m_Buttons[eControl_Avatar1],app.GetString(IDS_REINSTALL_AVATAR_ITEM_1));
	XuiControlSetText(m_Buttons[eControl_Avatar2],app.GetString(IDS_REINSTALL_AVATAR_ITEM_2));
	XuiControlSetText(m_Buttons[eControl_Avatar3],app.GetString(IDS_REINSTALL_AVATAR_ITEM_3));
#endif
}

wstring UIScene_ReinstallMenu::getMoviePath()
{
	if(app.GetLocalPlayerCount() > 1)
	{
		return L"ReinstallSplit";
	}
	else
	{
		return L"ReinstallMenu";
	}
}


void UIScene_ReinstallMenu::updatePlaceholderButtonLabel()
{
	m_buttons[eControl_Theme].setLabel(m_playerNick);
}

int UIScene_ReinstallMenu::KeyboardCompleteCallback(LPVOID lpParam, bool bRes)
{
	UIScene_ReinstallMenu *pClass = (UIScene_ReinstallMenu *)lpParam;
	if (bRes)
	{
		uint16_t pchText[128];
		ZeroMemory(pchText, 128 * sizeof(uint16_t));
		InputManager.GetText(pchText);

		pClass->m_playerNick = (wchar_t *)pchText;
		pClass->updatePlaceholderButtonLabel();
	}

	return 0;
}
void UIScene_ReinstallMenu::updateTooltips()
{
	ui.SetTooltips( m_iPad, IDS_TOOLTIPS_SELECT,IDS_TOOLTIPS_BACK);
}

void UIScene_ReinstallMenu::updateComponents()
{
	bool bNotInGame=(Minecraft::GetInstance()->level==NULL);
	if(bNotInGame)
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,true);
		m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,true);
	}
	else
	{
		m_parentLayer->showComponent(m_iPad,eUIComponent_Panorama,false);

		// 4J Stu - Do we want to show the logo in-game?
		//if( app.GetLocalPlayerCount() == 1 ) m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,true);
		//else m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,false);
		m_parentLayer->showComponent(m_iPad,eUIComponent_Logo,false);

	}
}

void UIScene_ReinstallMenu::handleInput(int iPad, int key, bool repeat, bool pressed, bool released, bool &handled)
{
	//app.DebugPrintf("UIScene_DebugOverlay handling input for pad %d, key %d, down- %s, pressed- %s, released- %s\n", iPad, key, down?"TRUE":"FALSE", pressed?"TRUE":"FALSE", released?"TRUE":"FALSE");

	ui.AnimateKeyPress(m_iPad, key, repeat, pressed, released);

	switch(key)
	{
	case ACTION_MENU_CANCEL:
		if(pressed && !repeat)
		{
			navigateBack();
		}
		break;
	case ACTION_MENU_OK:
#ifdef __ORBIS__
	case ACTION_MENU_TOUCHPAD_PRESS:
#endif
	case ACTION_MENU_UP:
	case ACTION_MENU_DOWN:
		sendInputToMovie(key, repeat, pressed, released);
		break;
	}
}


void UIScene_ReinstallMenu::handleEditNamePressed()
{
#if defined(__PS3__) || defined(__ORBIS__) || defined __PSVITA__
	int language = XGetLanguage();
	switch(language)
	{
	case XC_LANGUAGE_JAPANESE:
	case XC_LANGUAGE_KOREAN:
	case XC_LANGUAGE_TCHINESE:
		InputManager.RequestKeyboard(app.GetString(IDS_TITLE_RENAME), m_playerNick.c_str(), (DWORD)m_iPad, 25, &UIScene_ReinstallMenu::KeyboardCompleteCallback, this, C_4JInput::EKeyboardMode_Default);
		break;
	default:
		InputManager.RequestKeyboard(app.GetString(IDS_TITLE_RENAME), m_playerNick.c_str(), (DWORD)m_iPad, 25, &UIScene_ReinstallMenu::KeyboardCompleteCallback, this, C_4JInput::EKeyboardMode_Alphabet_Extended);
		break;
	}
#else
	InputManager.RequestKeyboard(app.GetString(IDS_TITLE_RENAME), m_playerNick.c_str(), (DWORD)m_iPad, 25, &UIScene_ReinstallMenu::KeyboardCompleteCallback, this, C_4JInput::EKeyboardMode_Default);
#endif
}

void UIScene_ReinstallMenu::handlePress(F64 controlId, F64 childId)
{
	switch((int)controlId)
	{
	case eControl_Theme:
		handleEditNamePressed();
		break;
	}
}
