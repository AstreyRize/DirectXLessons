#pragma once

#include "pch.h"
#include "Common\DeviceResources.h"
#include "Lesson_2Main.h"

namespace Lesson_2
{
	ref class Direct3DApplicationSource sealed : Windows::ApplicationModel::Core::IFrameworkViewSource
	{
		public: virtual Windows::ApplicationModel::Core::IFrameworkView^ CreateView();
	};
}