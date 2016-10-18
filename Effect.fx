//--------------------------------------------------------------------------------------
// File: LabProject08.fx
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer cbViewMatrix : register(b0)
{
	matrix		gmtxView : packoffset(c0);
	matrix		gmtxProjection : packoffset(c4);
};

cbuffer cbWorldMatrix : register(b1)
{
	matrix		gmtxWorld : packoffset(c0);
};

#include "Light.fx"

//--------------------------------------------------------------------------------------
struct VS_DIFFUSED_COLOR_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR0;
};

struct VS_DIFFUSED_COLOR_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

struct VS_INSTANCED_DIFFUSED_COLOR_INPUT
{
    float3 position : POSITION;
    float4 color : COLOR0;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_DIFFUSED_COLOR_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
};

VS_DIFFUSED_COLOR_OUTPUT VSDiffusedColor(VS_DIFFUSED_COLOR_INPUT input)
{
    VS_DIFFUSED_COLOR_OUTPUT output = (VS_DIFFUSED_COLOR_OUTPUT)0;
    output.position = mul(float4(input.position, 1.0f), mul(mul(gmtxWorld, gmtxView), gmtxProjection));
    output.color = input.color;

    return(output);
}

float4 PSDiffusedColor(VS_DIFFUSED_COLOR_OUTPUT input) : SV_Target
{
    return(input.color);
}

VS_INSTANCED_DIFFUSED_COLOR_OUTPUT VSInstancedDiffusedColor(VS_INSTANCED_DIFFUSED_COLOR_INPUT input)
{
    VS_INSTANCED_DIFFUSED_COLOR_OUTPUT output = (VS_INSTANCED_DIFFUSED_COLOR_OUTPUT)0;
    output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
	output.color = input.color;
    return(output);
}

float4 PSInstancedDiffusedColor(VS_INSTANCED_DIFFUSED_COLOR_OUTPUT input) : SV_Target
{
    return(input.color);
}

//--------------------------------------------------------------------------------------
struct VS_LIGHTING_COLOR_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_LIGHTING_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
};

struct VS_INSTANCED_LIGHTING_COLOR_INPUT
{
    float3 position : POSITION;
	float3 normal : NORMAL;
    float4x4 mtxTransform : INSTANCEPOS;
};

struct VS_INSTANCED_LIGHTING_COLOR_OUTPUT
{
    float4 position : SV_POSITION;
    float3 positionW : POSITION;
    float3 normalW : NORMAL;
};

VS_LIGHTING_COLOR_OUTPUT VSLightingColor(VS_LIGHTING_COLOR_INPUT input)
{
    VS_LIGHTING_COLOR_OUTPUT output = (VS_LIGHTING_COLOR_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
    output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSLightingColor(VS_LIGHTING_COLOR_OUTPUT input) : SV_Target
{ 
    input.normalW = normalize(input.normalW); 
	float4 cIllumination = Lighting(input.positionW, input.normalW);

    return(cIllumination);
}

VS_INSTANCED_LIGHTING_COLOR_OUTPUT VSInstancedLightingColor(VS_INSTANCED_LIGHTING_COLOR_INPUT input)
{
    VS_INSTANCED_LIGHTING_COLOR_OUTPUT output = (VS_INSTANCED_LIGHTING_COLOR_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)input.mtxTransform);
    output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
    output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	return(output);
}

float4 PSInstancedLightingColor(VS_INSTANCED_LIGHTING_COLOR_OUTPUT input) : SV_Target
{
    input.normalW = normalize(input.normalW); 
	float4 cIllumination = Lighting(input.positionW, input.normalW);

    return(cIllumination);
}

//---------------------------------------------------------------------------------------
//

// 텍스쳐를 사용하는 경우 정점 쉐이더의 입력을 위한 구조체이다.
struct VS_TEXTURED_COLOR_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;
};

// 텍스쳐를 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_TEXTURED_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

// 인스턴싱을 하면서 텍스쳐를 사용하는 경우 정점 쉐이더의 입력을 위한 구조체이다.
struct VS_INSTANCED_TEXTURED_COLOR_INPUT
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD0;

	float4x4 mtxTransform : INSTANCEPOS;
};

