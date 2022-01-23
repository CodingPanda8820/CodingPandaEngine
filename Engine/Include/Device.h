#pragma once

#include "GameInfo.h"

/*
Direct3D �� �ʱ�ȭ

1.	D3D11CreateDevice �Լ��� �̿��ؼ� ��ġ, �� ID3D11Device �������̽��� ��ġ ����
	�� ID3D11DeviceContext �������̽��� �����Ѵ�.
2.	ID3D11Device::CheckMultiSampleQualityLevels �޼��带 �̿��ؼ�
	4X MSAA[MultiSample Anti-Aliasing] ǰ�� ���� ���� ���θ� �˻��Ѵ�.
3.	������ ��ȯ �罽�� Ư���� �����ϴ� DXGI_SWAP_CHAIN_DESC ����ü�� ä���.
4.	��ġ�� �����ϴ� �� ����ߴ� IDXGIFactory �������̽��� �����ؼ�
	IDXGISwapChain �������̽��� �����Ѵ�.
5.	��ȯ �罽�� �ĸ� ���ۿ� ���� ���� ��� �並 �����Ѵ�.
6.	����.���ٽ� ���ۿ� �׿� ����Ǵ� ����.���ٽ� �並 �����Ѵ�.
7.	���� ��� ��� ����.���ٽ� �並 Direct3D�� ����� �� �ֵ��� ������ ������������
	��� ���ձ� �ܰ迡 ���´�.
8.	����Ʈ�� �����Ѵ�.
*/

class CDevice
{
private:
	ID3D11Device* m_Device; // ��� ���� ���˰� �ڿ��Ҵ翡 ���δ�.

	// ���� ����� ����, �ڿ��� �׷��� ���������ο� ���´�.
	// GPU�� ������ ������ ��ɵ��� �����ϴ� �� ���δ�.
	ID3D11DeviceContext* m_Context; 

	IDXGISwapChain* m_SwapChain;

	// BackBuffer�� �ȼ��� ���� ���� RenderTargetView�� �����
	// BackBuffer�� �� TargetView�� �������ش�.
	ID3D11RenderTargetView* m_TargetView;

	// Depth&Stencil ���� ���� ���� DepthStencilView�� ����� �������ش�.
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

