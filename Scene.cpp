//-----------------------------------------------------------------------------
// File: Scene.cpp
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "Scene.h"
#include "resource.h"

CScene::CScene()
{
	m_ppShaders = NULL;
	m_nShaders = 0;

	m_pLights = NULL;
	m_pd3dcbLights = NULL;

	m_pCamera = NULL;
	m_pSelectedObject = NULL;
}

CScene::~CScene()
{
}

bool CScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
		case WM_LBUTTONDOWN:
			m_pSelectedObject = PickObjectPointedByCursor(LOWORD(lParam), HIWORD(lParam));
			if(m_pSelectedObject) m_pSelectedObject->SetActive(false);
			break;
		case WM_RBUTTONDOWN:
			break;
		case WM_MOUSEMOVE:
			break;
		case WM_LBUTTONUP:
			break;
		case WM_RBUTTONUP:
			break;
		default:
			break;
	}
	return(false);
}

bool CScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	return(false);
}

void CScene::BuildObjects(ID3D11Device *pd3dDevice)
{
	// 텍스쳐 맵핑에 사용할 샘플러 상태 객체를 생성한다.
	ID3D11SamplerState *pd3dSamplerState = NULL;
	D3D11_SAMPLER_DESC d3dSamplerDesc;
	ZeroMemory(&d3dSamplerDesc, sizeof(D3D11_SAMPLER_DESC));
	d3dSamplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	d3dSamplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	d3dSamplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	d3dSamplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	d3dSamplerDesc.MinLOD = 0;
	d3dSamplerDesc.MaxLOD = 0;
	pd3dDevice->CreateSamplerState(&d3dSamplerDesc, &pd3dSamplerState);

	// 텍스쳐 리소스를 생성한다.
	ID3D11ShaderResourceView *pd3dsrvTexture = NULL;

	HRESULT hr;

	int temp = 0;

	CTexture *pStoneTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../../Assets/Image/Miscellaneous/Stone01.jpg"), NULL, NULL, &pd3dsrvTexture, &hr);
	if (FAILED(hr))
	{
		AllocConsole();
		FILE *file;
		freopen_s(&file, "CONOUT$", "wt", stdout);
		printf("error");
	};

	pStoneTexture->SetTexture(0, pd3dsrvTexture);
	pStoneTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	CTexture *pBrickTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../../Assets/Image/Miscellaneous/Brick01.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pBrickTexture->SetTexture(0, pd3dsrvTexture);
	pBrickTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();

	CTexture *pWoodTexture = new CTexture(1, 1, 0, 0);
	D3DX11CreateShaderResourceViewFromFile(pd3dDevice, _T("../../Assets/Image/Miscellaneous/Wood01.jpg"), NULL, NULL, &pd3dsrvTexture, NULL);
	pWoodTexture->SetTexture(0, pd3dsrvTexture);
	pWoodTexture->SetSampler(0, pd3dSamplerState);
	pd3dsrvTexture->Release();
	pd3dSamplerState->Release();
	 
	m_nShaders = 1 + 1 + 1 + 3;
	m_ppShaders = new CShader*[m_nShaders];

	m_ppShaders[0] = new CSkyBoxShader();
	m_ppShaders[0]->CreateShader(pd3dDevice);
	m_ppShaders[0]->BuildObjects(pd3dDevice);

	m_ppShaders[1] = new CTerrainShader();
	m_ppShaders[1]->CreateShader(pd3dDevice);
	m_ppShaders[1]->BuildObjects(pd3dDevice);

	m_ppShaders[2] = new CWaterBoxShader();
	m_ppShaders[2]->CreateShader(pd3dDevice);
	m_ppShaders[2]->BuildObjects(pd3dDevice);

	// 재질을 생성한다.
	CMaterial *pRedMaterial = new CMaterial();
	pRedMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pRedMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	pRedMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 5.0f);
	pRedMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	CMaterial *pGreenMaterial = new CMaterial();
	pGreenMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pGreenMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f);
	pGreenMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 3.0f);
	pGreenMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f);

	CMaterial *pBlueMaterial = new CMaterial();
	pBlueMaterial->m_Material.m_d3dxcDiffuse = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueMaterial->m_Material.m_d3dxcAmbient = D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f);
	pBlueMaterial->m_Material.m_d3dxcSpecular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 4.0f);
	pBlueMaterial->m_Material.m_d3dxcEmissive = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);

	/* 구는 3가지 종류(텍스쳐에 따라)이다. 다른 텍스쳐의 구들이 번갈아 나열되도록 한다. 텍스쳐의 종류에 따라 k가 0, 1, 2의 값을 가지고
	k에 따라 객체의 위치를 다르게 설정한다.*/
	CInstancedObjectsShader *pStoneInstancedObjectsShader = new CInstancedObjectsShader();
	pStoneInstancedObjectsShader->CreateShader(pd3dDevice);
	pStoneInstancedObjectsShader->BuildObjects(pd3dDevice, GetTerrain(), pRedMaterial, pStoneTexture, 0);
	m_ppShaders[3] = pStoneInstancedObjectsShader;