// 인스턴싱을 하면서 텍스쳐를 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_INSTANCED_TEXTURED_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD0;
};

// 텍스쳐 매핑을 위한 쉐이더 변수
Texture2D gtxtTexture : register(t0);
SamplerState gSamplerState : register(s0);

VS_TEXTURED_COLOR_OUTPUT VSTexturedColor(VS_TEXTURED_COLOR_INPUT input)
{
	VS_TEXTURED_COLOR_OUTPUT output = (VS_TEXTURED_COLOR_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

//각 픽셀에 대하여 텍스쳐 샘플링을 하기 위한 픽셀 쉐이더 함수이다.
float4 PSTexturedColor(VS_TEXTURED_COLOR_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.texCoord);

	return(cColor);
}


// 인스턴싱과 텍스쳐 매핑을 동시에 처리하는 정점 쉐이더 함수와 픽셀 쉐이더 함수를 추가한다.
VS_INSTANCED_TEXTURED_COLOR_OUTPUT VSInstancedTexturedColor(VS_INSTANCED_TEXTURED_COLOR_INPUT input)
{
	VS_INSTANCED_TEXTURED_COLOR_OUTPUT output = (VS_INSTANCED_TEXTURED_COLOR_OUTPUT)0;
	output.position = mul(mul(mul(float4(input.position, 1.0f), input.mtxTransform), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

float4 PSInstancedTexturedColor(VS_INSTANCED_TEXTURED_COLOR_OUTPUT input) : SV_TARGET
{
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.texCoord);

	return (cColor);
}

//////////////////////////////////////////////////////////////////////
//

// 디테일 텍스쳐를 사용하는 경우 정점 쉐이더의 입력과 출력을 위한 구조체.
struct VS_DETAIL_TEXTURED_COLOR_INPUT
{
	float3 position : POSITION;
	float2 texCoordBase :	TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

struct VS_DETAIL_TEXTURED_COLOR_OUTPUT
{
	float4 position :		SV_POSITION;
	float2 texCoordBase :	TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

Texture2D gtxtDetailTexture : register(t1);
SamplerState gDetailSamplerState : register(s1);

VS_DETAIL_TEXTURED_COLOR_OUTPUT VSDetailTexturedColor(VS_DETAIL_TEXTURED_COLOR_INPUT input)
{
	VS_DETAIL_TEXTURED_COLOR_OUTPUT output = (VS_DETAIL_TEXTURED_COLOR_OUTPUT)0;
	output.position			= mul(mul(mul(float4(input.position, 1.0f), gmtxWorld), gmtxView), gmtxProjection);
	output.texCoordBase		= input.texCoordBase;
	output.texCoordDetail	= input.texCoordDetail;

	return(output);
}

float4 PSDetailTexturedColor(VS_DETAIL_TEXTURED_COLOR_OUTPUT input) : SV_Target

{
	float4 cBaseTexColor	 = gtxtTexture.Sample(gSamplerState, input.texCoordBase);
	float4 cDetailTexColor	 = gtxtDetailTexture.Sample(gDetailSamplerState, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor*0.5f) + (cDetailTexColor * 0.5f));

	return (cColor);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//디테일 텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 입력을 위한 구조체이다.
struct VS_DETAIL_TEXTURED_LIGHTING_COLOR_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};

//디테일 텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_DETAIL_TEXTURED_LIGHTING_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoordBase : TEXCOORD0;
	float2 texCoordDetail : TEXCOORD1;
};
// 조명과 디테일 텍스쳐 매핑을 처리하는 정점 쉐이더 함수와 픽셀 쉐이더 함수를 다음과 같이 추가한다.
VS_DETAIL_TEXTURED_LIGHTING_COLOR_OUTPUT VSDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_COLOR_INPUT input)
{
	VS_DETAIL_TEXTURED_LIGHTING_COLOR_OUTPUT output = (VS_DETAIL_TEXTURED_LIGHTING_COLOR_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoordBase = input.texCoordBase;
	output.texCoordDetail = input.texCoordDetail;

	return(output);
}

float4 PSDetailTexturedLightingColor(VS_DETAIL_TEXTURED_LIGHTING_COLOR_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW); 
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cBaseTexColor = gtxtTexture.Sample(gSamplerState, input.texCoordBase);
	float4 cDetailTexColor = gtxtDetailTexture.Sample(gDetailSamplerState, input.texCoordDetail);
	float4 cColor = saturate((cBaseTexColor * 0.5f) + (cDetailTexColor * 0.5f));

	return(cColor*cIllumination);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

//인스턴싱, 텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 입력을 위한 구조체이다.
struct VS_INSTANCED_TEXTURED_LIGHTING_COLOR_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
	float4x4 mtxTransform : INSTANCEPOS;
};

//인스턴싱, 텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_INSTANCED_TEXTURED_LIGHTING_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};

// 조명과 텍스쳐 매핑을 처리하는 정점 쉐이더 함수와 픽셀 쉐이더 함수를 다음과 같이 추가한다.
VS_INSTANCED_TEXTURED_LIGHTING_COLOR_OUTPUT VSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_COLOR_INPUT input)
{
    VS_INSTANCED_TEXTURED_LIGHTING_COLOR_OUTPUT output = (VS_INSTANCED_TEXTURED_LIGHTING_COLOR_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)input.mtxTransform);
	output.positionW = mul(float4(input.position, 1.0f), input.mtxTransform).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);
	output.texCoord = input.texCoord;

	return(output);
}

