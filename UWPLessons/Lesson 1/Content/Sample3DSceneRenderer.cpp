#include "pch.h"
#include "Sample3DSceneRenderer.h"
#include "..\Common\DirectXHelper.h"

using namespace Lesson_1;
using namespace DirectX;
using namespace Windows::Foundation;

// Loads vertex and pixel shaders from files and instantiates the cube geometry.
// Загружаем вершины и шейдеры пикселей из файла и создаем экземпляр геометрии куба.
Sample3DSceneRenderer::Sample3DSceneRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_loadingComplete(false),
	m_degreesPerSecond(45),
	m_indexCount(0),
	m_tracking(false),
	m_deviceResources(deviceResources)
{
	CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}

// Initializes view parameters when the window size changes.
// Инициализируем параметры отображения когда размер окна меняется.
void Sample3DSceneRenderer::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// This is a simple example of change that can be made when the app is in
	// portrait or snapped view.
	// 	Это простой пример изменения которое может произойти когда приложение в альбомном или портретном режиме.
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// Обратите внимание, что матрица OrientationTransform3D умножается здесь 
	// для правильной ориентации сцены в соответствии с ориентацией дисплея. 
	// Этот шаг после умножения необходим для любых вызовов draw, которые 
	// сделано для цели рендеринга цепочки свопов. Для вызовов на другие цели, 
	// это преобразование не должно применяться.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	// В этом примере используется правосторонняя система координат с использованием матриц основных строк.
	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(fovAngleY, aspectRatio, 0.01f, 100.0f);
	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(&m_constantBufferData.projection, XMMatrixTranspose(perspectiveMatrix * orientationMatrix));

	// Eye is at (0,0.7,1.5), looking at point (0,-0.1,0) with the up-vector along the y-axis.
	// Глаз находится на (0,0.7,1.5), глядя на точку (0,-0.1,0) с вектором вверх вдоль оси Y.
	static const XMVECTORF32 eye = { 60.0f, 60.0f, 0.0f, 0.0f };
	static const XMVECTORF32 at = { 0.0f, -0.1f, 0.0f, 0.0f };
	static const XMVECTORF32 up = { 0.0f, 1.0f, 0.0f, 0.0f };

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixTranspose(XMMatrixLookAtRH(eye, at, up)));
}

// Called once per frame, rotates the cube and calculates the model and view matrices.
// Вызывается раз в кадр, поворачивает куб и рассчитывает модель и отображение матрицы.
void Sample3DSceneRenderer::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// Convert degrees to radians, then convert seconds to rotation angle
		// Конвертирует градусы в радианы, затем конвертирует секунды в углы поворота.
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
	}
}

// Rotate the 3D cube model a set amount of radians.
// Поворачивает 3д модель куба на заданное количество радиан.
void Sample3DSceneRenderer::Rotate(float radians)
{
	// Prepare to pass the updated model matrix to the shader
	// Подготовка к передаче обновленной матрицы модели шейдеру
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixTranspose(XMMatrixRotationY(radians)));
}

void Sample3DSceneRenderer::StartTracking()
{
	m_tracking = true;
}

// When tracking, the 3D cube can be rotated around its Y axis by tracking pointer position relative to the output screen width.
// При отслеживании трехмерный куб можно вращать вокруг своей оси Y, отслеживая положение указателя относительно ширины выходного экрана.
void Sample3DSceneRenderer::TrackingUpdate(float positionX)
{
	if (m_tracking)
	{
		float radians = XM_2PI * 2.0f * positionX / m_deviceResources->GetOutputSize().Width;
		Rotate(radians);
	}
}

void Sample3DSceneRenderer::StopTracking()
{
	m_tracking = false;
}

// Renders one frame using the vertex and pixel shaders.
// Отрисовывает один кадр используя вершины и пиксельные шейдеры.
void Sample3DSceneRenderer::Render()
{
	// Loading is asynchronous. Only draw geometry after it's loaded.
	// Загружаем асинхронно. Рисуем геометрию после того, как загрузится.
	if (!m_loadingComplete)
	{
		return;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// Prepare the constant buffer to send it to the graphics device.
	// Подготавливаем постоянный буфер для отправки его на графическое устройство.
	context->UpdateSubresource1(m_constantBuffer.Get(), 0, NULL, &m_constantBufferData, 0, 0, 0);

	// Each vertex is one instance of the VertexPositionColor struct.
	// Каждая вершина - один экземпляр структуры VertexPositionColor.
	UINT stride = sizeof(VertexPositionColor);
	UINT offset = 0;

	context->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);

	// Each index is one 16-bit unsigned integer (short). Каждый индекс - один 16-ти битный без знаковый int.
	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetInputLayout(m_inputLayout.Get());

	// Attach our vertex shader.
	// Присоединяем наш шейдер вершин.
	context->VSSetShader(m_vertexShader.Get(), nullptr, 0);

	// Send the constant buffer to the graphics device.
	// Передаем буфер в графическое устройство.
	context->VSSetConstantBuffers1(0, 1, m_constantBuffer.GetAddressOf(), nullptr, nullptr);

	// Attach our pixel shader.
	// Присоединяем наш пиксельный шейдер.
	context->PSSetShader(m_pixelShader.Get(), nullptr, 0);

	// Draw the objects.
	// Рисуем объект.
	context->DrawIndexed(m_indexCount, 0, 0);
}

