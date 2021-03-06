// http://www.directxtutorial.com/Lesson.aspx?lessonid=11-1-5
// ���������� ������������ �����.
#include <windows.h>
#include <windowsx.h>

// �������� ������� WindowProc.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// ����� ����� � ���������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// ���������� ��� ����, ����������� ��������.
	HWND hWnd;

	// ������ ��������� ������ ���������� ��� ������ ����.
	WNDCLASSEX wc;

	// ������� ����� ���� ��� �������������.
	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	// ��������� ��������� ������� �������.
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "WindowClass1";

	// ������������ ����� ����.
	RegisterClassEx(&wc);

	RECT wr = { 0, 0, 500, 400 };    // ������ ������, �� �� �������
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);    // ��������� �������

	// ������� ���� � �������� ���������� ����.
	hWnd = CreateWindowEx(NULL,
		"WindowClass1",    // �������� ������ ����.
		"Our First Windowed Program",   // ��������� ����.
		WS_OVERLAPPEDWINDOW,    // ����� ����.
		300,    // ������� ���� �� ��� X.
		300,    // ������� ���� �� ��� Y.
		wr.right - wr.left,    // ������ ����.
		wr.bottom - wr.top,    // ������ ����.
		NULL,    // ������������ ����, NULL
		NULL,    // ���������� �� �� ����, NULL
		hInstance,    // ���������� ����������
		NULL);    // ������������ � �������������� ������, NULL

	// ���������� ���� �� ������.
	ShowWindow(hWnd, nCmdShow);

	// ������ �������� �����:
	// ��� ��������� ������ ������� ����.
	MSG msg = { 0 };

	// ����������� ����.
	while (TRUE)
	{
		// ���������, ���� �� ������� � �������
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// ��������� ������� � ���������� ������.
			TranslateMessage(&msg);

			// �������� ������� � ����� WindowProc.
			DispatchMessage(&msg);

			// ���������, �� ����� �� ��� ����� �� �����.
			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			// ����� ��� ����.
			// ...
			// ...
		}
	}

	// ������ ��� ����� ��������� WM_QUIT � Windows.
	return msg.wParam;
}

// ��� ������� ���������� ������� � ���������.
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// ���������� ������ ������ �������.
	switch (message)
	{
		// ��� ������� ������� ��������� ����.
	case WM_DESTROY:
	{
		// ��������� ����������.
		PostQuitMessage(0);
		return 0;
	}
	break;
	}

	// ������������ ��� ��������� ���������, ������� �� �� ���������� � switch.
	return DefWindowProc(hWnd, message, wParam, lParam);
}