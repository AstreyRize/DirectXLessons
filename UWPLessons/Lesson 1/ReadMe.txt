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

����� APP.
����� App ��������� ��������� IFrameworkView � ��������� ��� ������ Initialize, SetWindow, Load, Run, Uninitialize.

� ������ Initialize ������������� �� ��������� ������� CoreApplicationView, � ��� �� ������� ������ � ������� ������ std::make_shared.
����� std::make_shared ������� ��� ���������� ����� ���������. ����� ���������� ��������� �����: unique_ptr,
shared_ptr � weak_ptr. � ������ ������ ��������� ��������� shared_ptr �� ������ DX::DeviceResources.
shared_ptr ��������� ������� ������ �� ������. ������ ����������� �����, ����� ������� ������ �� ���� ����� ����� 0.

� ������ Load ������� ��� ���� ����� ��������� �� ����� Lesson_1Main. � ����������� ����� ������ �������� ��������� � ������
Initialize ������ m_deviceResources. ����� Lesson_1Main ��������� ��������� DX::IDeviceNotify. �� ����� ���������� ������-�� ���
������� ����������. ��������� � ������� ����� ������ ����� ���������� �������������� ����� � ����������� �����������.

� ������ Run ����������� ����������� ����, ������� ��������, ���� ���� �� �������� ���������. � ������ ����� ���������, ��� ���� ������������
(�� ��������) � ����������� ������� ���������� �����������: m_main->Update(); �� ��������� ��������� ���������: m_main->Render().
���� ��������� �����������, ���������� ����: m_deviceResources->Present();