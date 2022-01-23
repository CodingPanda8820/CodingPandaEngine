#include "Engine.h"
#include "Device.h"
#include "PathManager.h"
#include "Timer.h"
#include "Resource/ResourceManager.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine():
	m_ClearColor{},
	m_Timer(nullptr)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(280);

	//m_ClearColor[1] = 1.f;
}

CEngine::~CEngine()
{
	CSceneManager::DestroyInst();

	CRenderManager::DestroyInst();

	CPathManager::DestroyInst();

	CResourceManager::DestroyInst();

	CDevice::DestroyInst();

	SAFE_DELETE(m_Timer);
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR * Name, unsigned int Width,
				   unsigned int Height, int IconID, bool WindowMode)
{
	m_hInst = hInst;

	m_RS.Width = Width;
	m_RS.Height = Height;

	Register(Name, IconID);
	Create(Name);

	return Init(hInst, m_hWnd, Width, Height, WindowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned int Width, unsigned int Height, bool WindowMode)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

	m_RS.Width = Width;
	m_RS.Height = Height;

	m_Timer = new CTimer;

	// Device �ʱ�ȭ
	if (!CDevice::GetInst()->Init(m_hWnd, Width, Height, WindowMode))
		return false;

	// ��� ������ �ʱ�ȭ
	if (!CPathManager::GetInst()->Init())
		return false;

	// ���ҽ� ������ �ʱ�ȭ
	if (!CResourceManager::GetInst()->Init())
		return false;

	// ������ ������ �ʱ�ȭ
	if (!CRenderManager::GetInst()->Init())
		return false;

	// ��� ������ �ʱ�ȭ
	if (!CSceneManager::GetInst()->Init())
		return false;

	return true;
}

int CEngine::Run()
{
	MSG msg;

	while (m_Loop)
	{
		// PeekMessage : �޼����� �̿� �����ϸ� nonzero�� �Ұ����ϸ� zero�� �����Ѵ�.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// TranslateMessage : �ؼ��� �޼����� �ִٸ� nonzero�� ����
			TranslateMessage(&msg);

			// DispatchMessage : ������ ���ν����� ������ ���� ����Ѵ�.
			DispatchMessage(&msg);
		}
		else
		{
			// �������� ����Ÿ�� : �޼����� �ö����� ����ϰ� �ִ� �ð�.
			// GetMessage�� ��� �޼����� ������ �ٸ� ���� �� �� ���� ������
			// PeekMessage�� �̿��� �޼����� ���� ��� �ش� �������� ���´�.
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	m_Timer->Update();

	float DeltaTime = m_Timer->GetDeltaTime();

	if (Update(DeltaTime))
		return;

	if (PostUpdate(DeltaTime))
		return;

	Render(DeltaTime);
}

bool CEngine::Update(float DeltaTime)
{
	if (CSceneManager::GetInst()->Update(DeltaTime))
		return true;

	return false;
}

bool CEngine::PostUpdate(float DeltaTime)
{
	if (CSceneManager::GetInst()->PostUpdate(DeltaTime))
		return true;

	return false;
}

bool CEngine::Render(float DeltaTime)
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(m_ClearColor);
	CDevice::GetInst()->ClearDepthStencil(1.f, 0);

	CRenderManager::GetInst()->Render();

	CDevice::GetInst()->Flip();

	return true;
}

ATOM CEngine::Register(const TCHAR * Name, int IconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEXW);

	// �۾������� �ٲ� �ٽ� �׸��Բ� ���ش�. ���ο� ���� ��� �ٽ� �׸���.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// �޼����� ť�� �������� �ش� �޼����� ��������
	// ������ �޼����� ���ڷ� �����ؼ� ȣ������ �Լ��� �����Ѵ�.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance�� �����Ѵ�.
	wcex.hInstance = m_hInst;

	// ���������� ��������� ����� ������ �̹����� �����Ѵ�.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IconID));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;

	// TEXT ��ũ�� : ""�տ� L�� �ٿ��� �����ڵ� ���ڿ��϶� L""�� multibyte �϶� "" �� ����
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

	// Ŭ���̾�Ʈ ������ 1280 x 720 ���� ������ش�.
	// RECT : �簢�� ����ü
	RECT rc = { 0, 0, (LONG)m_RS.Width, (LONG)m_RS.Height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
	// LRPRECT lpRect : ������ �䱸�ϴ� Ŭ���̾�Ʈ ������ �����Եȴ�.

	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0,
				 rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);

	// SW_SHOW : ������ â�� ȭ�鿡 �����ش�.
	// SW_HIDE : â�� ������ ������ ȭ�鿡 �����ִ°� �ƴϴ�.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow �Լ��� ������ �κ��� �ִٸ� �����϶�� ����� �����ִ� �Լ��̴�.
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
		// TextOut(hdc, 0, 0, TEXT("Hello, Windows!"), 15);
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
