// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-1-4
// Подключаем заголовочные файлы.
#include <windows.h>
#include <windowsx.h>

// Прототип функции WindowProc.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// Дескриптор для окна, заполненный функцией.
	HWND hWnd;

	// Данная структура хранит информацию для класса окна.
	WNDCLASSEX wc;

	// Очищаем класс окна для использования.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// Заполняем структуру нужными данными.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// Регистрируем класс окна.
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 500, 400 };    // Задаем размер, но не позицию
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // Применяем позицию

	// Создаем окно и получаем дескриптор окна.
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // Название класса окна.
		"Our First Windowed Program",   // Заголовок окна.
		WS_OVERLAPPEDWINDOW,    // Стиль окна.
		300,    // Позиция окна по оси X.
		300,    // Позиция окна по оси Y.
		wr.right - wr.left,    // Ширина окна.
		wr.bottom - wr.top,    // Высота окна.
		NULL,    // Родительское окно, NULL
		NULL,    // Используем ли мы меню, NULL
		hInstance,    // Дескриптор приложения
		NULL);    // Используется с множественными окнами, NULL

	// Отображаем окно на экране.
	ShowWindow(hWnd, nCmdShow);

	// Начало главного цикла:
	// Эта структура хранит события окна.
	MSG msg;

	// Ожидание следующего события в очереди, сохраняем результат в msg.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		// Переводим нажатие клавиш в нужный формат.
		TranslateMessage(&msg);

		// Передаем сообщение в функцию WindowProc.
		DispatchMessage(&msg);
	}

	// return this part of the WM_QUIT message to Windows
	return msg.wParam;
}

// Это главный обработчик событий в программе.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Определяем откуда пришло событие.
	switch (message)
	{
		// Это событие вызвано закрытием окна.
	case WM_DESTROY:
	{
		// Закрываем приложение.
		PostQuitMessage(0);
		return 0;
	}
	break;
	}

	// Обрабатываем все остальные сообщения, которые мы не обработали в switch.
	return DefWindowProc(hWnd, message, wParam, lParam);
}