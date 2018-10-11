// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-3
// ��������� � ������ ������������ ����� ��� windows � DirectX
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// ��������� ���������� DirectX
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// ���������� ����������
IDXGISwapChain *swapchain;										// ��������� �� swap chain interface
ID3D11Device *dev;												// ��������� �� Direct3D device interface
ID3D11DeviceContext *devcon;									// ��������� �� Direct3D device context
ID3D11RenderTargetView *backbuffer;								// ��������� �� ������ �����

// ��������� �������
void InitD3D(HWND hWnd);										// ����������� � �������������� DirectX
void RenderFrame(int &red, int &green, int &blue, int &step);	// �������� ���� �����
void CleanD3D(void);											// ��������� DirectX � ����������� ������

// �������� ������� WindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// ����� ����� � ����������
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
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass";

	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 800, 600 };
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

	// ����������� � �������������� DirectX
	InitD3D(hWnd);

	MSG msg;
	int red = 100000;
	int green = 0;
	int blue = 0;
	int step = 1;

	// ����������� ���� ����
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}

		RenderFrame(red, green, blue, step);
	}

	// ����������� �������
	CleanD3D();

	return msg.wParam;
}

// ���������� �������
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

// ��������� � ������������� DirectX
void InitD3D(HWND hWnd)
{
	// �������� ��������� ��� �������� ���������� � swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// �������� ������ ��� ��������� DXGI_SWAP_CHAIN_DESC
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// ��������� ��������� �������
	scd.BufferCount = 1;                                    // ���� ������ �����
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // ���������� 32 ������ ����
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // ��� swap chain ����� ��������������
	scd.OutputWindow = hWnd;                                // ���� ��� �������������
	scd.SampleDesc.Count = 1;                               // ��� ����� ����� multisample-��
	scd.SampleDesc.Quality = 0;                             // �������� multisample-��
	scd.Windowed = TRUE;                                    // ����� ����: windowed/full-screen mode

	//������� ����������, �������� ���������� � swap chain ��������� ���������� �� ��������� scd
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

	// �������� ����� ��� ������� ������
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// ���������� ����� ������� ������ ��� �������� ����������
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// ������������� ��������� ��� ������ �����
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// ������ �������������
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800;
	viewport.Height = 600;

	devcon->RSSetViewports(1, &viewport);
}

void RenderFrame(int &red, int &green, int &blue, int &step)
{
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR((float)red / 100000, (float)green / 100000, (float)blue / 100000, 1.0f));
	swapchain->Present(0, 0);

	// ������ �������
	if (red == 100000 && green < 100000 && blue == 0) {
		green += step;
	}
	else if (red > 0 && green == 100000 && blue == 0) {
		red -= step;
	}
	else if (red == 0 && green == 100000 && blue == 0) {
		blue = 100000;
	}

	// ������ �����
	else if (green == 100000 && blue < 100000 && red == 0) {
		blue += step;
	}
	else if (green > 0 && blue == 100000 && red == 0) {
		green -= step;
	}
	else if (green == 0 && blue == 100000 && red == 0) {
		red = 100000;
	}

	// ������ �������
	else if (blue == 100000 && red < 100000 && green == 0) {
		red += step;
	}
	else if (blue > 0 && red == 100000 && green == 0) {
		blue -= step;
	}
	else if (blue == 0 && red == 100000 && green == 0) {
		green = 100000;
	}
}

// ������� ������� ������
void CleanD3D(void)
{
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}