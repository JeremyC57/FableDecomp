#ifndef FABLE_ENGINE_DIRECT3D9DEVICE_H
#define FABLE_ENGINE_DIRECT3D9DEVICE_H
// Original Microsoft Direct3D9 SDK interface, all119 slots retained in SDK order.
// Copyright (C) Microsoft Corporation. All Rights Reserved.
// Cross-check: original Ego_r IDirect3DDevice9 TPI; native00989cb0 calls slot94.
// Pointer-only SDK types are forward declarations; original enum definitions follow.
typedef long HRESULT;typedef unsigned long ULONG;typedef unsigned long DWORD;
typedef unsigned int UINT;typedef int INT;typedef int BOOL;typedef unsigned long D3DCOLOR;
#ifndef MAKEFOURCC
#define MAKEFOURCC(ch0,ch1,ch2,ch3) ((unsigned long)(unsigned char)(ch0) | ((unsigned long)(unsigned char)(ch1)<<8) | ((unsigned long)(unsigned char)(ch2)<<16) | ((unsigned long)(unsigned char)(ch3)<<24))
#endif
typedef void* HANDLE;struct HWND__;typedef HWND__* HWND;
struct _GUID;typedef const _GUID& REFIID;
struct tagRECT;typedef tagRECT RECT;struct tagPOINT;typedef tagPOINT POINT;
struct _RGNDATA;typedef _RGNDATA RGNDATA;struct tagPALETTEENTRY;typedef tagPALETTEENTRY PALETTEENTRY;
struct IDirect3D9;
struct IDirect3DBaseTexture9;
struct IDirect3DCubeTexture9;
struct IDirect3DIndexBuffer9;
struct IDirect3DPixelShader9;
struct IDirect3DQuery9;
struct IDirect3DStateBlock9;
struct IDirect3DSurface9;
struct IDirect3DSwapChain9;
struct IDirect3DTexture9;
struct IDirect3DVertexBuffer9;
struct IDirect3DVertexDeclaration9;
struct IDirect3DVertexShader9;
struct IDirect3DVolumeTexture9;
typedef enum _D3DBACKBUFFER_TYPE
{
    D3DBACKBUFFER_TYPE_MONO         = 0,
    D3DBACKBUFFER_TYPE_LEFT         = 1,
    D3DBACKBUFFER_TYPE_RIGHT        = 2,

    D3DBACKBUFFER_TYPE_FORCE_DWORD  = 0x7fffffff
} D3DBACKBUFFER_TYPE;
struct _D3DCAPS9;typedef _D3DCAPS9 D3DCAPS9;
struct _D3DCLIPSTATUS9;typedef _D3DCLIPSTATUS9 D3DCLIPSTATUS9;
struct _D3DDEVICE_CREATION_PARAMETERS;typedef _D3DDEVICE_CREATION_PARAMETERS D3DDEVICE_CREATION_PARAMETERS;
struct _D3DDISPLAYMODE;typedef _D3DDISPLAYMODE D3DDISPLAYMODE;
typedef enum _D3DFORMAT
{
    D3DFMT_UNKNOWN              =  0,

    D3DFMT_R8G8B8               = 20,
    D3DFMT_A8R8G8B8             = 21,
    D3DFMT_X8R8G8B8             = 22,
    D3DFMT_R5G6B5               = 23,
    D3DFMT_X1R5G5B5             = 24,
    D3DFMT_A1R5G5B5             = 25,
    D3DFMT_A4R4G4B4             = 26,
    D3DFMT_R3G3B2               = 27,
    D3DFMT_A8                   = 28,
    D3DFMT_A8R3G3B2             = 29,
    D3DFMT_X4R4G4B4             = 30,
    D3DFMT_A2B10G10R10          = 31,
    D3DFMT_A8B8G8R8             = 32,
    D3DFMT_X8B8G8R8             = 33,
    D3DFMT_G16R16               = 34,
    D3DFMT_A2R10G10B10          = 35,
    D3DFMT_A16B16G16R16         = 36,

    D3DFMT_A8P8                 = 40,
    D3DFMT_P8                   = 41,

    D3DFMT_L8                   = 50,
    D3DFMT_A8L8                 = 51,
    D3DFMT_A4L4                 = 52,

    D3DFMT_V8U8                 = 60,
    D3DFMT_L6V5U5               = 61,
    D3DFMT_X8L8V8U8             = 62,
    D3DFMT_Q8W8V8U8             = 63,
    D3DFMT_V16U16               = 64,
    D3DFMT_A2W10V10U10          = 67,

    D3DFMT_UYVY                 = MAKEFOURCC('U', 'Y', 'V', 'Y'),
    D3DFMT_R8G8_B8G8            = MAKEFOURCC('R', 'G', 'B', 'G'),
    D3DFMT_YUY2                 = MAKEFOURCC('Y', 'U', 'Y', '2'),
    D3DFMT_G8R8_G8B8            = MAKEFOURCC('G', 'R', 'G', 'B'),
    D3DFMT_DXT1                 = MAKEFOURCC('D', 'X', 'T', '1'),
    D3DFMT_DXT2                 = MAKEFOURCC('D', 'X', 'T', '2'),
    D3DFMT_DXT3                 = MAKEFOURCC('D', 'X', 'T', '3'),
    D3DFMT_DXT4                 = MAKEFOURCC('D', 'X', 'T', '4'),
    D3DFMT_DXT5                 = MAKEFOURCC('D', 'X', 'T', '5'),

    D3DFMT_D16_LOCKABLE         = 70,
    D3DFMT_D32                  = 71,
    D3DFMT_D15S1                = 73,
    D3DFMT_D24S8                = 75,
    D3DFMT_D24X8                = 77,
    D3DFMT_D24X4S4              = 79,
    D3DFMT_D16                  = 80,

    D3DFMT_D32F_LOCKABLE        = 82,
    D3DFMT_D24FS8               = 83,

/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)

    /* Z-Stencil formats valid for CPU access */
    D3DFMT_D32_LOCKABLE         = 84,
    D3DFMT_S8_LOCKABLE          = 85,

