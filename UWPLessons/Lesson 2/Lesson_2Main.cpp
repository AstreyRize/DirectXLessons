#include "pch.h"
#include "Lesson_2Main.h"
#include "Common\DirectXHelper.h"

using namespace Lesson_2;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;

Lesson_2Main::Lesson_2Main(const std::shared_ptr<DX::DeviceResources>& deviceResources) : m_deviceResources(deviceResources)
{
	m_deviceResources->RegisterDeviceNotify(this);

	m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));
	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));
}

Lesson_2Main::~Lesson_2Main()
{
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

void Lesson_2Main::CreateWindowSizeDependentResources() 
{
	m_sceneRenderer->CreateWindowSizeDependentResources();
}

void Lesson_2Main::Update() 
{
	m_timer.Tick([&]()
	{
		m_sceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

bool Lesson_2Main::Render() 
{
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();
	auto viewport = m_deviceResources->GetScreenViewport();

	context->RSSetViewports(1, &viewport);

	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_sceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

void Lesson_2Main::OnDeviceLost()
{
	m_sceneRenderer->ReleaseDeviceDependentResources();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

void Lesson_2Main::OnDeviceRestored()
{
	m_sceneRenderer->CreateDeviceDependentResources();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
