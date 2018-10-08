// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-2
// Включаем в проект заголовочные файлы для windows и DirectX.
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// Включаем библиотеки Direct3D.
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// global declarations
IDXGISwapChain *swapchain;             // Указатель на интерфейс swap chain.
ID3D11Device *dev;                     // Указатель на Direct3D интерфейс.
ID3D11DeviceContext *devcon;           // Указатель на Direct3D device context.

// Прототипы функций.
void InitD3D(HWND hWnd);    // Инициализация Direct3D.
void CleanD3D(void);        // Финализация Direct3D.

// Прототип функции.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);


// Точка входа в программу.
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

	// Инициализация DirectX.
	InitD3D(hWnd);

	// Бесконечный цикл.
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
		else
		{
			// Run game code here
			// ...
			// ...
		}
	}

	// Финализация DirectX.
	CleanD3D();

	return msg.wParam;
}

// Обработчик событий.
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

// Данная функция инициализирует DirectX и подготавливает для использования.
void InitD3D(HWND hWnd)
{
	// Создаем структуру для хранения информации о swap chain.
	DXGI_SWAP_CHAIN_DESC scd;

	// Выделяем помять под структуру.
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Заполняем структуру данными.
	scd.BufferCount = 1;                                    // Один задний буфер.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Использовать 32-bit цвет.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // Как будет использоваться swap chain.
	scd.OutputWindow = hWnd;                                // Окно, которое будет использоваться.
	scd.SampleDesc.Count = 4;                               // Как много multisample-ов.
	scd.Windowed = TRUE;                                    // windowed/full-screen mode

	// Создать устройство, контекст устройства и swap chain используя информацию из scd.
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
}

// Освобождаем помять.
void CleanD3D(void)
{
	swapchain->Release();
	dev->Release();
	devcon->Release();
}