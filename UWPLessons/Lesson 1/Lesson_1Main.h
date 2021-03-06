﻿#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"

// Renders Direct2D and 3D content on the screen.

// Отрисовывает Direct2D и 3D содержимое сцены.
namespace Lesson_1
{
	class Lesson_1Main : public DX::IDeviceNotify
	{
	public:
		Lesson_1Main(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~Lesson_1Main();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// Cached pointer to device resources.

		// Кэширует указатель на ресурсы устройства.
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: Replace with your own content renderers.
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// Rendering loop timer.

		// Таймер цикла отрисовки.
		DX::StepTimer m_timer;
	};
}