#endif // !D3D_DISABLE_9EX
/* -- D3D9Ex only */


    D3DFMT_L16                  = 81,

    D3DFMT_VERTEXDATA           =100,
    D3DFMT_INDEX16              =101,
    D3DFMT_INDEX32              =102,

    D3DFMT_Q16W16V16U16         =110,

    D3DFMT_MULTI2_ARGB8         = MAKEFOURCC('M','E','T','1'),

    // Floating point surface formats

    // s10e5 formats (16-bits per channel)
    D3DFMT_R16F                 = 111,
    D3DFMT_G16R16F              = 112,
    D3DFMT_A16B16G16R16F        = 113,

    // IEEE s23e8 formats (32-bits per channel)
    D3DFMT_R32F                 = 114,
    D3DFMT_G32R32F              = 115,
    D3DFMT_A32B32G32R32F        = 116,

    D3DFMT_CxV8U8               = 117,

/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)

    // Monochrome 1 bit per pixel format
    D3DFMT_A1                   = 118,

    // 2.8 biased fixed point
    D3DFMT_A2B10G10R10_XR_BIAS  = 119,


    // Binary format indicating that the data has no inherent type
    D3DFMT_BINARYBUFFER         = 199,
    
#endif // !D3D_DISABLE_9EX
/* -- D3D9Ex only */


    D3DFMT_FORCE_DWORD          =0x7fffffff
} D3DFORMAT;
struct _D3DGAMMARAMP;typedef _D3DGAMMARAMP D3DGAMMARAMP;
struct _D3DLIGHT9;typedef _D3DLIGHT9 D3DLIGHT9;
struct _D3DMATERIAL9;typedef _D3DMATERIAL9 D3DMATERIAL9;
struct _D3DMATRIX;typedef _D3DMATRIX D3DMATRIX;
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
typedef enum _D3DPOOL {
    D3DPOOL_DEFAULT                 = 0,
    D3DPOOL_MANAGED                 = 1,
    D3DPOOL_SYSTEMMEM               = 2,
    D3DPOOL_SCRATCH                 = 3,

    D3DPOOL_FORCE_DWORD             = 0x7fffffff
} D3DPOOL;
struct _D3DPRESENT_PARAMETERS_;typedef _D3DPRESENT_PARAMETERS_ D3DPRESENT_PARAMETERS;
typedef enum _D3DPRIMITIVETYPE {
    D3DPT_POINTLIST             = 1,
    D3DPT_LINELIST              = 2,
    D3DPT_LINESTRIP             = 3,
    D3DPT_TRIANGLELIST          = 4,
    D3DPT_TRIANGLESTRIP         = 5,
    D3DPT_TRIANGLEFAN           = 6,
    D3DPT_FORCE_DWORD           = 0x7fffffff, /* force 32-bit size enum */
} D3DPRIMITIVETYPE;
typedef enum _D3DQUERYTYPE {
    D3DQUERYTYPE_VCACHE                 = 4, /* D3DISSUE_END */
    D3DQUERYTYPE_RESOURCEMANAGER        = 5, /* D3DISSUE_END */
    D3DQUERYTYPE_VERTEXSTATS            = 6, /* D3DISSUE_END */
    D3DQUERYTYPE_EVENT                  = 8, /* D3DISSUE_END */
    D3DQUERYTYPE_OCCLUSION              = 9, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_TIMESTAMP              = 10, /* D3DISSUE_END */
    D3DQUERYTYPE_TIMESTAMPDISJOINT      = 11, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_TIMESTAMPFREQ          = 12, /* D3DISSUE_END */
    D3DQUERYTYPE_PIPELINETIMINGS        = 13, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_INTERFACETIMINGS       = 14, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_VERTEXTIMINGS          = 15, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_PIXELTIMINGS           = 16, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_BANDWIDTHTIMINGS       = 17, /* D3DISSUE_BEGIN, D3DISSUE_END */
    D3DQUERYTYPE_CACHEUTILIZATION       = 18, /* D3DISSUE_BEGIN, D3DISSUE_END */
/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
    D3DQUERYTYPE_MEMORYPRESSURE         = 19, /* D3DISSUE_BEGIN, D3DISSUE_END */
#endif // !D3D_DISABLE_9EX
} D3DQUERYTYPE;
struct _D3DRASTER_STATUS;typedef _D3DRASTER_STATUS D3DRASTER_STATUS;
struct _D3DRECT;typedef _D3DRECT D3DRECT;
struct _D3DRECTPATCH_INFO;typedef _D3DRECTPATCH_INFO D3DRECTPATCH_INFO;
typedef enum _D3DRENDERSTATETYPE {
    D3DRS_ZENABLE                   = 7,    /* D3DZBUFFERTYPE (or TRUE/FALSE for legacy) */
    D3DRS_FILLMODE                  = 8,    /* D3DFILLMODE */
    D3DRS_SHADEMODE                 = 9,    /* D3DSHADEMODE */
    D3DRS_ZWRITEENABLE              = 14,   /* TRUE to enable z writes */
    D3DRS_ALPHATESTENABLE           = 15,   /* TRUE to enable alpha tests */
    D3DRS_LASTPIXEL                 = 16,   /* TRUE for last-pixel on lines */
    D3DRS_SRCBLEND                  = 19,   /* D3DBLEND */
    D3DRS_DESTBLEND                 = 20,   /* D3DBLEND */
    D3DRS_CULLMODE                  = 22,   /* D3DCULL */
    D3DRS_ZFUNC                     = 23,   /* D3DCMPFUNC */
    D3DRS_ALPHAREF                  = 24,   /* D3DFIXED */
    D3DRS_ALPHAFUNC                 = 25,   /* D3DCMPFUNC */
    D3DRS_DITHERENABLE              = 26,   /* TRUE to enable dithering */
    D3DRS_ALPHABLENDENABLE          = 27,   /* TRUE to enable alpha blending */
    D3DRS_FOGENABLE                 = 28,   /* TRUE to enable fog blending */
    D3DRS_SPECULARENABLE            = 29,   /* TRUE to enable specular */
    D3DRS_FOGCOLOR                  = 34,   /* D3DCOLOR */
    D3DRS_FOGTABLEMODE              = 35,   /* D3DFOGMODE */
    D3DRS_FOGSTART                  = 36,   /* Fog start (for both vertex and pixel fog) */
    D3DRS_FOGEND                    = 37,   /* Fog end      */
    D3DRS_FOGDENSITY                = 38,   /* Fog density  */
    D3DRS_RANGEFOGENABLE            = 48,   /* Enables range-based fog */
    D3DRS_STENCILENABLE             = 52,   /* BOOL enable/disable stenciling */
    D3DRS_STENCILFAIL               = 53,   /* D3DSTENCILOP to do if stencil test fails */
    D3DRS_STENCILZFAIL              = 54,   /* D3DSTENCILOP to do if stencil test passes and Z test fails */
    D3DRS_STENCILPASS               = 55,   /* D3DSTENCILOP to do if both stencil and Z tests pass */
    D3DRS_STENCILFUNC               = 56,   /* D3DCMPFUNC fn.  Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
    D3DRS_STENCILREF                = 57,   /* Reference value used in stencil test */
    D3DRS_STENCILMASK               = 58,   /* Mask value used in stencil test */
    D3DRS_STENCILWRITEMASK          = 59,   /* Write mask applied to values written to stencil buffer */
    D3DRS_TEXTUREFACTOR             = 60,   /* D3DCOLOR used for multi-texture blend */
    D3DRS_WRAP0                     = 128,  /* wrap for 1st texture coord. set */
    D3DRS_WRAP1                     = 129,  /* wrap for 2nd texture coord. set */
    D3DRS_WRAP2                     = 130,  /* wrap for 3rd texture coord. set */
    D3DRS_WRAP3                     = 131,  /* wrap for 4th texture coord. set */
    D3DRS_WRAP4                     = 132,  /* wrap for 5th texture coord. set */
    D3DRS_WRAP5                     = 133,  /* wrap for 6th texture coord. set */
    D3DRS_WRAP6                     = 134,  /* wrap for 7th texture coord. set */
    D3DRS_WRAP7                     = 135,  /* wrap for 8th texture coord. set */
    D3DRS_CLIPPING                  = 136,
    D3DRS_LIGHTING                  = 137,
    D3DRS_AMBIENT                   = 139,
    D3DRS_FOGVERTEXMODE             = 140,
    D3DRS_COLORVERTEX               = 141,
    D3DRS_LOCALVIEWER               = 142,
    D3DRS_NORMALIZENORMALS          = 143,
    D3DRS_DIFFUSEMATERIALSOURCE     = 145,
    D3DRS_SPECULARMATERIALSOURCE    = 146,
    D3DRS_AMBIENTMATERIALSOURCE     = 147,
    D3DRS_EMISSIVEMATERIALSOURCE    = 148,
    D3DRS_VERTEXBLEND               = 151,
    D3DRS_CLIPPLANEENABLE           = 152,
    D3DRS_POINTSIZE                 = 154,   /* float point size */
    D3DRS_POINTSIZE_MIN             = 155,   /* float point size min threshold */
    D3DRS_POINTSPRITEENABLE         = 156,   /* BOOL point texture coord control */
    D3DRS_POINTSCALEENABLE          = 157,   /* BOOL point size scale enable */
    D3DRS_POINTSCALE_A              = 158,   /* float point attenuation A value */
    D3DRS_POINTSCALE_B              = 159,   /* float point attenuation B value */
    D3DRS_POINTSCALE_C              = 160,   /* float point attenuation C value */
    D3DRS_MULTISAMPLEANTIALIAS      = 161,  // BOOL - set to do FSAA with multisample buffer
    D3DRS_MULTISAMPLEMASK           = 162,  // DWORD - per-sample enable/disable
    D3DRS_PATCHEDGESTYLE            = 163,  // Sets whether patch edges will use float style tessellation
    D3DRS_DEBUGMONITORTOKEN         = 165,  // DEBUG ONLY - token to debug monitor
    D3DRS_POINTSIZE_MAX             = 166,   /* float point size max threshold */
    D3DRS_INDEXEDVERTEXBLENDENABLE  = 167,
    D3DRS_COLORWRITEENABLE          = 168,  // per-channel write enable
    D3DRS_TWEENFACTOR               = 170,   // float tween factor
    D3DRS_BLENDOP                   = 171,   // D3DBLENDOP setting
    D3DRS_POSITIONDEGREE            = 172,   // NPatch position interpolation degree. D3DDEGREE_LINEAR or D3DDEGREE_CUBIC (default)
    D3DRS_NORMALDEGREE              = 173,   // NPatch normal interpolation degree. D3DDEGREE_LINEAR (default) or D3DDEGREE_QUADRATIC
    D3DRS_SCISSORTESTENABLE         = 174,
    D3DRS_SLOPESCALEDEPTHBIAS       = 175,
    D3DRS_ANTIALIASEDLINEENABLE     = 176,
    D3DRS_MINTESSELLATIONLEVEL      = 178,
    D3DRS_MAXTESSELLATIONLEVEL      = 179,
    D3DRS_ADAPTIVETESS_X            = 180,
    D3DRS_ADAPTIVETESS_Y            = 181,
    D3DRS_ADAPTIVETESS_Z            = 182,
    D3DRS_ADAPTIVETESS_W            = 183,
    D3DRS_ENABLEADAPTIVETESSELLATION = 184,
    D3DRS_TWOSIDEDSTENCILMODE       = 185,   /* BOOL enable/disable 2 sided stenciling */
    D3DRS_CCW_STENCILFAIL           = 186,   /* D3DSTENCILOP to do if ccw stencil test fails */
    D3DRS_CCW_STENCILZFAIL          = 187,   /* D3DSTENCILOP to do if ccw stencil test passes and Z test fails */
    D3DRS_CCW_STENCILPASS           = 188,   /* D3DSTENCILOP to do if both ccw stencil and Z tests pass */
    D3DRS_CCW_STENCILFUNC           = 189,   /* D3DCMPFUNC fn.  ccw Stencil Test passes if ((ref & mask) stencilfn (stencil & mask)) is true */
    D3DRS_COLORWRITEENABLE1         = 190,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
    D3DRS_COLORWRITEENABLE2         = 191,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
    D3DRS_COLORWRITEENABLE3         = 192,   /* Additional ColorWriteEnables for the devices that support D3DPMISCCAPS_INDEPENDENTWRITEMASKS */
    D3DRS_BLENDFACTOR               = 193,   /* D3DCOLOR used for a constant blend factor during alpha blending for devices that support D3DPBLENDCAPS_BLENDFACTOR */
    D3DRS_SRGBWRITEENABLE           = 194,   /* Enable rendertarget writes to be DE-linearized to SRGB (for formats that expose D3DUSAGE_QUERY_SRGBWRITE) */
    D3DRS_DEPTHBIAS                 = 195,
    D3DRS_WRAP8                     = 198,   /* Additional wrap states for vs_3_0+ attributes with D3DDECLUSAGE_TEXCOORD */
    D3DRS_WRAP9                     = 199,
    D3DRS_WRAP10                    = 200,
    D3DRS_WRAP11                    = 201,
    D3DRS_WRAP12                    = 202,
    D3DRS_WRAP13                    = 203,
    D3DRS_WRAP14                    = 204,
    D3DRS_WRAP15                    = 205,
    D3DRS_SEPARATEALPHABLENDENABLE  = 206,  /* TRUE to enable a separate blending function for the alpha channel */
    D3DRS_SRCBLENDALPHA             = 207,  /* SRC blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
    D3DRS_DESTBLENDALPHA            = 208,  /* DST blend factor for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */
    D3DRS_BLENDOPALPHA              = 209,  /* Blending operation for the alpha channel when D3DRS_SEPARATEDESTALPHAENABLE is TRUE */


    D3DRS_FORCE_DWORD               = 0x7fffffff, /* force 32-bit size enum */
} D3DRENDERSTATETYPE;
typedef enum _D3DSAMPLERSTATETYPE
{
    D3DSAMP_ADDRESSU       = 1,  /* D3DTEXTUREADDRESS for U coordinate */
    D3DSAMP_ADDRESSV       = 2,  /* D3DTEXTUREADDRESS for V coordinate */
    D3DSAMP_ADDRESSW       = 3,  /* D3DTEXTUREADDRESS for W coordinate */
    D3DSAMP_BORDERCOLOR    = 4,  /* D3DCOLOR */
    D3DSAMP_MAGFILTER      = 5,  /* D3DTEXTUREFILTER filter to use for magnification */
    D3DSAMP_MINFILTER      = 6,  /* D3DTEXTUREFILTER filter to use for minification */
    D3DSAMP_MIPFILTER      = 7,  /* D3DTEXTUREFILTER filter to use between mipmaps during minification */
    D3DSAMP_MIPMAPLODBIAS  = 8,  /* float Mipmap LOD bias */
    D3DSAMP_MAXMIPLEVEL    = 9,  /* DWORD 0..(n-1) LOD index of largest map to use (0 == largest) */
    D3DSAMP_MAXANISOTROPY  = 10, /* DWORD maximum anisotropy */
    D3DSAMP_SRGBTEXTURE    = 11, /* Default = 0 (which means Gamma 1.0,
                                   no correction required.) else correct for
                                   Gamma = 2.2 */
    D3DSAMP_ELEMENTINDEX   = 12, /* When multi-element texture is assigned to sampler, this
                                    indicates which element index to use.  Default = 0.  */
    D3DSAMP_DMAPOFFSET     = 13, /* Offset in vertices in the pre-sampled displacement map.
                                    Only valid for D3DDMAPSAMPLER sampler  */
    D3DSAMP_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} D3DSAMPLERSTATETYPE;