//	m_ppShaders[2] = pStoneInstancedObjectsShader;

	CInstancedObjectsShader *pBrickInstancedObjectsShader = new CInstancedObjectsShader();
	pBrickInstancedObjectsShader->CreateShader(pd3dDevice);
	pBrickInstancedObjectsShader->BuildObjects(pd3dDevice, GetTerrain(), pGreenMaterial, pBrickTexture, 1);
	m_ppShaders[4] = pBrickInstancedObjectsShader;
//	m_ppShaders[3] = pBrickInstancedObjectsShader;

	CInstancedObjectsShader *pWoodInstancedObjectsShader = new CInstancedObjectsShader();
	pWoodInstancedObjectsShader->CreateShader(pd3dDevice);
	pWoodInstancedObjectsShader->BuildObjects(pd3dDevice, GetTerrain(), pBlueMaterial, pWoodTexture, 2);
	m_ppShaders[5] = pWoodInstancedObjectsShader;
//	m_ppShaders[4] = pWoodInstancedObjectsShader;
	
	CreateShaderVariables(pd3dDevice);
}

CHeightMapTerrain *CScene::GetTerrain() 
{ 
	CTerrainShader *pTerrainShader = (CTerrainShader *)m_ppShaders[1];
	return(pTerrainShader->GetTerrain()); 
}

void CScene::ReleaseObjects()
{
	ReleaseShaderVariables();

	for (int j = 0; j < m_nShaders; j++) 
	{
		if (m_ppShaders[j]) m_ppShaders[j]->ReleaseObjects();
		if (m_ppShaders[j]) delete m_ppShaders[j];
	}
	if (m_ppShaders) delete[] m_ppShaders;
}

void CScene::CreateShaderVariables(ID3D11Device *pd3dDevice)
{
	m_pLights = new LIGHTS;
	::ZeroMemory(m_pLights, sizeof(LIGHTS));
	m_pLights->m_d3dxcGlobalAmbient = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);

	m_pLights->m_pLights[0].m_bEnable = 1.0f;
	m_pLights->m_pLights[0].m_nType = POINT_LIGHT;
	m_pLights->m_pLights[0].m_fRange = 300.0f;
	m_pLights->m_pLights[0].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcDiffuse = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[0].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvPosition = D3DXVECTOR3(300.0f, 300.0f, 300.0f);
	m_pLights->m_pLights[0].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[0].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.001f, 0.0001f);

	m_pLights->m_pLights[1].m_bEnable = 1.0f;
	m_pLights->m_pLights[1].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[1].m_fRange = 100.0f;
	m_pLights->m_pLights[1].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcDiffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxcSpecular = D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	m_pLights->m_pLights[1].m_d3dxvPosition = D3DXVECTOR3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[1].m_d3dxvDirection = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[1].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.0001f);
	m_pLights->m_pLights[1].m_fFalloff = 8.0f;
	m_pLights->m_pLights[1].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[1].m_fTheta = (float)cos(D3DXToRadian(20.0f));
	
	m_pLights->m_pLights[2].m_bEnable = 1.0f;
	m_pLights->m_pLights[2].m_nType = DIRECTIONAL_LIGHT;
	m_pLights->m_pLights[2].m_d3dxcAmbient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcDiffuse = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f);
	//m_pLights->m_pLights[2].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);
	//m_pLights->m_pLights[2].m_d3dxcDiffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	m_pLights->m_pLights[2].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[2].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	
	m_pLights->m_pLights[3].m_bEnable = 1.0f;
	m_pLights->m_pLights[3].m_nType = SPOT_LIGHT;
	m_pLights->m_pLights[3].m_fRange = 60.0f;
	m_pLights->m_pLights[3].m_d3dxcAmbient = D3DXCOLOR(0.1f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcDiffuse = D3DXCOLOR(0.5f, 0.0f, 0.0f, 1.0f);
	m_pLights->m_pLights[3].m_d3dxcSpecular = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvPosition = D3DXVECTOR3(500.0f, 300.0f, 500.0f);
	m_pLights->m_pLights[3].m_d3dxvDirection = D3DXVECTOR3(0.0f, -1.0f, 0.0f);
	m_pLights->m_pLights[3].m_d3dxvAttenuation = D3DXVECTOR3(1.0f, 0.01f, 0.001f);
	m_pLights->m_pLights[3].m_fFalloff = 20.0f;
	m_pLights->m_pLights[3].m_fPhi = (float)cos(D3DXToRadian(40.0f));
	m_pLights->m_pLights[3].m_fTheta = (float)cos(D3DXToRadian(15.0f));

	D3D11_BUFFER_DESC d3dBufferDesc;
	ZeroMemory(&d3dBufferDesc, sizeof(d3dBufferDesc));
	d3dBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	d3dBufferDesc.ByteWidth = sizeof(LIGHTS);
	d3dBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	d3dBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    D3D11_SUBRESOURCE_DATA d3dBufferData;
	ZeroMemory(&d3dBufferData, sizeof(D3D11_SUBRESOURCE_DATA));
    d3dBufferData.pSysMem = m_pLights;
    pd3dDevice->CreateBuffer(&d3dBufferDesc, &d3dBufferData, &m_pd3dcbLights);
}

