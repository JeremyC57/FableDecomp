// CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine&) -- draw one line as a camera-facing textured quad
// Grade: DIFFER(3323v3416) -- 1006 difflines: VC7.1 sinks the both-behind-near-plane early return (inline ~CShaderEnableSaver) to the function end where retail keeps it fall-through (0x00BC8C1B), retail homes this at [esp+60h] (ours keeps it in ebp), slot layout of the camera-space points / corners, the CRenderStateManager bookmark test/or flip (chunk 792: a whole-function effect), the texture-frame fetch's register use
// Next idea: pass this->VertexShader through a local pointer taken at entry (retail stores ecx at once), split the blend-state switch tail so cases 3 and 4 share the +0x2930 store (retail tail-merges it with esi = value), and try TransformPoint as CMatrix3x4::TransformPoint(const C3DVector&, C3DVector&) into fresh locals; the WorldToCamera points use the parenthesised TransformPoint and the corners the SUMS form (Y/Z left to right) -- fold into CMatrix3x4.h opt-ins at landing
// Leaf: ?RenderLine@CEnginePrimitiveManagerLines@@QAEXABVCEngineInternalPrimitiveLine@@@Z
#include "engine/C3DVector.h"
class C4DVector { public: float X, Y, Z, W; C4DVector() { } C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { } };
class CTexture;
enum ETextureType { TEXTURE_TYPE_0 = 0 };
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector& Normalise();                             // 0x00A14540 ?Normalise@C2DVector@@QAEAAV1@XZ
};
class CRGBColour { public: unsigned char B, G, R, A; };
void __cdecl operator delete(void* block);             // 0x00BFE9BC
void* __cdecl operator new(unsigned int size);         // 0x00BFEA1A

struct CTrackedRenderState {                       // 0x14 bytes
    long          m_stateId;                       // +0x00
    unsigned long m_value;                         // +0x04 requested value
    unsigned long m_captureMask;                   // +0x08
    long          m_appliedValue;                  // +0x0C
    bool          m_queued;                        // +0x10
    __forceinline void Set(unsigned long value);
};
struct SCapturedRenderState {
    CTrackedRenderState* m_pState;                 // +0x00
    unsigned long        m_priorValue;             // +0x04
};
struct CRenderManagerCoreView {                    // the render manager core @ 0x01436E18 / 0x013CB508
    unsigned char        _pad0000[0x8];
    SCapturedRenderState m_captured[0x400];        // +0x0008
    CTrackedRenderState* m_dirty[0x200];           // +0x2008
    long                 m_capturedCount;          // +0x2808
    long                 m_dirtyCount;             // +0x280C
    long                 m_unknown2810;            // +0x2810
    unsigned long        m_captureMask;            // +0x2814
    unsigned char        _pad2818[0x28b8 - 0x2818];
    CTrackedRenderState  m_state28b8;              // +0x28B8
    unsigned char        _pad28cc[0x291c - 0x28cc];
    CTrackedRenderState  m_state291c;              // +0x291C
    CTrackedRenderState  m_state2930;              // +0x2930
    unsigned char        _pad2944[0x3a24 - 0x2944];
    unsigned char        m_captureBlockMarker[4];  // +0x3A24

    __forceinline void ChangeState(CTrackedRenderState& state, unsigned long value)
    {
        if ((m_captureMask & state.m_captureMask) == 0) {
            state.m_captureMask |= m_captureMask;
            m_captured[m_capturedCount].m_pState = &state;
            m_captured[m_capturedCount].m_priorValue = state.m_value;
            ++m_capturedCount;
        }
        state.m_value = value;
        if (!state.m_queued) {
            state.m_queued = true;
            m_dirty[m_dirtyCount] = &state;
            ++m_dirtyCount;
        }
    }
    __forceinline void OpenCaptureBlock()
    {
        m_captured[m_capturedCount++].m_pState = (CTrackedRenderState*)m_captureBlockMarker;
        m_captureMask <<= 1;
    }
    void RestoreCaptureBlock();                    // 0x00A05840
    void DrawAsIndexedTris(CTexture** textures, unsigned long textureCount, const void* vertices, long stride,
                           long vertexCount, const void* indices, long triangleCount);   // 0x00A0AEA0
};
extern CRenderManagerCoreView* g_pRenderStateManager;  // 0x013CB508
extern CRenderManagerCoreView* g_pRenderManagerCore;   // 0x01436E18

