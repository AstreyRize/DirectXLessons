СОЗДАНИЕ ХОСТА ПРИЛОЖЕНИЯ.
Первым делом обратим внимание на два интерфейса IFrameworkViewSource и IFrameworkView

IFrameworkViewSource - представляет собой фабрику для создания представления. Экземпляры объектов,
которые расширяют этот интерфейс, получают по наследству метод CreateView.

    CreateView - Данный метод возвращает интерфейс IFrameworkView.

IFrameworkView - представляет собой интерфейс поставщика представления приложения. Класс, который
его расширяет, получает в наследство несколько методов:

    Initialize - Это первый метод, вызываемый в реализации представления, когда объект приложения активирован.
Он же вызывается  CoreApplication.Run.
    SetWindow - Этот метод вызывается после метода Initialize.
    Load - Загружает или активирует любые внешние ресурсы, используемые представлением приложения перед вызовом Run.
В этом методе мы создаем для своего представления ресурсы, специфичные для DirectX, такие как целевая поверхность рендеринга.
    Run - Этот метод вызывается после Load. Запускает фабрику внешних провайдеров представлений для получения провайдера
представлений.
    Uninitialize - Этот метод вызывается после того, как приложение было закрыто.

Вот как это выглядит в итоге. В точке входа обращаемся к фабрике MyFrameworkViewSource, которая расширяет
интерфейс IFrameworkViewSource. И получаем в результате класс, который расширяет интерфейс IFrameworkView.
Передаем данный класс в CoreApplication.Run.

[Platform::MTAThread]
int main(Platform::Array<Platform::String^>^)
{
    auto frameworkViewSource = ref new MyFrameworkViewSource();
    Windows::ApplicationModel::Core::CoreApplication::Run(frameworkViewSource);
    return 0;
}

Так выглядит реализация фабрики. Как видим, метод CreateView возвращает интерфейс IFrameworkView.
В данном случае интерфейс IFrameworkView расширяет класс MyFrameworkView.

ref class MyFrameworkViewSource : IFrameworkViewSource
{
public:
    virtual IFrameworkView^ CreateView()
    {
        return ref new MyFrameworkView();
    }
};

КЛАСС APP.
Класс App расширяет интерфейс IFrameworkView и реализует его методы Initialize, SetWindow, Load, Run, Uninitialize.

В методе Initialize подписываемся на некоторые события CoreApplicationView, а так же создаем ресурс с помощью метода std::make_shared.
Метод std::make_shared создает так называемый умный указатель. Таких указателей несколько видов: unique_ptr,
shared_ptr и weak_ptr. В данном случае создается указатель shared_ptr на ресурс DX::DeviceResources.
shared_ptr реализует подсчет ссылок на ресурс. Ресурс освободится тогда, когда счетчик ссылок на него будет равен 0.

В методе Load создаем еще один умный указатель на класс Lesson_1Main. В конструктор этого класса передаем созданный в методе
Initialize ресурс m_deviceResources. Класс Lesson_1Main расширяет интерфейс DX::IDeviceNotify. По этому интерфейсу почему-то нет
никакой информации. Насколько я понимаю через методы этого интерфейса осуществляется связь с графическим устройством.

В методе Run запускается бесконечный цикл, который работает, пока окно не окажется свернутым. В данном цикле проверяем, что окно отображается
(не свернуто) и запускается процесс обновления содержимого: m_main->Update(); По окончанию запускаем отрисовку: m_main->Render().
Если отрисовка закончилась, показываем кадр: m_deviceResources->Present();