void CScene::UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, LIGHTS *pLights)
{
    D3D11_MAPPED_SUBRESOURCE d3dMappedResource;
    pd3dDeviceContext->Map(m_pd3dcbLights, 0, D3D11_MAP_WRITE_DISCARD, 0, &d3dMappedResource);
    LIGHTS *pcbLight = (LIGHTS *)d3dMappedResource.pData;
    memcpy(pcbLight, pLights, sizeof(LIGHTS));
    pd3dDeviceContext->Unmap(m_pd3dcbLights, 0);
    pd3dDeviceContext->PSSetConstantBuffers(PS_SLOT_LIGHT, 1, &m_pd3dcbLights);
}

void CScene::ReleaseShaderVariables()
{
	if (m_pLights) delete m_pLights;
	if (m_pd3dcbLights) m_pd3dcbLights->Release();
}

bool CScene::ProcessInput(UCHAR *pKeysBuffer)
{
	return(false);
}

void CScene::AnimateObjects(float fTimeElapsed)
{
	if (m_pLights && m_pd3dcbLights) 
	{
		D3DXVECTOR3 d3dxvCameraPosition = m_pCamera->GetPosition();
		m_pLights->m_d3dxvCameraPosition = D3DXVECTOR4(d3dxvCameraPosition, 1.0f);

		CHeightMapTerrain *pTerrain = GetTerrain();
		static D3DXVECTOR3 d3dxvRotated = D3DXVECTOR3(pTerrain->GetWidth()*0.3f, 0.0f, 0.0f);
		D3DXMATRIX d3dxmtxRotate;
		D3DXMatrixRotationYawPitchRoll(&d3dxmtxRotate, (float)D3DXToRadian(30.0f*fTimeElapsed), 0.0f, 0.0f);
		D3DXVec3TransformCoord(&d3dxvRotated, &d3dxvRotated, &d3dxmtxRotate);
		D3DXVECTOR3 d3dxvTerrainCenter = D3DXVECTOR3(pTerrain->GetWidth()*0.5f, pTerrain->GetPeakHeight() + 10.0f, pTerrain->GetLength()*0.5f);
		m_pLights->m_pLights[0].m_d3dxvPosition = d3dxvTerrainCenter + d3dxvRotated;
		m_pLights->m_pLights[0].m_fRange = pTerrain->GetPeakHeight();

		CPlayer *pPlayer = m_pCamera->GetPlayer();
		m_pLights->m_pLights[1].m_d3dxvPosition = pPlayer->GetPosition();
		m_pLights->m_pLights[1].m_d3dxvDirection = pPlayer->GetLookVector();

		m_pLights->m_pLights[3].m_d3dxvPosition = pPlayer->GetPosition() + D3DXVECTOR3(0.0f, 40.0f, 0.0f);
	}

	for (int i = 0; i < m_nShaders; i++)
	{
		m_ppShaders[i]->AnimateObjects(fTimeElapsed);
	}
}

void CScene::Render(ID3D11DeviceContext	*pd3dDeviceContext, CCamera *pCamera)
{
	if (m_pLights && m_pd3dcbLights) 
		UpdateShaderVariable(pd3dDeviceContext, m_pLights);

	for (int i = 0; i < m_nShaders; i++)
	{
		m_ppShaders[i]->Render(pd3dDeviceContext, pCamera);

	}
}

CGameObject *CScene::PickObjectPointedByCursor(int xClient, int yClient)
{
	if (!m_pCamera) return(NULL);

	D3DXMATRIX d3dxmtxView = m_pCamera->GetViewMatrix();
	D3DXMATRIX d3dxmtxProjection = m_pCamera->GetProjectionMatrix();
	D3D11_VIEWPORT d3dViewport = m_pCamera->GetViewport();

	D3DXVECTOR3 d3dxvPickPosition;
	d3dxvPickPosition.x = (((2.0f * xClient) / d3dViewport.Width) - 1) / d3dxmtxProjection._11;
	d3dxvPickPosition.y = -(((2.0f * yClient) / d3dViewport.Height) - 1) / d3dxmtxProjection._22;
	d3dxvPickPosition.z = 1.0f;

	int nIntersected = 0;
	float fNearHitDistance = FLT_MAX;
	MESHINTERSECTINFO d3dxIntersectInfo;
	CGameObject *pIntersectedObject = NULL, *pNearestObject = NULL;
	for (int i = 0; i < m_nShaders; i++)
	{
		pIntersectedObject = m_ppShaders[i]->PickObjectByRayIntersection(&d3dxvPickPosition, &d3dxmtxView, &d3dxIntersectInfo);
		if (pIntersectedObject && (d3dxIntersectInfo.m_fDistance < fNearHitDistance))
		{
			fNearHitDistance = d3dxIntersectInfo.m_fDistance;
			pNearestObject = pIntersectedObject;
		}
	}
	return(pNearestObject);
}
