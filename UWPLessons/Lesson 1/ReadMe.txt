�������� ����� ����������.
������ ����� ������� �������� �� ��� ���������� IFrameworkViewSource � IFrameworkView

IFrameworkViewSource - ������������ ����� ������� ��� �������� �������������. ���������� ��������,
������� ��������� ���� ���������, �������� �� ���������� ����� CreateView.

    CreateView - ������ ����� ���������� ��������� IFrameworkView.

IFrameworkView - ������������ ����� ��������� ���������� ������������� ����������. �����, �������
��� ���������, �������� � ���������� ��������� �������:

    Initialize - ��� ������ �����, ���������� � ���������� �������������, ����� ������ ���������� �����������.
�� �� ����������  CoreApplication.Run.
    SetWindow - ���� ����� ���������� ����� ������ Initialize.
    Load - ��������� ��� ���������� ����� ������� �������, ������������ �������������� ���������� ����� ������� Run.
� ���� ������ �� ������� ��� ������ ������������� �������, ����������� ��� DirectX, ����� ��� ������� ����������� ����������.
    Run - ���� ����� ���������� ����� Load. ��������� ������� ������� ����������� ������������� ��� ��������� ����������
�������������.
    Uninitialize - ���� ����� ���������� ����� ����, ��� ���������� ���� �������.

��� ��� ��� �������� � �����. � ����� ����� ���������� � ������� MyFrameworkViewSource, ������� ���������
��������� IFrameworkViewSource. � �������� � ���������� �����, ������� ��������� ��������� IFrameworkView.
�������� ������ ����� � CoreApplication.Run.

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto frameworkViewSource = ref new MyFrameworkViewSource();
    Windows::ApplicationModel::Core::CoreApplication::Run(frameworkViewSource);
    return 0;
}

��� �������� ���������� �������. ��� �����, ����� CreateView ���������� ��������� IFrameworkView.
� ������ ������ ��������� IFrameworkView ��������� ����� MyFrameworkView.

ref class MyFrameworkViewSource : IFrameworkViewSource
{
public:
    virtual IFrameworkView^ CreateView()
    {
        return ref new MyFrameworkView();
    }
};