#ifndef FABLE_ENGINE_IDIRECT3DDEVICE9_H
#define FABLE_ENGINE_IDIRECT3DDEVICE9_H
// IDirect3DDevice9 (DirectX 9 SDK d3d9.h, DECLARE_INTERFACE_(IDirect3DDevice9, IUnknown)): the COM device interface the engine
// calls through its vtable (Fable.exe imports d3d9.dll; Ego_r.pdb keys it `U`, ?Initialise@CShaderRenderManager@@QAEXPAVCRenderManager@@PAUIDirect3DDevice9@@@Z).
// Methods in SDK order and with SDK names (slot N at +4N; e.g. SetTexture 65 +0x104, SetStreamSource 100 +0x190, SetIndices 104
// +0x1A0, DrawIndexedPrimitive 82 +0x148). d3d9.h is not on the verifier include path: parameter types are the SDK typedefs'
// underlying types (DWORD/D3DCOLOR = unsigned long, UINT = unsigned int, BOOL = int, the D3D enums = unsigned long) and the
// SDK structures (by their struct tags, so d3d9types.h / Direct3D9Types.h can coexist) and interfaces are forward declarations. Calls go through the vtable, so no symbol is bound.
struct _D3DCAPS9;
struct _D3DCLIPSTATUS9;
struct _D3DDEVICE_CREATION_PARAMETERS;
struct _D3DDISPLAYMODE;
struct _D3DGAMMARAMP;
struct _D3DLIGHT9;
struct _D3DMATERIAL9;
struct _D3DMATRIX;
struct _D3DPRESENT_PARAMETERS_;
struct _D3DRASTER_STATUS;
struct _D3DRECT;
struct _D3DRECTPATCH_INFO;
struct _D3DTRIPATCH_INFO;
struct _D3DVERTEXELEMENT9;
struct _D3DVIEWPORT9;
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
struct tagPALETTEENTRY;
struct tagPOINT;
struct tagRECT;
struct _RGNDATA;
struct HWND__;
struct _GUID;
struct IDirect3DDevice9 {
    virtual long __stdcall QueryInterface(const _GUID& riid, void** ppvObj) = 0;   // slot 0 (+0x0)
    virtual unsigned long __stdcall AddRef() = 0;   // slot 1 (+0x4)
    virtual unsigned long __stdcall Release() = 0;   // slot 2 (+0x8)
    virtual long __stdcall TestCooperativeLevel() = 0;   // slot 3 (+0xc)
    virtual unsigned int __stdcall GetAvailableTextureMem() = 0;   // slot 4 (+0x10)
    virtual long __stdcall EvictManagedResources() = 0;   // slot 5 (+0x14)
    virtual long __stdcall GetDirect3D(IDirect3D9** ppD3D9) = 0;   // slot 6 (+0x18)
    virtual long __stdcall GetDeviceCaps(_D3DCAPS9* pCaps) = 0;   // slot 7 (+0x1c)
    virtual long __stdcall GetDisplayMode(unsigned int iSwapChain, _D3DDISPLAYMODE* pMode) = 0;   // slot 8 (+0x20)
    virtual long __stdcall GetCreationParameters(_D3DDEVICE_CREATION_PARAMETERS* pParameters) = 0;   // slot 9 (+0x24)
    virtual long __stdcall SetCursorProperties(unsigned int XHotSpot, unsigned int YHotSpot, IDirect3DSurface9* pCursorBitmap) = 0;   // slot 10 (+0x28)
    virtual void __stdcall SetCursorPosition(int X, int Y, unsigned long Flags) = 0;   // slot 11 (+0x2c)
    virtual int __stdcall ShowCursor(int bShow) = 0;   // slot 12 (+0x30)
    virtual long __stdcall CreateAdditionalSwapChain(_D3DPRESENT_PARAMETERS_* pPresentationParameters, IDirect3DSwapChain9** pSwapChain) = 0;   // slot 13 (+0x34)
    virtual long __stdcall GetSwapChain(unsigned int iSwapChain, IDirect3DSwapChain9** pSwapChain) = 0;   // slot 14 (+0x38)
    virtual unsigned int __stdcall GetNumberOfSwapChains() = 0;   // slot 15 (+0x3c)
    virtual long __stdcall Reset(_D3DPRESENT_PARAMETERS_* pPresentationParameters) = 0;   // slot 16 (+0x40)
    virtual long __stdcall Present(const tagRECT* pSourceRect, const tagRECT* pDestRect, HWND__* hDestWindowOverride, const _RGNDATA* pDirtyRegion) = 0;   // slot 17 (+0x44)
    virtual long __stdcall GetBackBuffer(unsigned int iSwapChain, unsigned int iBackBuffer, unsigned long Type, IDirect3DSurface9** ppBackBuffer) = 0;   // slot 18 (+0x48)
    virtual long __stdcall GetRasterStatus(unsigned int iSwapChain, _D3DRASTER_STATUS* pRasterStatus) = 0;   // slot 19 (+0x4c)
    virtual long __stdcall SetDialogBoxMode(int bEnableDialogs) = 0;   // slot 20 (+0x50)
    virtual void __stdcall SetGammaRamp(unsigned int iSwapChain, unsigned long Flags, const _D3DGAMMARAMP* pRamp) = 0;   // slot 21 (+0x54)
    virtual void __stdcall GetGammaRamp(unsigned int iSwapChain, _D3DGAMMARAMP* pRamp) = 0;   // slot 22 (+0x58)
    virtual long __stdcall CreateTexture(unsigned int Width, unsigned int Height, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DTexture9** ppTexture, void** pSharedHandle) = 0;   // slot 23 (+0x5c)
    virtual long __stdcall CreateVolumeTexture(unsigned int Width, unsigned int Height, unsigned int Depth, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DVolumeTexture9** ppVolumeTexture, void** pSharedHandle) = 0;   // slot 24 (+0x60)
    virtual long __stdcall CreateCubeTexture(unsigned int EdgeLength, unsigned int Levels, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DCubeTexture9** ppCubeTexture, void** pSharedHandle) = 0;   // slot 25 (+0x64)
    virtual long __stdcall CreateVertexBuffer(unsigned int Length, unsigned long Usage, unsigned long FVF, unsigned long Pool, IDirect3DVertexBuffer9** ppVertexBuffer, void** pSharedHandle) = 0;   // slot 26 (+0x68)
    virtual long __stdcall CreateIndexBuffer(unsigned int Length, unsigned long Usage, unsigned long Format, unsigned long Pool, IDirect3DIndexBuffer9** ppIndexBuffer, void** pSharedHandle) = 0;   // slot 27 (+0x6c)
    virtual long __stdcall CreateRenderTarget(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long MultiSample, unsigned long MultisampleQuality, int Lockable, IDirect3DSurface9** ppSurface, void** pSharedHandle) = 0;   // slot 28 (+0x70)
    virtual long __stdcall CreateDepthStencilSurface(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long MultiSample, unsigned long MultisampleQuality, int Discard, IDirect3DSurface9** ppSurface, void** pSharedHandle) = 0;   // slot 29 (+0x74)
    virtual long __stdcall UpdateSurface(IDirect3DSurface9* pSourceSurface, const tagRECT* pSourceRect, IDirect3DSurface9* pDestinationSurface, const tagPOINT* pDestPoint) = 0;   // slot 30 (+0x78)
    virtual long __stdcall UpdateTexture(IDirect3DBaseTexture9* pSourceTexture, IDirect3DBaseTexture9* pDestinationTexture) = 0;   // slot 31 (+0x7c)
    virtual long __stdcall GetRenderTargetData(IDirect3DSurface9* pRenderTarget, IDirect3DSurface9* pDestSurface) = 0;   // slot 32 (+0x80)
    virtual long __stdcall GetFrontBufferData(unsigned int iSwapChain, IDirect3DSurface9* pDestSurface) = 0;   // slot 33 (+0x84)
    virtual long __stdcall StretchRect(IDirect3DSurface9* pSourceSurface, const tagRECT* pSourceRect, IDirect3DSurface9* pDestSurface, const tagRECT* pDestRect, unsigned long Filter) = 0;   // slot 34 (+0x88)
    virtual long __stdcall ColorFill(IDirect3DSurface9* pSurface, const tagRECT* pRect, unsigned long color) = 0;   // slot 35 (+0x8c)
    virtual long __stdcall CreateOffscreenPlainSurface(unsigned int Width, unsigned int Height, unsigned long Format, unsigned long Pool, IDirect3DSurface9** ppSurface, void** pSharedHandle) = 0;   // slot 36 (+0x90)
    virtual long __stdcall SetRenderTarget(unsigned long RenderTargetIndex, IDirect3DSurface9* pRenderTarget) = 0;   // slot 37 (+0x94)
    virtual long __stdcall GetRenderTarget(unsigned long RenderTargetIndex, IDirect3DSurface9** ppRenderTarget) = 0;   // slot 38 (+0x98)
    virtual long __stdcall SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) = 0;   // slot 39 (+0x9c)
    virtual long __stdcall GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface) = 0;   // slot 40 (+0xa0)
    virtual long __stdcall BeginScene() = 0;   // slot 41 (+0xa4)
    virtual long __stdcall EndScene() = 0;   // slot 42 (+0xa8)
    virtual long __stdcall Clear(unsigned long Count, const _D3DRECT* pRects, unsigned long Flags, unsigned long Color, float Z, unsigned long Stencil) = 0;   // slot 43 (+0xac)
    virtual long __stdcall SetTransform(unsigned long State, const _D3DMATRIX* pMatrix) = 0;   // slot 44 (+0xb0)
    virtual long __stdcall GetTransform(unsigned long State, _D3DMATRIX* pMatrix) = 0;   // slot 45 (+0xb4)
    virtual long __stdcall MultiplyTransform(unsigned long, const _D3DMATRIX*) = 0;   // slot 46 (+0xb8)
    virtual long __stdcall SetViewport(const _D3DVIEWPORT9* pViewport) = 0;   // slot 47 (+0xbc)
    virtual long __stdcall GetViewport(_D3DVIEWPORT9* pViewport) = 0;   // slot 48 (+0xc0)
    virtual long __stdcall SetMaterial(const _D3DMATERIAL9* pMaterial) = 0;   // slot 49 (+0xc4)
    virtual long __stdcall GetMaterial(_D3DMATERIAL9* pMaterial) = 0;   // slot 50 (+0xc8)
    virtual long __stdcall SetLight(unsigned long Index, const _D3DLIGHT9*) = 0;   // slot 51 (+0xcc)
    virtual long __stdcall GetLight(unsigned long Index, _D3DLIGHT9*) = 0;   // slot 52 (+0xd0)
    virtual long __stdcall LightEnable(unsigned long Index, int Enable) = 0;   // slot 53 (+0xd4)
    virtual long __stdcall GetLightEnable(unsigned long Index, int* pEnable) = 0;   // slot 54 (+0xd8)
    virtual long __stdcall SetClipPlane(unsigned long Index, const float* pPlane) = 0;   // slot 55 (+0xdc)
    virtual long __stdcall GetClipPlane(unsigned long Index, float* pPlane) = 0;   // slot 56 (+0xe0)
    virtual long __stdcall SetRenderState(unsigned long State, unsigned long Value) = 0;   // slot 57 (+0xe4)
    virtual long __stdcall GetRenderState(unsigned long State, unsigned long* pValue) = 0;   // slot 58 (+0xe8)
    virtual long __stdcall CreateStateBlock(unsigned long Type, IDirect3DStateBlock9** ppSB) = 0;   // slot 59 (+0xec)
    virtual long __stdcall BeginStateBlock() = 0;   // slot 60 (+0xf0)
    virtual long __stdcall EndStateBlock(IDirect3DStateBlock9** ppSB) = 0;   // slot 61 (+0xf4)
    virtual long __stdcall SetClipStatus(const _D3DCLIPSTATUS9* pClipStatus) = 0;   // slot 62 (+0xf8)
    virtual long __stdcall GetClipStatus(_D3DCLIPSTATUS9* pClipStatus) = 0;   // slot 63 (+0xfc)
    virtual long __stdcall GetTexture(unsigned long Stage, IDirect3DBaseTexture9** ppTexture) = 0;   // slot 64 (+0x100)
    virtual long __stdcall SetTexture(unsigned long Stage, IDirect3DBaseTexture9* pTexture) = 0;   // slot 65 (+0x104)
    virtual long __stdcall GetTextureStageState(unsigned long Stage, unsigned long Type, unsigned long* pValue) = 0;   // slot 66 (+0x108)
    virtual long __stdcall SetTextureStageState(unsigned long Stage, unsigned long Type, unsigned long Value) = 0;   // slot 67 (+0x10c)
    virtual long __stdcall GetSamplerState(unsigned long Sampler, unsigned long Type, unsigned long* pValue) = 0;   // slot 68 (+0x110)
    virtual long __stdcall SetSamplerState(unsigned long Sampler, unsigned long Type, unsigned long Value) = 0;   // slot 69 (+0x114)
    virtual long __stdcall ValidateDevice(unsigned long* pNumPasses) = 0;   // slot 70 (+0x118)
    virtual long __stdcall SetPaletteEntries(unsigned int PaletteNumber, const tagPALETTEENTRY* pEntries) = 0;   // slot 71 (+0x11c)
    virtual long __stdcall GetPaletteEntries(unsigned int PaletteNumber, tagPALETTEENTRY* pEntries) = 0;   // slot 72 (+0x120)
    virtual long __stdcall SetCurrentTexturePalette(unsigned int PaletteNumber) = 0;   // slot 73 (+0x124)
    virtual long __stdcall GetCurrentTexturePalette(unsigned int* PaletteNumber) = 0;   // slot 74 (+0x128)
    virtual long __stdcall SetScissorRect(const tagRECT* pRect) = 0;   // slot 75 (+0x12c)
    virtual long __stdcall GetScissorRect(tagRECT* pRect) = 0;   // slot 76 (+0x130)
    virtual long __stdcall SetSoftwareVertexProcessing(int bSoftware) = 0;   // slot 77 (+0x134)
    virtual int __stdcall GetSoftwareVertexProcessing() = 0;   // slot 78 (+0x138)
    virtual long __stdcall SetNPatchMode(float nSegments) = 0;   // slot 79 (+0x13c)
    virtual float __stdcall GetNPatchMode() = 0;   // slot 80 (+0x140)
    virtual long __stdcall DrawPrimitive(unsigned long PrimitiveType, unsigned int StartVertex, unsigned int PrimitiveCount) = 0;   // slot 81 (+0x144)
    virtual long __stdcall DrawIndexedPrimitive(unsigned long PrimitiveType, int BaseVertexIndex, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int startIndex, unsigned int primCount) = 0;   // slot 82 (+0x148)
    virtual long __stdcall DrawPrimitiveUP(unsigned long PrimitiveType, unsigned int PrimitiveCount, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) = 0;   // slot 83 (+0x14c)
    virtual long __stdcall DrawIndexedPrimitiveUP(unsigned long PrimitiveType, unsigned int MinVertexIndex, unsigned int NumVertices, unsigned int PrimitiveCount, const void* pIndexData, unsigned long IndexDataFormat, const void* pVertexStreamZeroData, unsigned int VertexStreamZeroStride) = 0;   // slot 84 (+0x150)
    virtual long __stdcall ProcessVertices(unsigned int SrcStartIndex, unsigned int DestIndex, unsigned int VertexCount, IDirect3DVertexBuffer9* pDestBuffer, IDirect3DVertexDeclaration9* pVertexDecl, unsigned long Flags) = 0;   // slot 85 (+0x154)
    virtual long __stdcall CreateVertexDeclaration(const _D3DVERTEXELEMENT9* pVertexElements, IDirect3DVertexDeclaration9** ppDecl) = 0;   // slot 86 (+0x158)
    virtual long __stdcall SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) = 0;   // slot 87 (+0x15c)
    virtual long __stdcall GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) = 0;   // slot 88 (+0x160)
    virtual long __stdcall SetFVF(unsigned long FVF) = 0;   // slot 89 (+0x164)
    virtual long __stdcall GetFVF(unsigned long* pFVF) = 0;   // slot 90 (+0x168)
    virtual long __stdcall CreateVertexShader(const unsigned long* pFunction, IDirect3DVertexShader9** ppShader) = 0;   // slot 91 (+0x16c)
    virtual long __stdcall SetVertexShader(IDirect3DVertexShader9* pShader) = 0;   // slot 92 (+0x170)
    virtual long __stdcall GetVertexShader(IDirect3DVertexShader9** ppShader) = 0;   // slot 93 (+0x174)
    virtual long __stdcall SetVertexShaderConstantF(unsigned int StartRegister, const float* pConstantData, unsigned int Vector4fCount) = 0;   // slot 94 (+0x178)
    virtual long __stdcall GetVertexShaderConstantF(unsigned int StartRegister, float* pConstantData, unsigned int Vector4fCount) = 0;   // slot 95 (+0x17c)
    virtual long __stdcall SetVertexShaderConstantI(unsigned int StartRegister, const int* pConstantData, unsigned int Vector4iCount) = 0;   // slot 96 (+0x180)
    virtual long __stdcall GetVertexShaderConstantI(unsigned int StartRegister, int* pConstantData, unsigned int Vector4iCount) = 0;   // slot 97 (+0x184)
    virtual long __stdcall SetVertexShaderConstantB(unsigned int StartRegister, const int* pConstantData, unsigned int BoolCount) = 0;   // slot 98 (+0x188)
    virtual long __stdcall GetVertexShaderConstantB(unsigned int StartRegister, int* pConstantData, unsigned int BoolCount) = 0;   // slot 99 (+0x18c)
    virtual long __stdcall SetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9* pStreamData, unsigned int OffsetInBytes, unsigned int Stride) = 0;   // slot 100 (+0x190)
    virtual long __stdcall GetStreamSource(unsigned int StreamNumber, IDirect3DVertexBuffer9** ppStreamData, unsigned int* pOffsetInBytes, unsigned int* pStride) = 0;   // slot 101 (+0x194)
    virtual long __stdcall SetStreamSourceFreq(unsigned int StreamNumber, unsigned int Setting) = 0;   // slot 102 (+0x198)
    virtual long __stdcall GetStreamSourceFreq(unsigned int StreamNumber, unsigned int* pSetting) = 0;   // slot 103 (+0x19c)
    virtual long __stdcall SetIndices(IDirect3DIndexBuffer9* pIndexData) = 0;   // slot 104 (+0x1a0)
    virtual long __stdcall GetIndices(IDirect3DIndexBuffer9** ppIndexData) = 0;   // slot 105 (+0x1a4)
    virtual long __stdcall CreatePixelShader(const unsigned long* pFunction, IDirect3DPixelShader9** ppShader) = 0;   // slot 106 (+0x1a8)
    virtual long __stdcall SetPixelShader(IDirect3DPixelShader9* pShader) = 0;   // slot 107 (+0x1ac)
    virtual long __stdcall GetPixelShader(IDirect3DPixelShader9** ppShader) = 0;   // slot 108 (+0x1b0)
    virtual long __stdcall SetPixelShaderConstantF(unsigned int StartRegister, const float* pConstantData, unsigned int Vector4fCount) = 0;   // slot 109 (+0x1b4)
    virtual long __stdcall GetPixelShaderConstantF(unsigned int StartRegister, float* pConstantData, unsigned int Vector4fCount) = 0;   // slot 110 (+0x1b8)
    virtual long __stdcall SetPixelShaderConstantI(unsigned int StartRegister, const int* pConstantData, unsigned int Vector4iCount) = 0;   // slot 111 (+0x1bc)
    virtual long __stdcall GetPixelShaderConstantI(unsigned int StartRegister, int* pConstantData, unsigned int Vector4iCount) = 0;   // slot 112 (+0x1c0)
    virtual long __stdcall SetPixelShaderConstantB(unsigned int StartRegister, const int* pConstantData, unsigned int BoolCount) = 0;   // slot 113 (+0x1c4)
    virtual long __stdcall GetPixelShaderConstantB(unsigned int StartRegister, int* pConstantData, unsigned int BoolCount) = 0;   // slot 114 (+0x1c8)
    virtual long __stdcall DrawRectPatch(unsigned int Handle, const float* pNumSegs, const _D3DRECTPATCH_INFO* pRectPatchInfo) = 0;   // slot 115 (+0x1cc)
    virtual long __stdcall DrawTriPatch(unsigned int Handle, const float* pNumSegs, const _D3DTRIPATCH_INFO* pTriPatchInfo) = 0;   // slot 116 (+0x1d0)
    virtual long __stdcall DeletePatch(unsigned int Handle) = 0;   // slot 117 (+0x1d4)
    virtual long __stdcall CreateQuery(unsigned long Type, IDirect3DQuery9** ppQuery) = 0;   // slot 118 (+0x1d8)
};

#endif // FABLE_ENGINE_IDIRECT3DDEVICE9_H
