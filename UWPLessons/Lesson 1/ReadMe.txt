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