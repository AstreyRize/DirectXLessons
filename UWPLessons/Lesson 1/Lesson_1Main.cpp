#include "pch.h"
#include "Lesson_1Main.h"
#include "Common\DirectXHelper.h"

using namespace Lesson_1;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

// Loads and initializes application assets when the application is loaded.

// Загружаем и инициализируем активы приложения пока приложение загружается.
Lesson_1Main::Lesson_1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// Register to be notified if the Device is lost or recreated

	// Регистрируемся, что бы получать уведомления если устройство будет потеряно или пересоздано.
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: Replace this with your app's content initialization.

	// TODO: Замените это своим инициализатором контента.
	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	// TODO: Change the timer settings if you want something other than the default variable timestep mode.
	// e.g. for 60 FPS fixed timestep update logic, call:

	// TODO: Изменить настройки времени если вы хотите что-то другое чем шаг времени по умолчанию.
	// т.е. для 60 кадров в секунду вызовете:

	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/
}

Lesson_1Main::~Lesson_1Main()
{
	// Deregister device notification

	// Отписываемся от уведомлений.
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// Updates application state when the window size changes (e.g. device orientation change)

// Обновляем состояние приложения когда размер окна изменится.
void Lesson_1Main::CreateWindowSizeDependentResources() 
{
	// TODO: Replace this with the size-dependent initialization of your app's content.

	// TODO: Замените это размеро-зависимой инициализацией. 
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

// Updates the application state once per frame.

// Обновляет состояние приложения раз в кадр.
void Lesson_1Main::Update() 
{
	// Update scene objects.

	// Обновляет объекты сцены.
	m_timer.Tick([&]()
	{
		// TODO: Replace this with your app's content update functions.
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// Renders the current frame according to the current application state.

// Рендерит текущий кадр в соответствии с текущем состоянием приложения.

// Returns true if the frame was rendered and is ready to be displayed.

// Возвращает true, если кадр был отрисован и готов быть отображен.
bool Lesson_1Main::Render() 
{
	// Don't try to render anything before the first Update.

	// Не пытайтесь отрисовать что-то до первого обновления (Update).
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Reset the viewport to target the whole screen.

	// Сбрасываем окно просмотра до пустой сцены.
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// Reset render targets to the screen.

	// Сбрасываем отрисовку сцены.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.

	// Очищаем задний буффер и  какой-то трафарет.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Render the scene objects.

	// Отрисовываем объекты сцены.

	// TODO: Replace this with your app's content rendering functions.

	// TODO: Замените это своей функцией отрисовки.
	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

// Notifies renderers that device resources need to be released.

// Уведомляет, что ресурсы устройства должны быть выгружены из памяти.
void Lesson_1Main::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// Notifies renderers that device resources may now be recreated.

// Уведомляет, что ресурсы устройства могут быть пересозданы.
void Lesson_1Main::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
