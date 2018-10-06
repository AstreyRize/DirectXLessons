//http://www.directxtutorial.com/Lesson.aspx?lessonid=11-1-2
// Подключаем заголовочный файл для работы с окнами windows.
#include <windows.h>    

// Точка входа в программу.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// Создаем окно "Hello World!".
	MessageBox(NULL, "Hello World!", "Just another Hello World program!", MB_ICONEXCLAMATION | MB_OK);

	// Выходим из программы.
	return 0;
}