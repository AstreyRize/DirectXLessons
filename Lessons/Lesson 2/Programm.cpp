//http://www.directxtutorial.com/Lesson.aspx?lessonid=11-1-2
// ���������� ������������ ���� ��� ������ � ������ windows.
#include <windows.h>    

// ����� ����� � ���������.
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	// ������� ���� "Hello World!".
	MessageBox(NULL, "Hello World!", "Just another Hello World program!", MB_ICONEXCLAMATION | MB_OK);

	// ������� �� ���������.
	return 0;
}