__forceinline void CTrackedRenderState::Set(unsigned long value)
{
    if (m_value != value)
        g_pRenderManagerCore->ChangeState(*this, value);
}

extern unsigned long g_renderStateDefault_01396f6c;    // 0x01396F6C
extern unsigned long g_renderStateDefault_01396f74;    // 0x01396F74
extern unsigned long g_renderStateDefault_01396f78;    // 0x01396F78
extern unsigned long g_renderStateDefault_01396f7c;    // 0x01396F7C
extern const unsigned short g_LineQuadIndices_0139bac8[6];   // 0x0139BAC8

static __forceinline void SetState(unsigned long value, CTrackedRenderState& state, CRenderManagerCoreView* core)
{
    if (state.m_value != value)
        core->ChangeState(state, value);
}
struct CShaderEnableState {                             // the static shader manager object @ 0x013BC470
    unsigned char _pad000[0x390];
    bool          VertexShadersEnabled;                 // +0x390
    bool          PixelShadersEnabled;                  // +0x391
    unsigned char _pad392[2];
    long          _pad394;
    void EnableVertexShaders();                         // 0x00987FE0
    void DisableVertexShaders();                        // 0x009880E0
    void EnablePixelShaders();                          // 0x00988110
    void DisablePixelShaders();                         // 0x00988190
    __forceinline void SetVertexShadersEnabled(bool enable)
    {
        if (enable != VertexShadersEnabled) {
            if (enable)
                EnableVertexShaders();
            else
                DisableVertexShaders();
        }
    }
    __forceinline void SetPixelShadersEnabled(bool enable)
    {
        if (enable != PixelShadersEnabled) {
            if (enable)
                EnablePixelShaders();
            else
                DisablePixelShaders();
        }
    }
};
extern CShaderEnableState g_ShaderRenderManager;        // 0x013BC470

template <class T> class CArray { public:
    T* First;
    T* Last;
    T* End;
    unsigned long Size() const { return Last - First; }
    const T* GetData() const { return Size() > 0 ? First : (const T*)this; }
};
class CShaderConstantRange { public:
    void* _vptr;                                        // +0
    long Offset;                                        // +4
    unsigned long Count;                                // +8
};
class CVertexShaderConstantLayout { public:
    void* _vptr;                                        // +0x00
    CShaderConstantRange Reserved;                      // +0x04
    unsigned char _pad10[0x130];                        // +0x10 the other ranges
    CArray<C4DVector> ReservedPresets;                  // +0x140
};
class CVertexShader;
class CPixelShader;

class CShaderRenderManager {
public:
    void ApplyVertexShader(CVertexShader* shader, bool force);                  // 0x00988020
    void ApplyPixelShader(CPixelShader* shader);                                // 0x00988140

    __forceinline void PrepareForRender(unsigned long flags)
    {
        if (UpdateFlags & 4)
            SetVSConstants(CurrentVSConstantLayout->Reserved.Offset, CurrentVSConstantLayout->ReservedPresets.GetData(),
                           CurrentVSConstantLayout->ReservedPresets.Size());
        if (UpdateFlags & 1)
            UpdatePixelShader();
        if (UpdateFlags & 2)
            UpdatePixelShaderConstants();
        if ((flags & 0x400) && (UpdateFlags & 0x20))
            UpdateBoneTransform();
        if (flags & 0x40) {
            if (UpdateFlags & 0x1000)
                UpdateWorldSpaceTextureTransform();
        } else if (flags & 0x80) {
            if (UpdateFlags & 0x800)
                UpdateObjectSpaceTextureTransform();
        }
        if (flags & 0x200) {
            if (UpdateFlags & 0x2000)
                UpdateWorldSpaceFogTransform();
        } else if (flags & 0x100) {
            if (UpdateFlags & 0x4000)
                UpdateObjectSpaceFogTransform();
        }
        if (UpdateFlags & 4)
            UpdateAmbient();
        if (UpdateFlags & 0x20000)
            UpdateFogColour();
        if (UpdateFlags & 0x400)
            UpdateLightGlobals();
        if (flags & 8) {
            if (UpdateFlags & 0x100)
                UpdateObjectSpaceLighting(flags);
        } else if (flags & 0x10) {
            if (UpdateFlags & 0x200)
                UpdateWorldSpaceLighting();
        }
        if ((flags & 0x20) && (UpdateFlags & 0x8000))
            UpdateLocalisedSphereMapping();
        if (!(flags & 1) && !WorldMatrixIsIdentity)
            ResetWorldTransform();
        if (UpdateFlags & 0x80)
            UpdateCombinedProjectionTransform();
        if ((flags & 4) && (UpdateFlags & 8))
            UpdateCameraPos();
    }

