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

	// 다중 표본화를 위해 추출할 표본의 개수와 품질 수준을 서술하는 구조체
	DXGI_SWAP_CHAIN_DESC SwapDesc = {};

	// BufferDesc : 후면버퍼의 속성들을 서술하는 개별적인 구조체이다.
	SwapDesc.BufferDesc.Width = Width; // 후면 버퍼의 너비
	SwapDesc.BufferDesc.Height = Height; // 후면 버퍼의 높이
	// 후면 버퍼 픽셀 형식
	// DXGI_FORMAT_R8G8B8A8_UNORM 을 사용하는 이유는 일반적으로 
	// 모니터 색상당 비트 수가 24비트 이하이므로 정밀도를 더 높게 잡아도 낭비이다.
	SwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	// 디스플레이 모드의 갱신율
	SwapDesc.BufferDesc.RefreshRate.Numerator = 1; 
	SwapDesc.BufferDesc.RefreshRate.Denominator = 60;
	// 디스플레이 비례 모드 / 스캔라인 모드
	SwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	SwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	SwapDesc.BufferCount = 1;
	// 버퍼의 용도를 서술하는 구조체
	SwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	// 렌더링 결과를 표시할 창의 핸들
	SwapDesc.OutputWindow = hWnd;
	// Anti-Aliasing 다중표본화를 위해 추출할 표본 개수와 품질 수준을 서술하는 구조체
	SwapDesc.SampleDesc.Count = 1; // 다중 표본화(MultiSampling)의 표본 개수
	SwapDesc.SampleDesc.Quality = 0; // 다중 표본화의 품질 수준
	// 창모드를 원하면 true 아니라면 false
	SwapDesc.Windowed = WindowMode;
	// 교환 효과를 서술하는 구조체
	SwapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Device가 가장 효율적인 방법을 선택

	// 기능 수준(Feature Level)
	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;

	// HRESULT D3D11CreateDevice(); 참조
	if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE,
											 0, Flag, &FLevel, 1, D3D11_SDK_VERSION,
											 &SwapDesc, &m_SwapChain, &m_Device,
											 &FLevel1, &m_Context)))
		return false;

	// SwapChain이 가지고 있는 백버퍼를 얻어온다.
	// Back Buffer
	// 후면 버퍼를 하나의 완전한 프레임으로서 화면에 표시하면 사용자는 완성된 프레임만을
	// 볼 수 있고, 그려지는 과정은 못보게 된다. (깜빡거림 해결법)
	// 전면 버퍼가 화면에 표시되어 있는동안 애니메이션의 다음 프레임을 후면 버퍼에 그린다.
	// 그런 다음에는 전면 버퍼와 후면 버퍼의 역할을 맞바꾼다.
	// 전면 버퍼와 후면 버퍼를 교환해서 화면에 표시되게 하는것을 '제시(presenting)' 라고 부른다.
	// 실제로 버퍼들의 내용을 맞바꾸는 것이아니라 포인터를 맞바꾸는 것이기 때문에 효율적이다.
	// 전면 버퍼와 후면 버퍼가 하나의 교환사슬을 형성한다. IDXGISwapChain
	ID3D11Texture2D* BackBuffer = nullptr;

	// SwapChain을 가리키는 포인터를 얻는다.
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// BackBuffer 와 연결된 RenderTargetView 를 생성한다.
	m_Device->CreateRenderTargetView(BackBuffer, nullptr, &m_TargetView);

	SAFE_RELEASE(BackBuffer);

	// 깊이 버퍼를 만든다.
	D3D11_TEXTURE2D_DESC DepthDesc = {};

	DepthDesc.Width = Width;
	DepthDesc.Height = Height;
	DepthDesc.MipLevels = 1; // 스텐실 버퍼를 위한 텍스처에서는 밉맵 수준이 하나만 필요하다.
	DepthDesc.ArraySize = 1; // 스텐실 버퍼의 경우에는 텍스처 하나만 필요하다.
	DepthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	DepthDesc.SampleDesc.Count = 1;
	DepthDesc.SampleDesc.Quality = 0;
	DepthDesc.Usage = D3D11_USAGE_DEFAULT; // 텍스처의 용도를 뜻하는 필드
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
	// 응용프로그램은 어떤 렌더링이던지 수행하기전에 먼저 후면 버퍼를 기본 색상으로 지운다.
	// 이때 깊이 버퍼도 기본값으로 지워진다.

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
