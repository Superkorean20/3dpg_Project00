// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

//#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C�� ��Ÿ�� ��� �����Դϴ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.

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

// �ؽ��Ŀ� ���÷� ���¸� �����ϱ� ���� ���̴��� ���� ��ȣ�� �����Ѵ�.
#define PS_SLOT_TEXTURE	0x00
#define PS_SLOT_SAMPLER_STATE 0x00


