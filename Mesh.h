//------------------------------------------------------- ----------------------
// File: Mesh.h
//-----------------------------------------------------------------------------

#pragma once

#define VERTEX_POSITION_ELEMENT			0x01
#define VERTEX_COLOR_ELEMENT			0x02
#define VERTEX_NORMAL_ELEMENT			0x04

struct MESHINTERSECTINFO {
	DWORD	m_dwFaceIndex;
	float	m_fU;
	float	m_fV;
	float	m_fDistance;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
#define EPSILON				1.0e-10f

inline bool IsZero(float fValue) { return((fabsf(fValue) < EPSILON)); }
inline bool IsEqual(float fA, float fB) { return(::IsZero(fA - fB)); }
inline float InverseSqrt(float fValue) { return 1.0f / sqrtf(fValue); }
inline void Swap(float *pfS, float *pfT) { float fTemp = *pfS; *pfS = *pfT; *pfT = fTemp; }

extern bool RayIntersectTriangle(D3DXVECTOR3 *pd3dxvOrigin, D3DXVECTOR3 *pd3dxvDirection, D3DXVECTOR3 *pd3dxvP0, D3DXVECTOR3 *pd3dxvP1, D3DXVECTOR3 *pd3dxvP2, float *pfU, float *pfV, float *pfRayToTriangle);

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class AABB
{
public:
	D3DXVECTOR3						m_d3dxvMinimum;
	D3DXVECTOR3						m_d3dxvMaximum;

public:
	AABB() { m_d3dxvMinimum = D3DXVECTOR3(+FLT_MAX, +FLT_MAX, +FLT_MAX); m_d3dxvMaximum = D3DXVECTOR3(-FLT_MAX, -FLT_MAX, -FLT_MAX); };
	AABB(D3DXVECTOR3 d3dxvMinimum, D3DXVECTOR3 d3dxvMaximum) { m_d3dxvMinimum = d3dxvMinimum; m_d3dxvMaximum = d3dxvMaximum; }

	void Merge(D3DXVECTOR3& d3dxvMinimum, D3DXVECTOR3& d3dxvMaximum);
	void Merge(AABB *pAABB);
	void Update(D3DXMATRIX *pd3dxmtxTransform);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMesh
{
public:
	CMesh(ID3D11Device *pd3dDevice);
	virtual ~CMesh();

private:
	int								m_nReferences;

public:
	void AddRef() { m_nReferences++; }
	void Release() { if (--m_nReferences <= 0) delete this; }

protected:
	AABB							m_bcBoundingCube;

protected:
	D3D11_PRIMITIVE_TOPOLOGY		m_d3dPrimitiveTopology;

	UINT							m_nType;

	ID3D11Buffer					*m_pd3dPositionBuffer;
	ID3D11Buffer					**m_ppd3dVertexBuffers;
	ID3D11Buffer					*m_pd3dIndexBuffer;

	int								m_nVertices;
	int								m_nBuffers;
	UINT							m_nSlot;
	UINT							m_nStartVertex;
	
	UINT							*m_pnVertexStrides;
	UINT							*m_pnVertexOffsets;
	
	UINT							m_nIndices;
	UINT							m_nStartIndex;
	int								m_nBaseVertex;
	DXGI_FORMAT						m_dxgiIndexFormat;
	UINT							m_nIndexOffset;

	ID3D11RasterizerState			*m_pd3dRasterizerState;

	D3DXVECTOR3						*m_pd3dxvPositions;
	UINT							*m_pnIndices;

public:
	AABB GetBoundingCube() { return(m_bcBoundingCube); }

	void AssembleToVertexBuffer(int nBuffers = 0, ID3D11Buffer **m_pd3dBuffers = NULL, UINT *pnBufferStrides = NULL, UINT *pnBufferOffsets = NULL);

	virtual void CreateRasterizerState(ID3D11Device *pd3dDevice);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
	virtual void RenderInstanced(ID3D11DeviceContext *pd3dDeviceContext, int nInstances = 0, int nStartInstance = 0);

	int CheckRayIntersection(D3DXVECTOR3 *pd3dxvRayPosition, D3DXVECTOR3 *pd3dxvRayDirection, MESHINTERSECTINFO *pd3dxIntersectInfo);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshDiffused : public CMesh
{
public:
    CMeshDiffused(ID3D11Device *pd3dDevice);
    virtual ~CMeshDiffused();

protected:
	ID3D11Buffer					*m_pd3dColorBuffer;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CAirplaneMeshDiffused : public CMeshDiffused
{
public:
	CAirplaneMeshDiffused(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 4.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CAirplaneMeshDiffused();
};

class CCubeMeshDiffused : public CMeshDiffused
{
public:
	CCubeMeshDiffused(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CCubeMeshDiffused();
};

class CSphereMeshDiffused : public CMeshDiffused
{
public:
	CSphereMeshDiffused(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20, D3DXCOLOR d3dxColor = D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	virtual ~CSphereMeshDiffused();
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshIlluminated : public CMesh
{
public:
    CMeshIlluminated(ID3D11Device *pd3dDevice);
    virtual ~CMeshIlluminated();

protected:
	ID3D11Buffer					*m_pd3dNormalBuffer;

public:
	D3DXVECTOR3 CalculateTriAngleNormal(UINT nIndex0, UINT nIndex1, UINT nIndex2);
	void SetTriAngleListVertexNormal(D3DXVECTOR3 *pd3dxvNormals);
	void SetAverageVertexNormal(D3DXVECTOR3 *pd3dxvNormals, int nPrimitives, int nOffset, bool bStrip);
	void CalculateVertexNormal(D3DXVECTOR3 *pd3dxvNormals);
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CCubeMeshIlluminated : public CMeshIlluminated
{
public:
    CCubeMeshIlluminated(ID3D11Device *pd3dDevice, float fWidth=2.0f, float fHeight=2.0f, float fDepth=2.0f);
    virtual ~CCubeMeshIlluminated();
};

class CSphereMeshIlluminated : public CMeshIlluminated
{
public:
	CSphereMeshIlluminated(ID3D11Device *pd3dDevice, float fRadius=2.0f, int nSlices=20, int nStacks=20);	
	virtual ~CSphereMeshIlluminated();
};

class CMeshTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshTexturedIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshTexturedIlluminated();

protected:
	ID3D11Buffer *m_pd3dTexCoordBuffer;
};

class CMeshDetailTexturedIlluminated : public CMeshIlluminated
{
public:
	CMeshDetailTexturedIlluminated(ID3D11Device *pd3dDevice);
	virtual ~CMeshDetailTexturedIlluminated();

protected:
	ID3D11Buffer *m_pd3dTexCoordBuffer;
	ID3D11Buffer *m_pd3dDetailTexCoordBuffer;

};
////////////////////////////////////////////////////////////////////////////////////////////////
//

class CCubeMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CCubeMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTexturedIlluminated();
};

class CSphereMeshTexturedIlluminated : public CMeshTexturedIlluminated
{
public:
	CSphereMeshTexturedIlluminated(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTexturedIlluminated();
};


/////////////////////////////////////////////////////////////////////////////////////////////////
//
class CMeshTextured : public CMesh
{
public:
	CMeshTextured(ID3D11Device *pd3dDevice);
	virtual ~CMeshTextured();

protected:
	// 텍스쳐 매핑을 하기 위하여 텍스쳐 좌표가 필요하다.
	ID3D11Buffer *m_pd3dTexCoordBuffer;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
//

// 텍스쳐 매핑을 사용하여 색상을 결정하기 위하여 정점이 텍스쳐 좌표를 가지는 직육면체 메쉬 클래이다.
class CCubeMeshTextured : public CMeshTextured
{
public:
	CCubeMeshTextured(ID3D11Device *pd3dDevice, float fWidth = 2.0f, float fHeight = 2.0f, float fDepth = 2.0f);
	virtual ~CCubeMeshTextured();
};

/* 텍스쳐 매핑을 사용하여 색상을 결정하기 위하여 정점이 텍스쳐 좌표를 가지는 구 메쉬 클래스이다.*/
class CSphereMeshTextured : public CMeshTextured
{
public:
	CSphereMeshTextured(ID3D11Device *pd3dDevice, float fRadius = 2.0f, int nSlices = 20, int nStacks = 20);
	virtual ~CSphereMeshTextured();

};

class CMeshDetailTextured : public CMeshTextured
{
public:
	CMeshDetailTextured(ID3D11Device *pd3dDevice);
	virtual ~CMeshDetailTextured();

protected:
	ID3D11Buffer *m_pd3dDetailTexCoordBuffer;
};

class CTexture;

class CSkyBoxMesh : public CMeshTextured
{
protected:
	ID3D11DepthStencilState *m_pd3dDepthStencilState;

	CTexture *m_pSkyboxTexture;

public:
	CSkyBoxMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f);
	virtual ~CSkyBoxMesh();

	void OnChangeSkyBoxTextures(ID3D11Device* pd3dDevice, int nIndex = 0);
	virtual void Render(ID3D11DeviceContext *pd3dDeviceContext);
}; 

class CWaterBoxMesh : public CMeshTexturedIlluminated
{
protected:
	int							m_nWidth;
	int							m_nLength;

public:
	CWaterBoxMesh(ID3D11Device *pd3dDevice, float fWidth = 20.0f, float fHeight = 20.0f, float fDepth = 20.0f, float fScale = 5.0f);
	virtual ~CWaterBoxMesh();
};

/* CHeightMapGridMesh 클래스의 베이스 클래스를 CMeshDetailTextured로 변경한다.
지형 메쉬의 각 정점은 두 개의 텍스쳐 좌표를 갖는다.
지형의 색상을 텍스쳐 매핑을 사용하여 결정하기 위하여 지형 메쉬의 각 정점은 텍스쳐 좌표를 갖는다. */
class CHeightMapGridMesh : public CMeshDetailTexturedIlluminated
{
protected:
	int							m_nWidth;
	int							m_nLength;
	D3DXVECTOR3					m_d3dxvScale;

public:
	CHeightMapGridMesh(ID3D11Device *pd3dDevice, int xStart, int zStart, int nWidth, int nLength, D3DXVECTOR3 d3dxvScale = D3DXVECTOR3(1.0f, 1.0f, 1.0f), void *pContext = NULL);
	virtual ~CHeightMapGridMesh();

	D3DXVECTOR3 GetScale() { return(m_d3dxvScale); }
	int GetWidth() { return(m_nWidth); }
	int GetLength() { return(m_nLength); }

	virtual float OnGetHeight(int x, int z, void *pContext);
};

