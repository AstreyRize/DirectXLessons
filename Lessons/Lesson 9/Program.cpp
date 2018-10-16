// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-5
// ��������� ������������ �����
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// ��������� ���������� DirectX
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// ������������� ���������� ������
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// ���������� ����������
IDXGISwapChain *swapchain;				// ��������� �� swap chain interface
ID3D11Device *dev;						// ��������� �� Direct3D device interface
ID3D11DeviceContext *devcon;			// ��������� �� Direct3D device context
ID3D11RenderTargetView *backbuffer;		// ��������� �� ������ �����
ID3D11InputLayout *pLayout;				// ��������� �� ������� ����
ID3D11VertexShader *pVS;				// ��������� �� ������ ������
ID3D11PixelShader *pPS;					// ��������� �� ������ ��������
ID3D11Buffer *pVBuffer;					// ��������� �� ����� ������

// ��������� �������
struct VERTEX
{
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};

// �������� �������
void InitD3D(HWND hWnd);				// ��������� � ������������� DirectX
void RenderFrame(void);					// ������ �����
void CleanD3D(void);					// ����������� ������ DirectX
void InitGraphics(void);				// ������� ����� ��� �������
void InitPipeline(void);				// ��������� � �������������� �������

// ���������� �������
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// ����� ����� � ���������
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow)
{
	HWND hWnd;
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"WindowClass",
		"Our First Direct3D Program",
		WS_OVERLAPPEDWINDOW,
		300,
		300,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	// ��������� � ������������� Direct3D
	InitD3D(hWnd);

	MSG msg;

	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame();
	}

	CleanD3D();

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	} break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}


// �������������� � �������������� DirectX.
void InitD3D(HWND hWnd)
{
	// ������� ��������� ��� �������� swap chain.
	DXGI_SWAP_CHAIN_DESC scd;

	// �������� ������ ��� ���������.
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;									// ���� ������ �����.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 32-bit ����.
	scd.BufferDesc.Width = SCREEN_WIDTH;					// ������������� ������ ������� ������.
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// ������������� ������ ������� ������.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// ���������� ��� ����� �������������� swap chain.
	scd.OutputWindow = hWnd;								// ���������� ����, ������� ����� ��������������.
	scd.SampleDesc.Count = 4;								// ������ ���������� multisample-��.
	scd.Windowed = TRUE;									// ������� �����.
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// ��������� ������������� �����.

	// ������� ����������, �������� ���������� � swap chain, ��������� ���������� �� scd.
	D3D11CreateDeviceAndSwapChain(NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scd,
		&swapchain,
		&dev,
		NULL,
		&devcon);

	// �������� ����� ������� ������.
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// ��������� ����� ������� ������ ��� �������� ���������� ��� �������.
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// ������ ��������� ��� ������� ��� ������ �����.
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// ������������� �������������.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;

	devcon->RSSetViewports(1, &viewport);

	InitPipeline();
	InitGraphics();
}

// ������ ������ �����.
void RenderFrame(void)
{
	// ������� ������ ����� � ������������� ����� ����.
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// �������� ����� ������ ��� �����������.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// �������� ��� ���������, ������� ����� ������������.
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// ������ ������� �� ������ ������ � ������ �����.
	devcon->Draw(3, 0);

	// ����������� ������ ����� � �������� �����.
	swapchain->Present(0, 0);
}

// ��� �������, ������� ������� DirectX � COM �������.
void CleanD3D(void)
{
	// ����������� ����� ���� �� �������.
	swapchain->SetFullscreenState(FALSE, NULL);

	// ��������� � ����������� �������.
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

// � ���� ������� ������� ������ ��� �������.
void InitGraphics()
{
	// ������� ����������� ��������� ��������� VERTEX.
	VERTEX OurVertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	// ������� ����� ������.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;										// ������������� ������� ������� ��� CPU � GPU.
	bd.ByteWidth = sizeof(VERTEX) * 3;									// ������ ��� ���� �������� VERTEX.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// ���������� ��� ����� ������.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// ��������� CPU ������ � �����.

	dev->CreateBuffer(&bd, NULL, &pVBuffer);							// ������� �����

	// �������� ������� � �����.
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// ����� �����.
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));					// �������� ������.
	devcon->Unmap(pVBuffer, NULL);										// �������� �����.
}

// ��� ������� ��������� � �������������� �������.
void InitPipeline()
{
	// ��������� � ����������� �������.
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// ������������� ��� ������� � ������.
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// ������ ������� ��������.
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// ������� ������ �������� ����.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}