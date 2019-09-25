#pragma once

// The Windows Runtime C++ Template Library (WRL) is a template library that provides a low-level way to author and use Windows Runtime components.
// Шаблон времени выполнения Windows - шаблон библиотеки, который предоставляет низкоуровневый путь для использования компонентов времени выполнения.
#include <wrl.h>

// 
#include <wrl/client.h>

// Microsoft DirectX Graphics Infrastructure (DXGI) handles enumerating graphics adapters, enumerating display modes, selecting buffer formats, 
// sharing resources between processes (such as, between applications and the Desktop Window Manager (DWM)), 
// and presenting rendered frames to a window or monitor for display.

// Инфраструктура Microsoft DirectX Graphics содержит перечисление адаптеров графики, перечисление модулей отображения, форматы буферов,
// делит ресурсы между процессами (например, между приложением и Desktop Window Manager (DWM)),
// и представляет отрисованные кадры для отображения.
#include <dxgi1_4.h>
#include <d3d11_3.h>
#include <d2d1_3.h>
#include <d2d1effects_2.h>
#include <dwrite_3.h>
#include <wincodec.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <memory>
#include <agile.h>
#include <concrt.h>