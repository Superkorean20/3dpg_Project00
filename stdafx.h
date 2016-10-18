// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C의 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <d3d11.h>
#include <d3dx11.h>
#include <D3DX10Math.h>
#include <d3dcompiler.h>

//#include <D3D9Types.h>

#include <Mmsystem.h>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#define FRAME_BUFFER_WIDTH				640
#define FRAME_BUFFER_HEIGHT				480

#define VS_SLOT_CAMERA					0x00
#define VS_SLOT_WORLD_MATRIX			0x01

#define PS_SLOT_LIGHT					0x00
#define PS_SLOT_MATERIAL				0x01
#define PS_SLOT_TRANSLATION				0x02

//#define _WITH_SHADER_ROW_MAJOR_MATRIX

#define RANDOM_COLOR	D3DXCOLOR((rand() * 0xFFFFFF) / RAND_MAX)

//#define _WITH_TERRAIN_PARTITION
#define _WITH_FRUSTUM_CULLING_BY_OBJECT
//#define _WITH_FRUSTUM_CULLING_BY_SUBMESH

// 텍스쳐와 샘플러 상태를 설정하기 위한 쉐이더의 슬롯 번호를 정의한다.
#define PS_SLOT_TEXTURE	0x00
#define PS_SLOT_SAMPLER_STATE 0x00