    void* _vptr;                                        // +0x000
    CVertexShaderConstantLayout* CurrentVSConstantLayout;   // +0x004
    unsigned char _pad008[0x1e0];                       // +0x008
    bool WorldMatrixIsIdentity;                         // +0x1e8
    unsigned char _pad1e9[0x1e3];                       // +0x1e9
    C4DVector* PSConstants;                             // +0x3cc
    unsigned char _pad3d0[8];                           // +0x3d0
    unsigned long UpdateFlags;                          // +0x3d8
    __forceinline void SetPSConstant(unsigned long index, const C4DVector& value) { PSConstants[index] = value; UpdateFlags |= 2; }

private:
    void SetVSConstants(long reg, const void* data, unsigned long count);   // 0x0098B930
    void UpdatePixelShader();                           // 0x00988A20
    void UpdatePixelShaderConstants();                  // 0x00989BF0
    void UpdateBoneTransform();                         // 0x0098A8E0
    void UpdateWorldSpaceTextureTransform();            // 0x009894B0
    void UpdateObjectSpaceTextureTransform();           // 0x00989230
    void UpdateWorldSpaceFogTransform();                // 0x00989640
    void UpdateObjectSpaceFogTransform();               // 0x00989540
    void UpdateAmbient();                               // 0x00989760
    void UpdateFogColour();                             // 0x009897C0
    void UpdateLightGlobals();                          // 0x0098A760
    void UpdateObjectSpaceLighting(unsigned long flags);   // 0x00989CB0
    void UpdateWorldSpaceLighting();                    // 0x0098A540
    void UpdateLocalisedSphereMapping();                // 0x00988E80
public:
    void ResetWorldTransform();                         // 0x00988290
private:
    void UpdateCombinedProjectionTransform();           // 0x00988A50
    void UpdateCameraPos();                             // 0x00989700
};
extern CShaderRenderManager* g_pShaderRenderManager;    // 0x01436E14
struct CShaderEnableSaver {                             // saves and forces the shader enables, restores them on exit
    bool m_vertexShaders;
    bool m_pixelShaders;
    __forceinline CShaderEnableSaver(bool vertexShaders, bool pixelShaders)
    {
        m_vertexShaders = g_ShaderRenderManager.VertexShadersEnabled;
        g_ShaderRenderManager.SetVertexShadersEnabled(vertexShaders);
        m_pixelShaders = g_ShaderRenderManager.PixelShadersEnabled;
        g_ShaderRenderManager.SetPixelShadersEnabled(pixelShaders);
    }
    __forceinline ~CShaderEnableSaver()
    {
        g_ShaderRenderManager.SetVertexShadersEnabled(m_vertexShaders);
        g_ShaderRenderManager.SetPixelShadersEnabled(m_pixelShaders);
    }
};

