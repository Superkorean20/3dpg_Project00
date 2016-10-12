//-----------------------------------------------------------------------------
// File: Shader.h
//-----------------------------------------------------------------------------

#pragma once

#include "Object.h"
#include "Camera.h"
#include "Player.h"

#define _WITH_INSTANCING_CULLING
#ifdef _WITH_INSTANCING_CULLING
#define _WITH_INSTANCING_FROM_OBJECTS_DYNAMICALLY
#endif

struct VS_CB_WORLD_MATRIX
{
	D3DXMATRIX						m_d3dxmtxWorld;
};

struct VS_VB_INSTANCE
{
	D3DXMATRIX						m_d3dxTransform;
	D3DXCOLOR						m_d3dxColor;
};

class CShader
{
public:
	CShader();
	virtual ~CShader();

	void CreateVertexShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11VertexShader **ppd3dVertexShader, D3D11_INPUT_ELEMENT_DESC *pd3dInputElements, UINT nElements, ID3D11InputLayout **ppd3dInputLayout);
	void CreatePixelShaderFromFile(ID3D11Device *pd3dDevice, WCHAR *pszFileName, LPCSTR pszShaderName, LPCSTR pszShaderModel, ID3D11PixelShader **ppd3dPixelShader);

	virtual void CreateShader(ID3D11Device *pd3dDevice);
	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void ReleaseObjects();
	virtual void AnimateObjects(float fTimeElapsed);
	virtual void OnPrepareRender(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	virtual CGameObject *PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

	static void CreateShaderVariables(ID3D11Device *pd3dDevice);
	static void ReleaseShaderVariables();
	static void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, D3DXMATRIX *pd3dxmtxWorld);

protected:
	ID3D11VertexShader				*m_pd3dVertexShader;
	ID3D11InputLayout				*m_pd3dVertexLayout;

	ID3D11PixelShader				*m_pd3dPixelShader;

	CGameObject						**m_ppObjects;
	int								m_nObjects;

	static ID3D11Buffer				*m_pd3dcbWorldMatrix;
};

class CDiffusedShader : public CShader
{
public:
    CDiffusedShader();
    virtual ~CDiffusedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};

class CIlluminatedShader : public CShader
{
public:
    CIlluminatedShader();
    virtual ~CIlluminatedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

	static ID3D11Buffer				*m_pd3dcbMaterial;

	static void CreateShaderVariables(ID3D11Device *pd3dDevice);
	static void ReleaseShaderVariables();
	static void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, MATERIAL *pMaterial);
};

class CObjectsShader : public CIlluminatedShader
{
public:
	CObjectsShader();
	virtual ~CObjectsShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice, CHeightMapTerrain *pHeightMapTerrain);
};

//class CPlayerShader : public CDiffusedShader
class CPlayerShader : public CIlluminatedShader
{
public:
	CPlayerShader();
	virtual ~CPlayerShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	CPlayer *GetPlayer(int nIndex = 0) { return((CPlayer *)m_ppObjects[nIndex]); }
};

class CTexturedIlluminatedShader : public CIlluminatedShader
{
public: 
	CTexturedIlluminatedShader();
	virtual ~CTexturedIlluminatedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};

class CDetailTexturedIlluminatedShader : public CTexturedIlluminatedShader
{
public:
	CDetailTexturedIlluminatedShader();
	virtual ~CDetailTexturedIlluminatedShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);
};

///////////////////////////////////////////

class CTexturedShader : public CShader
{
public:
	CTexturedShader();
	virtual ~CTexturedShader();

	virtual void CreateShader(ID3D11Device* pd3dDevice);
};

class CDetailTexturedShader: public CTexturedShader
{
public:
	CDetailTexturedShader();
	virtual ~CDetailTexturedShader();

	virtual void CreateShader(ID3D11Device* pd3dDevice);
};

class CTerrainShader : public CDetailTexturedIlluminatedShader
{
public:
	CTerrainShader();
	virtual ~CTerrainShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice);

	CHeightMapTerrain *GetTerrain();
};

class CWaterBoxShader : public CTexturedIlluminatedShader
{
public:
	CWaterBoxShader();
	virtual ~CWaterBoxShader();

	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera *pCamera);
	virtual void BuildObjects(ID3D11Device *pd3dDevice);
};

////////////////////////////////////////////////////
class CInstancedObjectsShader : public CTexturedShader
{
public:
	CInstancedObjectsShader();
	virtual ~CInstancedObjectsShader();

	virtual void CreateShader(ID3D11Device *pd3dDevice);

	virtual void BuildObjects(ID3D11Device *pd3dDevice, CHeightMapTerrain *pHeightMapTerrain, CMaterial *pMaterial, CTexture *pTexture, int k);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

private:
	UINT							m_nInstanceBufferStride;
	UINT							m_nInstanceBufferOffset;

	ID3D11Buffer					*m_pd3dCubeInstanceBuffer;
	ID3D11Buffer					*m_pd3dSphereInstanceBuffer;

	CMaterial						*m_pMaterial;
	CTexture						*m_pTexture;

public:
	ID3D11Buffer *CreateInstanceBuffer(ID3D11Device *pd3dDevice, int nObjects, UINT nBufferStride, void *pBufferData);
};

class CSkyBoxShader : public CTexturedShader
{
public:
	CSkyBoxShader();
	virtual ~CSkyBoxShader();

	virtual void BuildObjects(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext* pd3dDeviceContext, CCamera *pCamera);
};
