#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

namespace Lesson_2
{
	class Lesson_2Main : public DX::IDeviceNotify
	{
	public:
		Lesson_2Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Lesson_2Main();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		DX::StepTimer m_timer;
	};
}