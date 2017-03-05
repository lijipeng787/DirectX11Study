#include "Graphics.h"

#include <new>

#include "../CommonFramework/TypeDefine.h"
#include "../CommonFramework/DirectX11Device.h"
#include "../CommonFramework/Input.h"
#include "../CommonFramework/Camera.h"

#include "modelclass.h"
#include "alphamapshaderclass.h"

GraphicsClass::GraphicsClass() {}

GraphicsClass::~GraphicsClass() {}

bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;
	XMMATRIX baseViewMatrix;

	{
		m_D3D = new DirectX11Device;
		if (!m_D3D) {
			return false;
		}
		result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
		if (!result) {
			MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
			return false;
		}
	}

	{
		m_Camera = (Camera*)_aligned_malloc(sizeof(Camera), 16);
		new (m_Camera)Camera();
		if (!m_Camera) {
			return false;
		}
		m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
		m_Camera->Render();
		m_Camera->GetViewMatrix(baseViewMatrix);
	}

	{
		m_Model = new ModelClass();
		if (!m_Model) {
			return false;
		}

		result = m_Model->Initialize(
			m_D3D->GetDevice(),
			"../../tut19/data/square.txt",
			L"../../tut19/data/stone01.dds",
			L"../../tut19/data/dirt01.dds",
			L"../../tut19/data/alpha01.dds"
		);
		if (!result) {
			MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
			return false;
		}
	}

	{
		m_AlphaMapShader = (AlphaMapShaderClass*)_aligned_malloc(sizeof(AlphaMapShaderClass), 16);
		new (m_AlphaMapShader)AlphaMapShaderClass();
		if (!m_AlphaMapShader) {
			return false;
		}

		result = m_AlphaMapShader->Initialize(m_D3D->GetDevice(), hwnd);
		if (!result) {
			MessageBox(hwnd, L"Could not initialize the alpha map shader object.", L"Error", MB_OK);
			return false;
		}
	}

	return true;
}

void GraphicsClass::Shutdown() {

	if (m_AlphaMapShader) {
		m_AlphaMapShader->Shutdown();
		m_AlphaMapShader->~AlphaMapShaderClass();
		_aligned_free(m_AlphaMapShader);
		m_AlphaMapShader = 0;
	}

	if (m_Model) {
		m_Model->Shutdown();
		delete m_Model;
		m_Model = 0;
	}

	if (m_Camera) {
		m_Camera->~Camera();
		_aligned_free(m_Camera);
		m_Camera = 0;
	}

	if (m_D3D) {
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}
}


bool GraphicsClass::Frame() {

	bool result;

	result = Render();
	if (!result) {
		return false;
	}

	return true;
}


bool GraphicsClass::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;

	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	m_Camera->Render();

	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);

	m_Model->Render(m_D3D->GetDeviceContext());

	m_AlphaMapShader->Render(m_D3D->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
							 m_Model->GetTextureArray());

	m_D3D->EndScene();

	return true;
}