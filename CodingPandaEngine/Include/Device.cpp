#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice():
	m_Device(nullptr),
	m_Context(nullptr),
	m_SwapChain(nullptr),
	m_TargetView(nullptr),
	m_DepthView(nullptr)
{
}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthView);

	if (m_Context)
		m_Context->ClearState();

	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);
}

bool CDevice::Init(HWND hWnd, unsigned int Width, unsigned int Height, bool WindowMode)
{
	m_hWnd = hWnd;
	m_RS.Width = Width;
	m_RS.Height = Height;

	unsigned int Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	Flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG

	// ���� ǥ��ȭ�� ���� ������ ǥ���� ������ ǰ�� ������ �����ϴ� ����ü
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};

	// BufferDesc : �ĸ������ �Ӽ����� �����ϴ� �������� ����ü�̴�.
	SwapDesc.BufferDesc.Width = Width; // �ĸ� ������ �ʺ�
	SwapDesc.BufferDesc.Height = Height; // �ĸ� ������ ����
	// �ĸ� ���� �ȼ� ����
	// DXGI_FORMAT_R8G8B8A8_UNORM �� ����ϴ� ������ �Ϲ������� 
	// ����� ����� ��Ʈ ���� 24��Ʈ �����̹Ƿ� ���е��� �� ���� ��Ƶ� �����̴�.
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// ���÷��� ����� ������
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1; 
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	// ���÷��� ��� ��� / ��ĵ���� ���
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.BufferCount = 1;
	// ������ �뵵�� �����ϴ� ����ü
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// ������ ����� ǥ���� â�� �ڵ�
	SwapDesc.OutputWindow = hWnd;
	// Anti-Aliasing ����ǥ��ȭ�� ���� ������ ǥ�� ������ ǰ�� ������ �����ϴ� ����ü
	SwapDesc.SampleDesc.Count = 1; // ���� ǥ��ȭ(MultiSampling)�� ǥ�� ����
	SwapDesc.SampleDesc.Quality = 0; // ���� ǥ��ȭ�� ǰ�� ����
	// â��带 ���ϸ� true �ƴ϶�� false
	SwapDesc.Windowed = WindowMode;
	// ��ȯ ȿ���� �����ϴ� ����ü
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Device�� ���� ȿ������ ����� ����

	// ��� ����(Feature Level)
	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;

	// HRESULT D3D11CreateDevice(); ����
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
											 0, Flag, &FLevel, 1, D3D11_SDK_VERSION,
											 &SwapDesc, &m_SwapChain, &m_Device,
											 &FLevel1, &m_Context)))
		return false;

	// SwapChain�� ������ �ִ� ����۸� ���´�.
	// Back Buffer
	// �ĸ� ���۸� �ϳ��� ������ ���������μ� ȭ�鿡 ǥ���ϸ� ����ڴ� �ϼ��� �����Ӹ���
	// �� �� �ְ�, �׷����� ������ ������ �ȴ�. (�����Ÿ� �ذ��)
	// ���� ���۰� ȭ�鿡 ǥ�õǾ� �ִµ��� �ִϸ��̼��� ���� �������� �ĸ� ���ۿ� �׸���.
	// �׷� �������� ���� ���ۿ� �ĸ� ������ ������ �¹ٲ۴�.
	// ���� ���ۿ� �ĸ� ���۸� ��ȯ�ؼ� ȭ�鿡 ǥ�õǰ� �ϴ°��� '����(presenting)' ��� �θ���.
	// ������ ���۵��� ������ �¹ٲٴ� ���̾ƴ϶� �����͸� �¹ٲٴ� ���̱� ������ ȿ�����̴�.
	// ���� ���ۿ� �ĸ� ���۰� �ϳ��� ��ȯ�罽�� �����Ѵ�. IDXGISwapChain
	ID3D11Texture2D* BackBuffer = nullptr;

	// SwapChain�� ����Ű�� �����͸� ��´�.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// BackBuffer �� ����� RenderTargetView �� �����Ѵ�.
	m_Device->CreateRenderTargetView(BackBuffer, nullptr, &m_TargetView);

	SAFE_RELEASE(BackBuffer);

	// ���� ���۸� �����.
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.MipLevels = 1; // ���ٽ� ���۸� ���� �ؽ�ó������ �Ӹ� ������ �ϳ��� �ʿ��ϴ�.
	DepthDesc.ArraySize = 1; // ���ٽ� ������ ��쿡�� �ؽ�ó �ϳ��� �ʿ��ϴ�.
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT; // �ؽ�ó�� �뵵�� ���ϴ� �ʵ�
	DepthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	

	ID3D11Texture2D* DepthBuffer = nullptr;

	m_Device->CreateTexture2D(&DepthDesc, nullptr, &DepthBuffer);

	m_Device->CreateDepthStencilView(DepthBuffer, nullptr, &m_DepthView);
	
	SAFE_RELEASE(DepthBuffer);

	D3D11_VIEWPORT VP = {};

	VP.Width = (float)Width;
	VP.Height = (float)Height;
	VP.MaxDepth = 1.f;

	m_Context->RSSetViewports(1, &VP);

	return true;
}

void CDevice::ClearRenderTarget(float ClearColor[4])
{
	m_Context->ClearRenderTargetView(m_TargetView, ClearColor);
}

void CDevice::ClearDepthStencil(float Depth, unsigned char Stencil)
{
	// �������α׷��� � �������̴��� �����ϱ����� ���� �ĸ� ���۸� �⺻ �������� �����.
	// �̶� ���� ���۵� �⺻������ ��������.

	m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
									 Depth, Stencil);
}

void CDevice::RenderStart()
{
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	m_SwapChain->Present(0, 0);
}
