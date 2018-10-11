// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-3
// Добавляем в проект заголовочные файлы для windows и DirectX
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// Добавляем библиотеки DirectX
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// Глобальные переменные
IDXGISwapChain *swapchain;										// Указатель на swap chain interface
ID3D11Device *dev;												// Указатель на Direct3D device interface
ID3D11DeviceContext *devcon;									// Указатель на Direct3D device context
ID3D11RenderTargetView *backbuffer;								// Указатель на задний буфер

// Прототипы функций
void InitD3D(HWND hWnd);										// Настраиваем и инициализируем DirectX
void RenderFrame(int &red, int &green, int &blue, int &step);	// Рендерим один фрейм
void CleanD3D(void);											// Закрываем DirectX и освобождаем память

// Прототип функции WindowProc
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Точка входа в приложение
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

	// Настраиваем и инициализируем DirectX
	InitD3D(hWnd);

	MSG msg;
	int red = 100000;
	int green = 0;
	int blue = 0;
	int step = 1;

	// Бесконечный цикл игры
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

	// Освобождаем ресурсы
	CleanD3D();

	return msg.wParam;
}

// Обработчик событий
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

// Настройка и инициализация DirectX
void InitD3D(HWND hWnd)
{
	// Создание структуры для хранения информации о swap chain
	DXGI_SWAP_CHAIN_DESC scd;

	// Выделяем память под структуру DXGI_SWAP_CHAIN_DESC
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// Заполняем структуру данными
	scd.BufferCount = 1;                                    // Одни задний буфер
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // Используем 32 битный цвет
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // Как swap chain будет использоваться
	scd.OutputWindow = hWnd;                                // Окно для использования
	scd.SampleDesc.Count = 1;                               // Как много будет multisample-ов
	scd.SampleDesc.Quality = 0;                             // Качество multisample-ов
	scd.Windowed = TRUE;                                    // Режим окна: windowed/full-screen mode

	//Создаем устройство, контекст устройства и swap chain используя информация из структуры scd
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

	// Получаем адрес для заднего буфера
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// Используем адрес заднего буфера для создания контейнера
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// Устанавливаем контейнер как задний буфер
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Задаем представление
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

	// Растет зеленый
	if (red == 100000 && green < 100000 && blue == 0) {
		green += step;
	}
	else if (red > 0 && green == 100000 && blue == 0) {
		red -= step;
	}
	else if (red == 0 && green == 100000 && blue == 0) {
		blue = 100000;
	}

	// Растет синий
	else if (green == 100000 && blue < 100000 && red == 0) {
		blue += step;
	}
	else if (green > 0 && blue == 100000 && red == 0) {
		green -= step;
	}
	else if (green == 0 && blue == 100000 && red == 0) {
		red = 100000;
	}

	// Растет красный
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

// Функция очистки памяти
void CleanD3D(void)
{
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}