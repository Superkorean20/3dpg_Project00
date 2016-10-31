//------------------------------------------------------- ----------------------
// File: Object.h
//-----------------------------------------------------------------------------

#pragma once

#include "Mesh.h"
#include "Camera.h"

#define DIR_FORWARD				0x01
#define DIR_BACKWARD			0x02
#define DIR_LEFT				0x04
#define DIR_RIGHT				0x08
#define DIR_UP					0x10
#define DIR_DOWN				0x20

struct MATERIAL
{
	D3DXCOLOR					m_d3dxcAmbient;
	D3DXCOLOR					m_d3dxcDiffuse;
	D3DXCOLOR					m_d3dxcSpecular; //(r,g,b,a=power)
	D3DXCOLOR					m_d3dxcEmissive; // power. 
};
	
class CMaterial
{
public:
	CMaterial();
    virtual ~CMaterial();

private:
	int							m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	MATERIAL					m_Material;
};

// 게임 객체응 하나 이상의 텍스쳐를 가질 수 있다. CTexture는 텍스쳐를 관히하기 위한 클래스이다.
class CTexture
{
public:
	CTexture(int nTextures = 1, int nSampler = 1, int nTextureStartSlot = 0, int nSamplerStartSlot = 0, int nAlphaBlendStartSlot = 0);
	virtual ~CTexture();

private:
	int m_nReference;

public:
	void AddRef(){ m_nReference++; }
	void Release() { if (--m_nReference <= 0) delete this; }

private:
	int m_nTextures;
	int m_nSamplers;
	int m_nAlphaBlends;

	int m_nTextureStartSlot;
	int m_nSamplerStartSlot;
	int m_nAlphaBlendStartSlot;

	ID3D11ShaderResourceView **m_ppd3dsrvTextures;
	ID3D11SamplerState **m_ppd3dSamplerStates;
	
public:
	void SetTexture(int nIndex, ID3D11ShaderResourceView *pd3dsrvTexture);
	void SetSampler(int nIndex, ID3D11SamplerState *pd3dSamplerState);

	void UpdateShaderVariable(ID3D11DeviceContext *pd3dDeviceContext);

	void UpdateTextureShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex = 0, int nSlot = 0);

	void UpdateSamplerShaderVariable(ID3D11DeviceContext *pd3dDeviceContext, int nIndex, int nSlot = 0);
};

class CGameObject
{
public:
	CGameObject(int nMeshes=0);
	virtual ~CGameObject();

private:
	int								m_nReferences;

	bool							m_bActive;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

	D3DXMATRIX						m_d3dxmtxWorld;

	CMesh							**m_ppMeshes;
	CMaterial						*m_pMaterial;
	CTexture						*m_pTexture;
	int								m_nMeshes;

	AABB							m_bcMeshBoundingCube;

	void SetTexture(CTexture *pTexture);
	void SetMesh(CMesh *pMesh, int nIndex=0);
	void SetMaterial(CMaterial *pMaterial);
	CMesh *GetMesh(int nIndex = 0) { return(m_ppMeshes[nIndex]); }

	D3DXMATRIX GetTransform() { return(m_d3dxmtxWorld); }
	void GetTransformToMappedResource(D3DXMATRIX *pd3dxmtxBuffer);

	void SetPosition(float x, float y, float z);
	void SetPosition(D3DXVECTOR3 d3dxvPosition);

	void MoveStrafe(float fDistance = 1.0f);
	void MoveUp(float fDistance = 1.0f);
	void MoveForward(float fDistance = 1.0f);

	void Rotate(float fPitch = 10.0f, float fYaw = 10.0f, float fRoll = 10.0f);
	void Rotate(D3DXVECTOR3 *pd3dxvAxis, float fAngle);

	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetLook();
	D3DXVECTOR3 GetUp();
	D3DXVECTOR3 GetRight();

	bool IsVisible(CCamera *pCamera = NULL);

