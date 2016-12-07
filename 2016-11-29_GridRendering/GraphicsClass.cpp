#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
	m_Direct3D = 0;
	 m_Camera = 0;
	 m_Model = 0;
	 m_ColorShader = 0;

}

GraphicsClass::GraphicsClass(const GraphicsClass &)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	m_Direct3D = new D3DClass;

	if (!m_Direct3D)
		return false;

	result = m_Direct3D->Initialize(screenWidth,
		screenHeight,
		VSYNC_ENABLED,
		hwnd,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_Camera = new CameraClass;

	if (!m_Camera)
		return false;

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_Model = new ModelClass;

	if (!m_Model)
		return false;

	result = m_Model->Initialize(m_Direct3D->GetDevice());

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	m_ColorShader = new ColorShaderClass;

	if (!m_ColorShader)
		return false;

	result = m_ColorShader->Initialize(m_Direct3D->GetDevice(), hwnd);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

void GraphicsClass::Shutdown()
{
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		delete m_ColorShader;
		m_ColorShader = NULL;
	}

	if (m_Model)
	{
		m_Model->Shutdown();
		delete m_Model;
		m_Model = NULL;
	}

	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = NULL;
	}

	if (m_Direct3D)
	{
		m_Direct3D->Shutdown();
		delete m_Direct3D;
		m_Direct3D = NULL;
	}

	return;
}

bool GraphicsClass::Frame()
{
	bool result;

	result = Render();
	if (!result)
		return false;

	return true;
}

bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_Direct3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view and projection matrices from the camera and 3d3 object.
	m_Direct3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_Direct3D->GetProjectionMatrix(projectionMatrix);

	// Put the model verex and index buffer son the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_Direct3D->GetDeviceContext());

	// Render the model using them color shader.
	result = m_ColorShader->Render(m_Direct3D->GetDeviceContext(),
		m_Model->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix);

	if (!result)
		return false;

	// Present the renderd scene to the screen.
	m_Direct3D->EndScene();

	return true;
}