typedef enum _D3DSTATEBLOCKTYPE
{
    D3DSBT_ALL           = 1, // capture all state
    D3DSBT_PIXELSTATE    = 2, // capture pixel state
    D3DSBT_VERTEXSTATE   = 3, // capture vertex state
    D3DSBT_FORCE_DWORD   = 0x7fffffff,
} D3DSTATEBLOCKTYPE;
typedef enum _D3DTEXTUREFILTERTYPE
{
    D3DTEXF_NONE            = 0,    // filtering disabled (valid for mip filter only)
    D3DTEXF_POINT           = 1,    // nearest
    D3DTEXF_LINEAR          = 2,    // linear interpolation
    D3DTEXF_ANISOTROPIC     = 3,    // anisotropic
    D3DTEXF_PYRAMIDALQUAD   = 6,    // 4-sample tent
    D3DTEXF_GAUSSIANQUAD    = 7,    // 4-sample gaussian
/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)

    D3DTEXF_CONVOLUTIONMONO = 8,    // Convolution filter for monochrome textures

#endif // !D3D_DISABLE_9EX
/* -- D3D9Ex only */
    D3DTEXF_FORCE_DWORD     = 0x7fffffff,   // force 32-bit size enum
} D3DTEXTUREFILTERTYPE;
typedef enum _D3DTEXTURESTAGESTATETYPE
{
    D3DTSS_COLOROP        =  1, /* D3DTEXTUREOP - per-stage blending controls for color channels */
    D3DTSS_COLORARG1      =  2, /* D3DTA_* (texture arg) */
    D3DTSS_COLORARG2      =  3, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAOP        =  4, /* D3DTEXTUREOP - per-stage blending controls for alpha channel */
    D3DTSS_ALPHAARG1      =  5, /* D3DTA_* (texture arg) */
    D3DTSS_ALPHAARG2      =  6, /* D3DTA_* (texture arg) */
    D3DTSS_BUMPENVMAT00   =  7, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT01   =  8, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT10   =  9, /* float (bump mapping matrix) */
    D3DTSS_BUMPENVMAT11   = 10, /* float (bump mapping matrix) */
    D3DTSS_TEXCOORDINDEX  = 11, /* identifies which set of texture coordinates index this texture */
    D3DTSS_BUMPENVLSCALE  = 22, /* float scale for bump map luminance */
    D3DTSS_BUMPENVLOFFSET = 23, /* float offset for bump map luminance */
    D3DTSS_TEXTURETRANSFORMFLAGS = 24, /* D3DTEXTURETRANSFORMFLAGS controls texture transform */
    D3DTSS_COLORARG0      = 26, /* D3DTA_* third arg for triadic ops */
    D3DTSS_ALPHAARG0      = 27, /* D3DTA_* third arg for triadic ops */
    D3DTSS_RESULTARG      = 28, /* D3DTA_* arg for result (CURRENT or TEMP) */
    D3DTSS_CONSTANT       = 32, /* Per-stage constant D3DTA_CONSTANT */


    D3DTSS_FORCE_DWORD   = 0x7fffffff, /* force 32-bit size enum */
} D3DTEXTURESTAGESTATETYPE;
typedef enum _D3DTRANSFORMSTATETYPE {
    D3DTS_VIEW          = 2,
    D3DTS_PROJECTION    = 3,
    D3DTS_TEXTURE0      = 16,
    D3DTS_TEXTURE1      = 17,
    D3DTS_TEXTURE2      = 18,
    D3DTS_TEXTURE3      = 19,
    D3DTS_TEXTURE4      = 20,
    D3DTS_TEXTURE5      = 21,
    D3DTS_TEXTURE6      = 22,
    D3DTS_TEXTURE7      = 23,
    D3DTS_FORCE_DWORD     = 0x7fffffff, /* force 32-bit size enum */
} D3DTRANSFORMSTATETYPE;
struct _D3DTRIPATCH_INFO;typedef _D3DTRIPATCH_INFO D3DTRIPATCH_INFO;
struct _D3DVERTEXELEMENT9;typedef _D3DVERTEXELEMENT9 D3DVERTEXELEMENT9;
struct _D3DVIEWPORT9;typedef _D3DVIEWPORT9 D3DVIEWPORT9;
struct IUnknown {
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj)=0;
    virtual ULONG __stdcall AddRef()=0;
    virtual ULONG __stdcall Release()=0;
};
struct IDirect3DDevice9 : public IUnknown {
    virtual HRESULT __stdcall QueryInterface(REFIID riid, void** ppvObj)=0; // 0
    virtual ULONG __stdcall AddRef()=0; // 1
    virtual ULONG __stdcall Release()=0; // 2
    virtual HRESULT __stdcall TestCooperativeLevel()=0; // 3
    virtual UINT __stdcall GetAvailableTextureMem()=0; // 4
    virtual HRESULT __stdcall EvictManagedResources()=0; // 5
    virtual HRESULT __stdcall GetDirect3D(IDirect3D9** ppD3D9)=0; // 6
    virtual HRESULT __stdcall GetDeviceCaps(D3DCAPS9* pCaps)=0; // 7
    virtual HRESULT __stdcall GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode)=0; // 8
    virtual HRESULT __stdcall GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters)=0; // 9
    virtual HRESULT __stdcall SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap)=0; // 10
    virtual void __stdcall SetCursorPosition(int X,int Y,DWORD Flags)=0; // 11
    virtual BOOL __stdcall ShowCursor(BOOL bShow)=0; // 12
    virtual HRESULT __stdcall CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain)=0; // 13
    virtual HRESULT __stdcall GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)=0; // 14
    virtual UINT __stdcall GetNumberOfSwapChains()=0; // 15
    virtual HRESULT __stdcall Reset(D3DPRESENT_PARAMETERS* pPresentationParameters)=0; // 16
    virtual HRESULT __stdcall Present(const RECT* pSourceRect,const RECT* pDestRect,HWND hDestWindowOverride,const RGNDATA* pDirtyRegion)=0; // 17
    virtual HRESULT __stdcall GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer)=0; // 18
    virtual HRESULT __stdcall GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus)=0; // 19
    virtual HRESULT __stdcall SetDialogBoxMode(BOOL bEnableDialogs)=0; // 20
    virtual void __stdcall SetGammaRamp(UINT iSwapChain,DWORD Flags,const D3DGAMMARAMP* pRamp)=0; // 21
    virtual void __stdcall GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp)=0; // 22
    virtual HRESULT __stdcall CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle)=0; // 23
    virtual HRESULT __stdcall CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle)=0; // 24
    virtual HRESULT __stdcall CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle)=0; // 25
    virtual HRESULT __stdcall CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle)=0; // 26
    virtual HRESULT __stdcall CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle)=0; // 27
    virtual HRESULT __stdcall CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0; // 28
    virtual HRESULT __stdcall CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0; // 29
    virtual HRESULT __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface,const RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,const POINT* pDestPoint)=0; // 30
    virtual HRESULT __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture)=0; // 31
    virtual HRESULT __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface)=0; // 32
    virtual HRESULT __stdcall GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface)=0; // 33
    virtual HRESULT __stdcall StretchRect(IDirect3DSurface9* pSourceSurface,const RECT* pSourceRect,IDirect3DSurface9* pDestSurface,const RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter)=0; // 34
    virtual HRESULT __stdcall ColorFill(IDirect3DSurface9* pSurface,const RECT* pRect,D3DCOLOR color)=0; // 35
    virtual HRESULT __stdcall CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle)=0; // 36
    virtual HRESULT __stdcall SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget)=0; // 37
    virtual HRESULT __stdcall GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget)=0; // 38
    virtual HRESULT __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil)=0; // 39
    virtual HRESULT __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface)=0; // 40
    virtual HRESULT __stdcall BeginScene()=0; // 41
    virtual HRESULT __stdcall EndScene()=0; // 42
    virtual HRESULT __stdcall Clear(DWORD Count,const D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil)=0; // 43
    virtual HRESULT __stdcall SetTransform(D3DTRANSFORMSTATETYPE State,const D3DMATRIX* pMatrix)=0; // 44
    virtual HRESULT __stdcall GetTransform(D3DTRANSFORMSTATETYPE State,D3DMATRIX* pMatrix)=0; // 45
    virtual HRESULT __stdcall MultiplyTransform(D3DTRANSFORMSTATETYPE,const D3DMATRIX*)=0; // 46
    virtual HRESULT __stdcall SetViewport(const D3DVIEWPORT9* pViewport)=0; // 47
    virtual HRESULT __stdcall GetViewport(D3DVIEWPORT9* pViewport)=0; // 48
    virtual HRESULT __stdcall SetMaterial(const D3DMATERIAL9* pMaterial)=0; // 49
    virtual HRESULT __stdcall GetMaterial(D3DMATERIAL9* pMaterial)=0; // 50
    virtual HRESULT __stdcall SetLight(DWORD Index,const D3DLIGHT9*)=0; // 51
    virtual HRESULT __stdcall GetLight(DWORD Index,D3DLIGHT9*)=0; // 52
    virtual HRESULT __stdcall LightEnable(DWORD Index,BOOL Enable)=0; // 53
    virtual HRESULT __stdcall GetLightEnable(DWORD Index,BOOL* pEnable)=0; // 54
    virtual HRESULT __stdcall SetClipPlane(DWORD Index,const float* pPlane)=0; // 55
    virtual HRESULT __stdcall GetClipPlane(DWORD Index,float* pPlane)=0; // 56
    virtual HRESULT __stdcall SetRenderState(D3DRENDERSTATETYPE State,DWORD Value)=0; // 57
    virtual HRESULT __stdcall GetRenderState(D3DRENDERSTATETYPE State,DWORD* pValue)=0; // 58
    virtual HRESULT __stdcall CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB)=0; // 59
    virtual HRESULT __stdcall BeginStateBlock()=0; // 60
    virtual HRESULT __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB)=0; // 61
    virtual HRESULT __stdcall SetClipStatus(const D3DCLIPSTATUS9* pClipStatus)=0; // 62
    virtual HRESULT __stdcall GetClipStatus(D3DCLIPSTATUS9* pClipStatus)=0; // 63
    virtual HRESULT __stdcall GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture)=0; // 64
    virtual HRESULT __stdcall SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture)=0; // 65
    virtual HRESULT __stdcall GetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD* pValue)=0; // 66
    virtual HRESULT __stdcall SetTextureStageState(DWORD Stage,D3DTEXTURESTAGESTATETYPE Type,DWORD Value)=0; // 67
    virtual HRESULT __stdcall GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue)=0; // 68
    virtual HRESULT __stdcall SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value)=0; // 69
    virtual HRESULT __stdcall ValidateDevice(DWORD* pNumPasses)=0; // 70
    virtual HRESULT __stdcall SetPaletteEntries(UINT PaletteNumber,const PALETTEENTRY* pEntries)=0; // 71
    virtual HRESULT __stdcall GetPaletteEntries(UINT PaletteNumber,PALETTEENTRY* pEntries)=0; // 72
    virtual HRESULT __stdcall SetCurrentTexturePalette(UINT PaletteNumber)=0; // 73
    virtual HRESULT __stdcall GetCurrentTexturePalette(UINT *PaletteNumber)=0; // 74
    virtual HRESULT __stdcall SetScissorRect(const RECT* pRect)=0; // 75
    virtual HRESULT __stdcall GetScissorRect(RECT* pRect)=0; // 76
    virtual HRESULT __stdcall SetSoftwareVertexProcessing(BOOL bSoftware)=0; // 77
    virtual BOOL __stdcall GetSoftwareVertexProcessing()=0; // 78
    virtual HRESULT __stdcall SetNPatchMode(float nSegments)=0; // 79
    virtual float __stdcall GetNPatchMode()=0; // 80
    virtual HRESULT __stdcall DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)=0; // 81
    virtual HRESULT __stdcall DrawIndexedPrimitive(D3DPRIMITIVETYPE,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)=0; // 82
    virtual HRESULT __stdcall DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,const void* pVertexStreamZeroData,UINT VertexStreamZeroStride)=0; // 83
    virtual HRESULT __stdcall DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,const void* pIndexData,D3DFORMAT IndexDataFormat,const void* pVertexStreamZeroData,UINT VertexStreamZeroStride)=0; // 84
    virtual HRESULT __stdcall ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags)=0; // 85
    virtual HRESULT __stdcall CreateVertexDeclaration(const D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl)=0; // 86
    virtual HRESULT __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl)=0; // 87
    virtual HRESULT __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl)=0; // 88
    virtual HRESULT __stdcall SetFVF(DWORD FVF)=0; // 89
    virtual HRESULT __stdcall GetFVF(DWORD* pFVF)=0; // 90
    virtual HRESULT __stdcall CreateVertexShader(const DWORD* pFunction,IDirect3DVertexShader9** ppShader)=0; // 91
    virtual HRESULT __stdcall SetVertexShader(IDirect3DVertexShader9* pShader)=0; // 92
    virtual HRESULT __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader)=0; // 93
    virtual HRESULT __stdcall SetVertexShaderConstantF(UINT StartRegister,const float* pConstantData,UINT Vector4fCount)=0; // 94
    virtual HRESULT __stdcall GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)=0; // 95
    virtual HRESULT __stdcall SetVertexShaderConstantI(UINT StartRegister,const int* pConstantData,UINT Vector4iCount)=0; // 96
    virtual HRESULT __stdcall GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)=0; // 97
    virtual HRESULT __stdcall SetVertexShaderConstantB(UINT StartRegister,const BOOL* pConstantData,UINT  BoolCount)=0; // 98
    virtual HRESULT __stdcall GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)=0; // 99
    virtual HRESULT __stdcall SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride)=0; // 100
    virtual HRESULT __stdcall GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride)=0; // 101
    virtual HRESULT __stdcall SetStreamSourceFreq(UINT StreamNumber,UINT Setting)=0; // 102
    virtual HRESULT __stdcall GetStreamSourceFreq(UINT StreamNumber,UINT* pSetting)=0; // 103
    virtual HRESULT __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData)=0; // 104
    virtual HRESULT __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData)=0; // 105
    virtual HRESULT __stdcall CreatePixelShader(const DWORD* pFunction,IDirect3DPixelShader9** ppShader)=0; // 106
    virtual HRESULT __stdcall SetPixelShader(IDirect3DPixelShader9* pShader)=0; // 107
    virtual HRESULT __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader)=0; // 108
    virtual HRESULT __stdcall SetPixelShaderConstantF(UINT StartRegister,const float* pConstantData,UINT Vector4fCount)=0; // 109
    virtual HRESULT __stdcall GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount)=0; // 110
    virtual HRESULT __stdcall SetPixelShaderConstantI(UINT StartRegister,const int* pConstantData,UINT Vector4iCount)=0; // 111
    virtual HRESULT __stdcall GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)=0; // 112
    virtual HRESULT __stdcall SetPixelShaderConstantB(UINT StartRegister,const BOOL* pConstantData,UINT  BoolCount)=0; // 113
    virtual HRESULT __stdcall GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)=0; // 114
    virtual HRESULT __stdcall DrawRectPatch(UINT Handle,const float* pNumSegs,const D3DRECTPATCH_INFO* pRectPatchInfo)=0; // 115
    virtual HRESULT __stdcall DrawTriPatch(UINT Handle,const float* pNumSegs,const D3DTRIPATCH_INFO* pTriPatchInfo)=0; // 116
    virtual HRESULT __stdcall DeletePatch(UINT Handle)=0; // 117
    virtual HRESULT __stdcall CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery)=0; // 118
};
#endif