float4 PSInstancedTexturedLightingColor(VS_INSTANCED_TEXTURED_LIGHTING_COLOR_OUTPUT input) : SV_Target
{ 
	input.normalW = normalize(input.normalW); 
	float4 cIllumination = Lighting(input.positionW, input.normalW);
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.texCoord) * cIllumination;

	return(cColor);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 입력을 위한 구조체이다.

cbuffer TranslationBuffer
{
	float textureTranslation;
};

cbuffer TransparentBuffer
{
	float blendAmount;
};

 struct VS_TEXTURED_LIGHTING_COLOR_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD0;
};

//텍스쳐와 조명을 같이 사용하는 경우 정점 쉐이더의 출력을 위한 구조체이다.
struct VS_TEXTURED_LIGHTING_COLOR_OUTPUT
{
	float4 position : SV_POSITION;
	float3 positionW : POSITION;
	float3 normalW : NORMAL;
	float2 texCoord : TEXCOORD0;
};


// 조명과 텍스쳐 매핑을 처리하는 정점 쉐이더 함수와 픽셀 쉐이더 함수를 다음과 같이 추가한다.
VS_TEXTURED_LIGHTING_COLOR_OUTPUT VSTexturedLightingColor(VS_TEXTURED_LIGHTING_COLOR_INPUT input)
{
	VS_TEXTURED_LIGHTING_COLOR_OUTPUT output = (VS_TEXTURED_LIGHTING_COLOR_OUTPUT)0;
	output.normalW = mul(input.normal, (float3x3)gmtxWorld);
	output.positionW = mul(float4(input.position, 1.0f), gmtxWorld).xyz;
	output.position = mul(mul(float4(output.positionW, 1.0f), gmtxView), gmtxProjection);

	output.texCoord = input.texCoord;

	return(output);
}

float4 PSTexturedLightingColor(VS_TEXTURED_LIGHTING_COLOR_OUTPUT input) : SV_Target
{
	input.normalW = normalize(input.normalW);
	float4 cIllumination = Lighting(input.positionW, input.normalW);

	input.texCoord.x += textureTranslation*2.0f;
//	input.texCoord = mul(float4(input.texCoord, 0.0f, 0.0f), float4(textureTranslation, 0.0f, 0.0f, 0.0f)).x;
	float4 cColor = gtxtTexture.Sample(gSamplerState, input.texCoord) * cIllumination;

	cColor.a = blendAmount;
	return(cColor);
}