#pragma once

#include "GameInfo.h"

class CEngine
{
private:
	HINSTANCE	m_hInst;
	HWND		m_hWnd;
	Resolution	m_RS;

	static bool	m_Loop;

public:
	bool Init(HINSTANCE hInst, const TCHAR* Name, unsigned int Width,
			  unsigned int Height, int IconID, bool WindowMode = true);
	int Run();

private:
	ATOM Register(const TCHAR* Name, int IconID);
	BOOL Create(const TCHAR* Name);
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_SINGLE(CEngine)
};