	virtual void Animate(float fTimeElapsed);
	virtual void OnPrepareRender() { }
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);

	void SetActive(bool bActive = false) { m_bActive = bActive; }
	void GenerateRayForPicking(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxWorld, D3DXMATRIX *pd3dxmtxView,
		D3DXVECTOR3 *pd3dxvPickRayPosition, D3DXVECTOR3 *pd3dxvPickRayDirection);
	int PickObjectByRayIntersection(D3DXVECTOR3 *pd3dxvPickPosition, D3DXMATRIX *pd3dxmtxView, MESHINTERSECTINFO *pd3dxIntersectInfo);

};

class CRotatingObject : public CGameObject
{
public:
	CRotatingObject(int nMeshes=1);
	virtual ~CRotatingObject();

private:
	D3DXVECTOR3					m_d3dxvRotationAxis;
	float						m_fRotationSpeed;

public:
	void SetRotationSpeed(float fRotationSpeed) { m_fRotationSpeed = fRotationSpeed; }
	void SetRotationAxis(D3DXVECTOR3 d3dxvRotationAxis) { m_d3dxvRotationAxis = d3dxvRotationAxis; }

	virtual void Animate(float fTimeElapsed);
};

class CRevolvingObject : public CGameObject
{
public:
	CRevolvingObject(int nMeshes=1);
	virtual ~CRevolvingObject();

private:
	D3DXVECTOR3					m_d3dxvRevolutionAxis;
	float						m_fRevolutionSpeed;

public:
	void SetRevolutionSpeed(float fRevolutionSpeed) { m_fRevolutionSpeed = fRevolutionSpeed; }
	void SetRevolutionAxis(D3DXVECTOR3 d3dxvRevolutionAxis) { m_d3dxvRevolutionAxis = d3dxvRevolutionAxis; }

	virtual void Animate(float fTimeElapsed);
};

class CHeightMap
{
private:
	BYTE						*m_pHeightMapImage;
	int							m_nWidth;
	int							m_nLength;
	D3DXVECTOR3					m_d3dxvScale;

public:
	CHeightMap(LPCTSTR pFileName, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale);
	~CHeightMap(void);

	float GetHeight(float x, float z, bool bReverseQuad = false);
	D3DXVECTOR3 GetHeightMapNormal(int x, int z);
	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }

	BYTE *GetHeightMapImage() { return(m_pHeightMapImage); }
	int GetHeightMapWidth() { return(m_nWidth); }
	int GetHeightMapLength() { return(m_nLength); }
};

class CHeightMapTerrain : public CGameObject
{
public:
	CHeightMapTerrain(ID3D11Device *pd3dDevice, LPCTSTR pFileName, int nWidth, int nLength, int nBlockWidth, int nBlockLength, D3DXVECTOR3 d3dxvScale);
	virtual ~CHeightMapTerrain();

private:
	CHeightMap					*m_pHeightMap;

	int							m_nWidth;
	int							m_nLength;

	D3DXVECTOR3					m_d3dxvScale;

public:
	float GetHeight(float x, float z, bool bReverseQuad = false) { return(m_pHeightMap->GetHeight(x, z, bReverseQuad) * m_d3dxvScale.y); } //World
	D3DXVECTOR3 GetNormal(float x, float z) { return(m_pHeightMap->GetHeightMapNormal(int(x / m_d3dxvScale.x), int(z / m_d3dxvScale.z))); }

	int GetHeightMapWidth() { return(m_pHeightMap->GetHeightMapWidth()); }
	int GetHeightMapLength() { return(m_pHeightMap->GetHeightMapLength()); }

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	float GetWidth() { return(m_nWidth * m_d3dxvScale.x); }
	float GetLength() { return(m_nLength * m_d3dxvScale.z); }

	float GetPeakHeight() { return(m_bcMeshBoundingCube.m_d3dxvMaximum.y); }
};

class CSkyBox : public CGameObject
{

public:

	CSkyBox(ID3D11Device *pd3dDevice);
	virtual ~CSkyBox();

	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext, CCamera *pCamera);
};

class CWaterBox : public CGameObject
{
public:
	CWaterBox(ID3D11Device *pd3dDevice);
	virtual ~CWaterBox();
};


class CMirrorBox : public CGameObject
{
public:
	CMirrorBox(ID3D11Device *pd3dDevice);
	virtual ~CMirrorBox();
};