class CResourceList;
class CResource { public:                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointer count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
private:
    void ListRemove();                                 // 0x009FBFB0
public:
private:
    void ListInsert(CResource*a0, CResource*a1); // 0x009FBF90
public:
    __forceinline void Touch();
};
class CResourceList { public:                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
__forceinline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CCPInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CTexturePointer {                               // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo* Info;
    __forceinline ~CTexturePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CGraphicFrameData { public:                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CTexturePointer PTexture;                          // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture { public:                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
};
template <class T> struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
    T* P;
    __forceinline CFrameDataPointer& operator=(T* p)
    {
        if (P) {
            P->DecRef();
            P = 0;
        }
        P = p;
        return *this;
    }
    T* operator->() const { return P; }
    operator bool() const { return P != 0; }
};
class CGraphicInfo;
class CGraphicInfoFrame { public: class CFrameData; public:                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer<CFrameData> Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0 (Ego_r.pdb ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z, private)   // 0x009FF6B0
public:
};
class CGraphicInfoFrame::CFrameData : public CResource { public:                        // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
    CFrameData(const CGraphicInfoFrame* frame);              // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};   // Ego_r.pdb: nested in CGraphicInfoFrame (RTTI .?AVCFrameData@CGraphicInfoFrame@@), 0x44 bytes
typedef CGraphicInfoFrame::CFrameData CFrameData;   // the row's free code keeps the unqualified name


class CGraphicInfo { public:                                  // Ego_r.pdb, 0x2c bytes
    unsigned char      _pad00[0x1c];                   // +0x00 CGraphicHeader Header
    ETextureType       TextureType;                    // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
template<class T> struct SVector {                  // STLport std::vector<T> (start, finish, end of storage)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    __forceinline unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    __forceinline const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CGraphicDataBankFull : public CGraphicDataBank { public:   // the bank's graphic list (std::vector<CGraphicInfo>) and blank textures
    unsigned char      _pad01[0x1e0 - 0x198];
    SVector<CGraphicInfo> GraphicList;                 // +0x1e0
    unsigned char      _pad02[0x2b4 - 0x1ec];
    __forceinline unsigned long GetNoGraphics() const { return GraphicList.size(); }
    __forceinline const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
};

class CGraphicFrameRef { public:                       // a counted copy of a CGraphicFrame
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrameRef(const CGraphicFrame& frame) : FrameData(frame.FrameData) { if (FrameData) ++FrameData->ReferenceCount; }
    __forceinline ~CGraphicFrameRef() { FrameData->DecRef(); }
};
struct CEngineResourceManagerView {                    // CEngineResourceManager at 0x01436E98 (local view; header debt: engine/CEngineResourceManager.h)
    unsigned char _pad000[0x954];
    CGraphicDataBankFull* MainGraphicBank;             // +0x954 retail (Ego_r CEngineResourceManager::MainGraphicBank +0xA8C)
};
extern CEngineResourceManagerView* g_pResourceManager;  // 0x01436E98 (bound spelling, landed 0x00BE3660)
class CMatrix3x4 { public: float E11, E12, E13, E21, E22, E23, E31, E32, E33, E41, E42, E43; };
struct CEngineCameraView {                             // CEngineCamera at 0x01436EA0 (local view; header debt: engine/CEngineCamera.h)
    unsigned char _pad000[0x6c];
    float CameraDesc_NearZPlane;                       // +0x6c = Ego_r CameraDesc (+0x14) . NearZPlane (+0x58)
    unsigned char _pad070[0x114 - 0x70];
    CMatrix3x4 NoScaleViewTransform;                   // +0x114 (Ego_r CEngineCamera, same offset)
    CMatrix3x4 InverseNoScaleViewTransform;            // +0x144 (Ego_r CEngineCamera, same offset)
};
extern CEngineCameraView* g_pEngineCamera_01436ea0;   // 0x01436EA0
class CPixelShader;
struct CEnginePrimitiveRendererSharedDataView {        // CEnginePrimitiveRendererSharedData at 0x01436E78 (local view; header debt)
    unsigned char _pad000[0xdc];
    unsigned char PixelShaderDiffuseOnly[0x18];        // +0xdc (Ego_r, same offset)
    unsigned char PixelShaderDiffuse[0x18];            // +0xf4 (Ego_r, same offset)
};
extern CEnginePrimitiveRendererSharedDataView* g_pPrimitiveRendererSharedData;   // 0x01436E78 (bound spelling)
extern const float DELTA_NOTIONAL_ZERO;                // 0x0129BA3C (bound spelling)

class CEngineInternalPrimitiveLine { public:
    unsigned char _pad00[0x48];
    C3DVector     Start;                               // +0x48
    C3DVector     End;                                 // +0x54
    CRGBColour    Colour;                              // +0x60
    float         Width;                               // +0x64
    unsigned long BankIndex;                           // +0x68
    unsigned long Frame;                               // +0x6c
    long          BlendMode;                           // +0x70
    long          BlendOp;                             // +0x74
    bool          UseZBuffer;                          // +0x78
};
class CVertexShader;
class CEnginePrimitiveManagerLines { public:
    unsigned char _pad00[0xc];
    unsigned char VertexShader[8];                     // +0x0c
    void RenderLine(const CEngineInternalPrimitiveLine& line);
};
struct SLineVertex {
    C3DVector  Pos;
    CRGBColour Colour;
    float      U, V;
};

static __forceinline void TransformPoint(const CMatrix3x4& m, C3DVector& p)
{
    const float xx = p.X;
    const float yy = p.Y;
    const float zz = p.Z;
    p.X = m.E11 * xx + (m.E21 * yy + m.E31 * zz) + m.E41;
    p.Y = m.E12 * xx + (m.E22 * yy + m.E32 * zz) + m.E42;
    p.Z = m.E13 * xx + (m.E23 * yy + m.E33 * zz) + m.E43;
}
static __forceinline void TransformPointSums(const CMatrix3x4& m, C3DVector& p)
{
    const float xx = p.X;
    const float yy = p.Y;
    const float zz = p.Z;
    p.X = m.E11 * xx + (m.E21 * yy + m.E31 * zz) + m.E41;
    p.Y = m.E12 * xx + m.E22 * yy + m.E32 * zz + m.E42;
    p.Z = m.E13 * xx + m.E23 * yy + m.E33 * zz + m.E43;
}

void CEnginePrimitiveManagerLines::RenderLine(const CEngineInternalPrimitiveLine& line)
{
    g_pShaderRenderManager->ResetWorldTransform();
    CShaderEnableSaver shaders(true, true);
    C3DVector start = line.Start;
    C3DVector end = line.End;
    TransformPoint(g_pEngineCamera_01436ea0->NoScaleViewTransform, start);
    TransformPoint(g_pEngineCamera_01436ea0->NoScaleViewTransform, end);
    float near_plane = g_pEngineCamera_01436ea0->CameraDesc_NearZPlane;
    if (start.Z < near_plane) {
        if (end.Z < near_plane)
            return;
        C3DVector delta = end - start;
        start += delta * ((near_plane - start.Z) / delta.Z);
    } else if (end.Z < near_plane) {
        C3DVector delta = start - end;
        end += delta * ((near_plane - end.Z) / delta.Z);
    }
    float inv_start = 1.0f / start.Z;
    C2DVector screen_start(start.X * inv_start, start.Y * inv_start);
    float inv_end = 1.0f / end.Z;
    C2DVector screen_end(end.X * inv_end, end.Y * inv_end);
    C2DVector dir(screen_end.X - screen_start.X, screen_end.Y - screen_start.Y);
    if (!(dir.X * dir.X + dir.Y * dir.Y > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO))
        return;
    dir.Normalise();
    float half_width = line.Width * 0.5f;
    dir.X *= half_width;
    dir.Y *= half_width;
    C2DVector perp(dir.Y, -dir.X);
    C3DVector corner0(start.X - perp.X, start.Y - perp.Y, start.Z);
    C3DVector corner1(start.X + perp.X, start.Y + perp.Y, start.Z);
    C3DVector corner2(end.X - perp.X, end.Y - perp.Y, end.Z);
    C3DVector corner3(end.X + perp.X, end.Y + perp.Y, end.Z);
    TransformPointSums(g_pEngineCamera_01436ea0->InverseNoScaleViewTransform, corner0);
    TransformPointSums(g_pEngineCamera_01436ea0->InverseNoScaleViewTransform, corner1);
    TransformPointSums(g_pEngineCamera_01436ea0->InverseNoScaleViewTransform, corner2);
    TransformPointSums(g_pEngineCamera_01436ea0->InverseNoScaleViewTransform, corner3);
    CRenderManagerCoreView* core = g_pRenderManagerCore;
    SetState(1, core->m_state28b8, core);
    unsigned long brightness;
    switch (line.BlendMode) {
    case 3:
        SetState(g_renderStateDefault_01396f6c, g_pRenderManagerCore->m_state291c, g_pRenderManagerCore);
        SetState(g_renderStateDefault_01396f6c, g_pRenderManagerCore->m_state2930, g_pRenderManagerCore);
        brightness = line.Colour.A;
        break;
    case 4:
        SetState(g_renderStateDefault_01396f6c, g_pRenderManagerCore->m_state291c, g_pRenderManagerCore);
        SetState(g_renderStateDefault_01396f74, g_pRenderManagerCore->m_state2930, g_pRenderManagerCore);
        brightness = line.Colour.A;
        break;
    default:
        SetState(g_renderStateDefault_01396f78, g_pRenderManagerCore->m_state291c, g_pRenderManagerCore);
        SetState(g_renderStateDefault_01396f7c, g_pRenderManagerCore->m_state2930, g_pRenderManagerCore);
        brightness = 0xff;
        break;
    }
    bool textured = line.BankIndex != 0;
    float level = (float)brightness * (1.0f / 255.0f);
    g_pShaderRenderManager->SetPSConstant(2, C4DVector(level, level, level, (float)line.Colour.A * (1.0f / 255.0f)));
    g_pShaderRenderManager->ApplyPixelShader((CPixelShader*)(textured ? g_pPrimitiveRendererSharedData->PixelShaderDiffuse : g_pPrimitiveRendererSharedData->PixelShaderDiffuseOnly));
    SLineVertex verts[4];
    for (long i = 0; i < 4; ++i)
        verts[i].Colour = line.Colour;
    verts[0].Pos = corner0;
    verts[1].Pos = corner1;
    verts[2].Pos = corner2;
    verts[3].Pos = corner3;
    verts[0].U = 0.0f; verts[0].V = 0.0f;
    verts[1].U = 1.0f; verts[1].V = 0.0f;
    verts[2].U = 0.0f; verts[2].V = 1.0f;
    verts[3].U = 1.0f; verts[3].V = 1.0f;
    g_pShaderRenderManager->ApplyVertexShader((CVertexShader*)VertexShader, false);
    g_pShaderRenderManager->ResetWorldTransform();
    g_pShaderRenderManager->PrepareForRender(1);
    const CTexture* texture;
    unsigned long texture_count = 0;
    if (line.BankIndex != 0) {
        const CGraphicDataBankFull* bank = g_pResourceManager->MainGraphicBank;
        CGraphicInfoFrame* frame = bank->GetGraphicInfo(line.BankIndex) ? &bank->GetGraphicInfo(line.BankIndex)->Frames[line.Frame] : 0;
        if (!frame->Data) {
            frame->Data = new CFrameData(frame);
            frame->Data->BeginLoading(0, 0);
        } else if (!frame->Data->Frame.FrameData) {
            frame->Data->BeginLoading(0, 0);
        } else {
            frame->Data->TouchedFrame = frame->GraphicInfo->Bank->CurrentFrame;
            frame->Data->ManagedTexture.Touch();
            frame->Data->Touch();
            if ((unsigned char)frame->Data->TouchedMipmapLevel > 0)
                frame->Data->TouchedMipmapLevel = 0;
            if ((unsigned char)frame->Data->CurrentMipmapLevel > 0)
                frame->Data->BeginLoading(0, 0);
            CGraphicFrameRef graphic(frame->Data->Frame);
            texture = graphic.FrameData->PTexture.Data;
            texture_count = 1;
        }
    }
    g_pRenderManagerCore->DrawAsIndexedTris((CTexture**)&texture, texture_count, verts, 0x18, 4, g_LineQuadIndices_0139bac8, 2);
}
