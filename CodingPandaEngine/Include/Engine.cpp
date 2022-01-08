#include "Engine.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine()
{
}

CEngine::~CEngine()
{
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR * Name, unsigned int Width, unsigned int Height, int IconID, bool WindowMode)
{
	m_hInst = hInst;

	m_RS.Width = Width;
	m_RS.Height = Height;

	Register(Name, IconID);
	Create(Name);

	return true;
}

int CEngine::Run()
{
	MSG msg;

	while (m_Loop)
	{
		// PeekMessage : 메세지가 이용 가능하면 nonzero를 불가능하면 zero를 리턴한다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// TranslateMessage : 해석된 메세지가 있다면 nonzero를 리턴
			TranslateMessage(&msg);

			// DispatchMessage : 윈도우 프로시져가 리턴한 값을 명시한다.
			DispatchMessage(&msg);
		}
		else
		{
			// 윈도우의 데드타임 : 메세지가 올때까지 대기하고 있는 시간.
			// GetMessage의 경우 메세지가 없을때 다른 일을 할 수 없기 때문에
			// PeekMessage를 이용해 메세지가 없을 경우 해당 구간으로 들어온다.
		}
	}

	return (int)msg.wParam;
}

ATOM CEngine::Register(const TCHAR * Name, int IconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);

	// 작업영역이 바뀔때 다시 그리게끔 해준다. 가로와 세로 모두 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고
	// 꺼내온 메세지를 인자로 전달해서 호출해줄 함수를 지정한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance를 지정한다.
	wcex.hInstance = m_hInst;

	// 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;

	// TEXT 매크로 : ""앞에 L을 붙여서 유니코드 문자열일땐 L""로 multibyte 일땐 "" 로 설정
	wcex.lpszClassName = Name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IconID));
	
	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR * Name)
{
	m_hWnd = CreateWindowW(Name, Name, WS_OVERLAPPEDWINDOW,
						   0, 0, m_RS.Width, m_RS.Height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
		return FALSE;

	// 클라이언트 영역을 1280 x 720 으로 만들어준다.
	// RECT : 사각형 구조체
	RECT rc = { 0, 0, (LONG)m_RS.Width, (LONG)m_RS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// LRPRECT lpRect : 실제로 요구하는 클라이언트 영역이 들어오게된다.

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,
				 rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);

	// SW_SHOW : 윈도우 창을 화면에 보여준다.
	// SW_HIDE : 창을 만들기는 하지만 화면에 보여주는건 아니다.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		HDC hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 0, 0, TEXT("Hello, Windows!"), 15);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
