#pragma once

#include "GameInfo.h"

/*
Direct3D 의 초기화

1.	D3D11CreateDevice 함수를 이용해서 장치, 즉 ID3D11Device 인터페이스와 장치 문맥
	즉 ID3D11DeviceContext 인터페이스를 생성한다.
2.	ID3D11Device::CheckMultiSampleQualityLevels 메서드를 이용해서
	4X MSAA[MultiSample Anti-Aliasing] 품질 수준 지원 여부를 검사한다.
3.	생성할 교환 사슬의 특성을 서술하는 DXGI_SWAP_CHAIN_DESC 구조체를 채운다.
4.	장치를 생성하는 데 사용했던 IDXGIFactory 인터페이스를 질의해서
	IDXGISwapChain 인터페이스를 생성한다.
5.	교환 사슬의 후면 버퍼에 대한 렌더 대상 뷰를 생성한다.
6.	깊이.스텐실 버퍼와 그에 연결되는 깊이.스텐실 뷰를 생성한다.
7.	렌더 대상 뷰와 깊이.스텐실 뷰를 Direct3D가 사용할 수 있도록 렌더링 파이프라인의
	출력 병합기 단계에 묶는다.
8.	뷰포트를 설정한다.
*/

class CDevice
{
private:
	ID3D11Device* m_Device; // 기능 지원 점검과 자원할당에 쓰인다.

	// 렌더 대상을 설정, 자원을 그래픽 파이프라인에 묶는다.
	// GPU가 수행할 렌더링 명령들을 지시하는 데 쓰인다.
	ID3D11DeviceContext* m_Context; 

	IDXGISwapChain* m_SwapChain;

	// BackBuffer에 픽셀을 쓰기 위해 RenderTargetView를 만들고
	// BackBuffer를 이 TargetView에 연결해준다.
	ID3D11RenderTargetView* m_TargetView;

	// Depth&Stencil 값을 쓰기 위해 DepthStencilView를 만들고 연결해준다.
	ID3D11DepthStencilView* m_DepthView;

	HWND m_hWnd;
	Resolution m_RS;

public:
	ID3D11Device* GetDevice()	const
	{
		return m_Device;
	}

	ID3D11DeviceContext* GetContext()	const
	{
		return m_Context;
	}

	IDXGISwapChain* GetSwapChain()	const
	{
		return m_SwapChain;
	}

	Resolution GetResolution()	const
	{
		return m_RS;
	}

public:
	bool Init(HWND hWnd, unsigned int Width, unsigned int Height,
			  bool WindowMode);
	void ClearRenderTarget(float ClearColor[4]);
	void ClearDepthStencil(float Depth, unsigned char Stencil);
	void RenderStart();
	void Flip();

	DECLARE_SINGLE(CDevice)
};

