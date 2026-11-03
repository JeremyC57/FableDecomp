#ifndef FABLE_ENGINE_DIRECT3D9_TYPES_H
#define FABLE_ENGINE_DIRECT3D9_TYPES_H
// Selected original Direct3D9 SDK declarations, independently checked against Ego_r TPI.
// Copyright (C) Microsoft Corporation. All Rights Reserved.
typedef unsigned long DWORD;
typedef unsigned int UINT;
typedef int INT;
typedef int BOOL;
struct HWND__;typedef HWND__* HWND;
typedef enum _D3DDEVTYPE
{
    D3DDEVTYPE_HAL         = 1,
    D3DDEVTYPE_REF         = 2,
    D3DDEVTYPE_SW          = 3,

    D3DDEVTYPE_NULLREF     = 4,

    D3DDEVTYPE_FORCE_DWORD  = 0x7fffffff
} D3DDEVTYPE;
typedef enum _D3DMULTISAMPLE_TYPE
{
    D3DMULTISAMPLE_NONE            =  0,
    D3DMULTISAMPLE_NONMASKABLE     =  1,
    D3DMULTISAMPLE_2_SAMPLES       =  2,
    D3DMULTISAMPLE_3_SAMPLES       =  3,
    D3DMULTISAMPLE_4_SAMPLES       =  4,
    D3DMULTISAMPLE_5_SAMPLES       =  5,
    D3DMULTISAMPLE_6_SAMPLES       =  6,
    D3DMULTISAMPLE_7_SAMPLES       =  7,
    D3DMULTISAMPLE_8_SAMPLES       =  8,
    D3DMULTISAMPLE_9_SAMPLES       =  9,
    D3DMULTISAMPLE_10_SAMPLES      = 10,
    D3DMULTISAMPLE_11_SAMPLES      = 11,
    D3DMULTISAMPLE_12_SAMPLES      = 12,
    D3DMULTISAMPLE_13_SAMPLES      = 13,
    D3DMULTISAMPLE_14_SAMPLES      = 14,
    D3DMULTISAMPLE_15_SAMPLES      = 15,
    D3DMULTISAMPLE_16_SAMPLES      = 16,

    D3DMULTISAMPLE_FORCE_DWORD     = 0x7fffffff
} D3DMULTISAMPLE_TYPE;
typedef enum _D3DSWAPEFFECT
{
    D3DSWAPEFFECT_DISCARD           = 1,
    D3DSWAPEFFECT_FLIP              = 2,
    D3DSWAPEFFECT_COPY              = 3,

/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
    D3DSWAPEFFECT_OVERLAY           = 4,
    D3DSWAPEFFECT_FLIPEX            = 5,
#endif // !D3D_DISABLE_9EX
/* -- D3D9Ex only */

    D3DSWAPEFFECT_FORCE_DWORD       = 0x7fffffff
} D3DSWAPEFFECT;
enum _D3DFORMAT {
 D3DFMT_UNKNOWN=0,
 D3DFMT_R8G8B8=20,
 D3DFMT_A8R8G8B8=21,
 D3DFMT_X8R8G8B8=22,
 D3DFMT_R5G6B5=23,
 D3DFMT_X1R5G5B5=24,
 D3DFMT_A1R5G5B5=25,
 D3DFMT_A4R4G4B4=26,
 D3DFMT_R3G3B2=27,
 D3DFMT_A8=28,
 D3DFMT_A8R3G3B2=29,
 D3DFMT_X4R4G4B4=30,
 D3DFMT_A2B10G10R10=31,
 D3DFMT_A8B8G8R8=32,
 D3DFMT_X8B8G8R8=33,
 D3DFMT_G16R16=34,
 D3DFMT_A2R10G10B10=35,
 D3DFMT_A16B16G16R16=36,
 D3DFMT_A8P8=40,
 D3DFMT_P8=41,
 D3DFMT_L8=50,
 D3DFMT_A8L8=51,
 D3DFMT_A4L4=52,
 D3DFMT_V8U8=60,
 D3DFMT_L6V5U5=61,
 D3DFMT_X8L8V8U8=62,
 D3DFMT_Q8W8V8U8=63,
 D3DFMT_V16U16=64,
 D3DFMT_A2W10V10U10=67,
 D3DFMT_UYVY=1498831189,
 D3DFMT_R8G8_B8G8=1195525970,
 D3DFMT_YUY2=844715353,
 D3DFMT_G8R8_G8B8=1111970375,
 D3DFMT_DXT1=827611204,
 D3DFMT_DXT2=844388420,
 D3DFMT_DXT3=861165636,
 D3DFMT_DXT4=877942852,
 D3DFMT_DXT5=894720068,
 D3DFMT_D16_LOCKABLE=70,
 D3DFMT_D32=71,
 D3DFMT_D15S1=73,
 D3DFMT_D24S8=75,
 D3DFMT_D24X8=77,
 D3DFMT_D24X4S4=79,
 D3DFMT_D16=80,
 D3DFMT_D32F_LOCKABLE=82,
 D3DFMT_D24FS8=83,
 D3DFMT_L16=81,
 D3DFMT_VERTEXDATA=100,
 D3DFMT_INDEX16=101,
 D3DFMT_INDEX32=102,
 D3DFMT_Q16W16V16U16=110,
 D3DFMT_MULTI2_ARGB8=827606349,
 D3DFMT_R16F=111,
 D3DFMT_G16R16F=112,
 D3DFMT_A16B16G16R16F=113,
 D3DFMT_R32F=114,
 D3DFMT_G32R32F=115,
 D3DFMT_A32B32G32R32F=116,
 D3DFMT_CxV8U8=117,
 D3DFMT_FORCE_DWORD=2147483647
};
typedef _D3DFORMAT D3DFORMAT;
typedef struct _D3DVSHADERCAPS2_0
{
        DWORD Caps;
        INT DynamicFlowControlDepth;
        INT NumTemps;
        INT StaticFlowControlDepth;
} D3DVSHADERCAPS2_0;
typedef struct _D3DPSHADERCAPS2_0
{
    DWORD Caps;
    INT DynamicFlowControlDepth;
    INT NumTemps;
    INT StaticFlowControlDepth;
    INT NumInstructionSlots;
} D3DPSHADERCAPS2_0;
typedef struct _D3DCAPS9
{
    /* Device Info */
    D3DDEVTYPE  DeviceType;
    UINT        AdapterOrdinal;

    /* Caps from DX7 Draw */
    DWORD   Caps;
    DWORD   Caps2;
    DWORD   Caps3;
    DWORD   PresentationIntervals;

    /* Cursor Caps */
    DWORD   CursorCaps;

    /* 3D Device Caps */
    DWORD   DevCaps;

    DWORD   PrimitiveMiscCaps;
    DWORD   RasterCaps;
    DWORD   ZCmpCaps;
    DWORD   SrcBlendCaps;
    DWORD   DestBlendCaps;
    DWORD   AlphaCmpCaps;
    DWORD   ShadeCaps;
    DWORD   TextureCaps;
    DWORD   TextureFilterCaps;          // D3DPTFILTERCAPS for IDirect3DTexture9's
    DWORD   CubeTextureFilterCaps;      // D3DPTFILTERCAPS for IDirect3DCubeTexture9's
    DWORD   VolumeTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DVolumeTexture9's
    DWORD   TextureAddressCaps;         // D3DPTADDRESSCAPS for IDirect3DTexture9's
    DWORD   VolumeTextureAddressCaps;   // D3DPTADDRESSCAPS for IDirect3DVolumeTexture9's

    DWORD   LineCaps;                   // D3DLINECAPS

    DWORD   MaxTextureWidth, MaxTextureHeight;
    DWORD   MaxVolumeExtent;

    DWORD   MaxTextureRepeat;
    DWORD   MaxTextureAspectRatio;
    DWORD   MaxAnisotropy;
    float   MaxVertexW;

    float   GuardBandLeft;
    float   GuardBandTop;
    float   GuardBandRight;
    float   GuardBandBottom;

    float   ExtentsAdjust;
    DWORD   StencilCaps;

    DWORD   FVFCaps;
    DWORD   TextureOpCaps;
    DWORD   MaxTextureBlendStages;
    DWORD   MaxSimultaneousTextures;

    DWORD   VertexProcessingCaps;
    DWORD   MaxActiveLights;
    DWORD   MaxUserClipPlanes;
    DWORD   MaxVertexBlendMatrices;
    DWORD   MaxVertexBlendMatrixIndex;

    float   MaxPointSize;

    DWORD   MaxPrimitiveCount;          // max number of primitives per DrawPrimitive call
    DWORD   MaxVertexIndex;
    DWORD   MaxStreams;
    DWORD   MaxStreamStride;            // max stride for SetStreamSource

    DWORD   VertexShaderVersion;
    DWORD   MaxVertexShaderConst;       // number of vertex shader constant registers

    DWORD   PixelShaderVersion;
    float   PixelShader1xMaxValue;      // max value storable in registers of ps.1.x shaders

    // Here are the DX9 specific ones
    DWORD   DevCaps2;

    float   MaxNpatchTessellationLevel;
    DWORD   Reserved5;

    UINT    MasterAdapterOrdinal;       // ordinal of master adaptor for adapter group
    UINT    AdapterOrdinalInGroup;      // ordinal inside the adapter group
    UINT    NumberOfAdaptersInGroup;    // number of adapters in this adapter group (only if master)
    DWORD   DeclTypes;                  // Data types, supported in vertex declarations
    DWORD   NumSimultaneousRTs;         // Will be at least 1
    DWORD   StretchRectFilterCaps;      // Filter caps supported by StretchRect
    D3DVSHADERCAPS2_0 VS20Caps;
    D3DPSHADERCAPS2_0 PS20Caps;
    DWORD   VertexTextureFilterCaps;    // D3DPTFILTERCAPS for IDirect3DTexture9's for texture, used in vertex shaders
    DWORD   MaxVShaderInstructionsExecuted; // maximum number of vertex shader instructions that can be executed
    DWORD   MaxPShaderInstructionsExecuted; // maximum number of pixel shader instructions that can be executed
    DWORD   MaxVertexShader30InstructionSlots; 
    DWORD   MaxPixelShader30InstructionSlots;
} D3DCAPS9;
typedef struct _D3DDISPLAYMODE
{
    UINT            Width;
    UINT            Height;
    UINT            RefreshRate;
    D3DFORMAT       Format;
} D3DDISPLAYMODE;
typedef struct _D3DVIEWPORT9 {
    DWORD       X;
    DWORD       Y;            /* Viewport Top left */
    DWORD       Width;
    DWORD       Height;       /* Viewport Dimensions */
    float       MinZ;         /* Min/max of clip Volume */
    float       MaxZ;
} D3DVIEWPORT9;
typedef struct _D3DPRESENT_PARAMETERS_
{
    UINT                BackBufferWidth;
    UINT                BackBufferHeight;
    D3DFORMAT           BackBufferFormat;
    UINT                BackBufferCount;

    D3DMULTISAMPLE_TYPE MultiSampleType;
    DWORD               MultiSampleQuality;

    D3DSWAPEFFECT       SwapEffect;
    HWND                hDeviceWindow;
    BOOL                Windowed;
    BOOL                EnableAutoDepthStencil;
    D3DFORMAT           AutoDepthStencilFormat;
    DWORD               Flags;

    /* FullScreen_RefreshRateInHz must be zero for Windowed mode */
    UINT                FullScreen_RefreshRateInHz;
    UINT                PresentationInterval;
} D3DPRESENT_PARAMETERS;
#endif