void Sample3DSceneRenderer::CreateDeviceDependentResources()
{
	// Load shaders asynchronously.
	// Загрузка шейдеров асинхронно.
	auto loadVSTask = DX::ReadDataAsync(L"SampleVertexShader.cso");
	auto loadPSTask = DX::ReadDataAsync(L"SamplePixelShader.cso");

	// After the vertex shader file is loaded, create the shader and input layout.
	// После шейдеров вершин файл загружается, создает шейдеры и выходной слой.
	auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
		m_deviceResources->GetD3DDevice()->CreateVertexShader(&fileData[0], fileData.size(), nullptr, &m_vertexShader);

		static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		m_deviceResources->GetD3DDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), &fileData[0], fileData.size(), &m_inputLayout);
		});

	// After the pixel shader file is loaded, create the shader and constant buffer.
	// После пиксельных шейдеров файл загружается, создает шейдеры и буфер констант.
	auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
		m_deviceResources->GetD3DDevice()->CreatePixelShader(&fileData[0], fileData.size(), nullptr, &m_pixelShader);
		CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
		m_deviceResources->GetD3DDevice()->CreateBuffer(&constantBufferDesc, nullptr, &m_constantBuffer);
		});

	// Once both shaders are loaded, create the mesh.
	// Когда оба шейдера загружены, создаем мешь.
	auto createCubeTask = (createPSTask && createVSTask).then([this]() {
		// Load mesh vertices. Each vertex has a position and a color.
		// Загружаем мешь вершин. Каждая вершина имеет позицию и цвет.
		DX::ModelHelper modelHelper(L"\\Assets\\Корпус 1.stl");
		std::vector<VertexPositionColor> cubeVertices = modelHelper.GetVertices();

		D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
		vertexBufferData.pSysMem = &cubeVertices[0];
		vertexBufferData.SysMemPitch = 0;
		vertexBufferData.SysMemSlicePitch = 0;

		CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices[0]) * cubeVertices.size(), D3D11_BIND_VERTEX_BUFFER);

		m_deviceResources->GetD3DDevice()->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &m_vertexBuffer);

		// Load mesh indices. Each trio of indices represents
		// a triangle to be rendered on the screen.
		// For example: 0,2,1 means that the vertices with indexes
		// 0, 2 and 1 from the vertex buffer compose the 
		// first triangle of this mesh.

		// Загружаем индексы сетки.Каждая тройка индексов представляет
		// треугольник, отображаемый на экране.
		// Например: 0,2,1 означает, что вершины с индексами
		// 0, 2 и 1 из буфера вершин составляют
		// первый треугольник этой сетки.
		std::vector<unsigned short> cubeIndices = modelHelper.GetReletionships();

		m_indexCount = cubeIndices.size();

		D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
		indexBufferData.pSysMem = &cubeIndices[0];
		indexBufferData.SysMemPitch = 0;
		indexBufferData.SysMemSlicePitch = 0;

		CD3D11_BUFFER_DESC indexBufferDesc(sizeof(unsigned short) * cubeVertices.size(), D3D11_BIND_INDEX_BUFFER);

		m_deviceResources->GetD3DDevice()->CreateBuffer(&indexBufferDesc, &indexBufferData, &m_indexBuffer);
		});

	// Once the cube is loaded, the object is ready to be rendered.
	// Когда куб загружен, объект готов быть загружен.
	createCubeTask.then([this]() {
		m_loadingComplete = true;
		});
}

void Sample3DSceneRenderer::ReleaseDeviceDependentResources()
{
	m_loadingComplete = false;
	m_vertexShader.Reset();
	m_inputLayout.Reset();
	m_pixelShader.Reset();
	m_constantBuffer.Reset();
	m_vertexBuffer.Reset();
	m_indexBuffer.Reset();
}