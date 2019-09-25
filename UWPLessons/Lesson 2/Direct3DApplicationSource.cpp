#include "pch.h"
#include "Direct3DApplicationSource.h"
#include "Window.h"

using namespace Lesson_2;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;

IFrameworkView^ Direct3DApplicationSource::CreateView()
{
	return ref new Window();
}
