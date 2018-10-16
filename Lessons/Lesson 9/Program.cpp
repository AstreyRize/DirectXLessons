// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-4-5
// Добавляем заголовочные файлы
#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <d3dx11.h>
#include <d3dx10.h>

// Добавляем библиотеки DirectX
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment (lib, "d3dx10.lib")

// Устанавливаем разрешение экрана
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600

// Глобальные переменные
IDXGISwapChain *swapchain;				// Указатель на swap chain interface
ID3D11Device *dev;						// Указатель на Direct3D device interface
ID3D11DeviceContext *devcon;			// Указатель на Direct3D device context
ID3D11RenderTargetView *backbuffer;		// Указатель на задний буфер
ID3D11InputLayout *pLayout;				// Указатель на входной слой
ID3D11VertexShader *pVS;				// Указатель на шейдер вершин
ID3D11PixelShader *pPS;					// Указатель на шейдер пикселей
ID3D11Buffer *pVBuffer;					// Указатель на буфер вершин

// Структура вершины
struct VERTEX
{
	FLOAT X, Y, Z;
	D3DXCOLOR Color;
};

// Прототип функции
void InitD3D(HWND hWnd);				// Настройка и инициализация DirectX
void RenderFrame(void);					// Рендер кадра
void CleanD3D(void);					// Освобождаем память DirectX
void InitGraphics(void);				// Создаем форму для рендера
void InitPipeline(void);				// Загружаем и подготавливаем шейдеры

// Обработчик событий
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Точка входа в программу
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

	// Настройка и инициализация Direct3D
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


// Инициализируем и подготавливаем DirectX.
void InitD3D(HWND hWnd)
{
	// Создаем структуру для хранения swap chain.
	DXGI_SWAP_CHAIN_DESC scd;

	// Выделяем память под структуру.
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	// fill the swap chain description struct
	scd.BufferCount = 1;									// Один задний буфер.
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;		// 32-bit цвет.
	scd.BufferDesc.Width = SCREEN_WIDTH;					// Устанавливаем ширину заднего буфера.
	scd.BufferDesc.Height = SCREEN_HEIGHT;					// Устанавливаем высоту заднего буфера.
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;		// Определяем как будет использоваться swap chain.
	scd.OutputWindow = hWnd;								// Дескриптор окна, который будет использоваться.
	scd.SampleDesc.Count = 4;								// Задаем количество multisample-ов.
	scd.Windowed = TRUE;									// Оконный режим.
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;		// Разрешаем полноэкранный режим.

	// Создаем устройство, контекст устройства и swap chain, используя информацию из scd.
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

	// Получаем адрес заднего буфера.
	ID3D11Texture2D *pBackBuffer;
	swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);

	// Используя адрес заднего буфера для создания контейнера для рендера.
	dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	pBackBuffer->Release();

	// Задаем контейнер для рендера как задний буфер.
	devcon->OMSetRenderTargets(1, &backbuffer, NULL);

	// Устанавливаем представление.
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

// Рендер одного кадра.
void RenderFrame(void)
{
	// Очищаем задний буфер и устанавливаем синий цвет.
	devcon->ClearRenderTargetView(backbuffer, D3DXCOLOR(0.0f, 0.2f, 0.4f, 1.0f));

	// Выбираем буфер вершин для отображения.
	UINT stride = sizeof(VERTEX);
	UINT offset = 0;
	devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

	// Выбираем тип приметива, который будем использовать.
	devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Рисуем вершины из буфера вершин в задний буфер.
	devcon->Draw(3, 0);

	// Переключаем задний буфер и передний буфер.
	swapchain->Present(0, 0);
}

// Эта функция, которая очищает DirectX и COM объекты.
void CleanD3D(void)
{
	// Переключаем режим окна на оконный.
	swapchain->SetFullscreenState(FALSE, NULL);

	// Закрываем и освобождаем ресурсы.
	pLayout->Release();
	pVS->Release();
	pPS->Release();
	pVBuffer->Release();
	swapchain->Release();
	backbuffer->Release();
	dev->Release();
	devcon->Release();
}

// В этой функции создаем объект для рендера.
void InitGraphics()
{
	// Создаем треугольник используя структуру VERTEX.
	VERTEX OurVertices[] =
	{
		{0.0f, 0.5f, 0.0f, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f)},
		{0.45f, -0.5, 0.0f, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f)},
		{-0.45f, -0.5f, 0.0f, D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f)}
	};

	// Создаем буфер вершин.
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));

	bd.Usage = D3D11_USAGE_DYNAMIC;										// Устанавливаем уровень доступа для CPU и GPU.
	bd.ByteWidth = sizeof(VERTEX) * 3;									// Размер для трех структур VERTEX.
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;							// Используем как буфер вершин.
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;							// Позволить CPU писать в буфер.

	dev->CreateBuffer(&bd, NULL, &pVBuffer);							// Создаем буфер

	// Копируем вершины в буфер.
	D3D11_MAPPED_SUBRESOURCE ms;
	devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);	// Мапим буфер.
	memcpy(ms.pData, OurVertices, sizeof(OurVertices));					// Копируем данные.
	devcon->Unmap(pVBuffer, NULL);										// Размапим буфер.
}

// Эта функция загружает и подготавливает шейдеры.
void InitPipeline()
{
	// Загружаем и компилируем шейдеры.
	ID3D10Blob *VS, *PS;
	D3DX11CompileFromFile("shaders.shader", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, 0, 0);
	D3DX11CompileFromFile("shaders.shader", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, 0, 0);

	// Инкапсулируем оба шейдеры в объект.
	dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
	dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

	// Задаем объекты шейдеров.
	devcon->VSSetShader(pVS, 0, 0);
	devcon->PSSetShader(pPS, 0, 0);

	// Создаем объект входного слоя.
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
	devcon->IASetInputLayout(pLayout);
}