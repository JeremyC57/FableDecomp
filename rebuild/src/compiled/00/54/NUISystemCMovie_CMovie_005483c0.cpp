// NUISystem::CMovie::~CMovie() -- movie component dtor: clear the primitive handle, let the parent viewport drop its handle, and if this is the playing movie stop it, free the movie texture and delete the shared video system; then the member and base destructors
// Leaf: ??1CMovie@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005483C0, 289 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x0124881C / 0x012487F4 / 0x012487EC carry RTTI `.?AVCMovie@NUISystem@@`; rtti_bases: CMovie :
//   CChangingStateComponent : CComponent; Ego_r.pdb has `??1CMovie@NUISystem@@UAE@XZ`. The catalog label is right.
// Callees: CVideoSys::Close 0x00A3B380 and ~CVideoSys 0x00A3BC20 (Ego_r.pdb `?Close@CVideoSys@@QAEXXZ` / `??1CVideoSys@@QAE@XZ`,
//   bound so), operator delete 0x00BFE9BC, ~CWideString 0x0099B510 (Movie), ~CBaseClass 0x0099A2E0 (the handle's base),
//   NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0 (tail jump). Virtual calls: the parent viewport's
//   CComponent::ClearHandle (slot 91, +0x16C: the CViewport vtable 0x01249A5C holds the landed ClearHandle body 0x0053FC90 at slot
//   91, and Ego_r.pdb has `?ClearHandle@CComponent@NUISystem@@UAEXXZ`), CIEngine::FreeTextureFromEngineResourcePools (slot 70,
//   +0x118, inside the inlined protected CMovie::FreeTextures 0x00547B90), the primitive's NotifyReleased (3) and Delete (1).
// Data: the class statics keep their bound spellings (no PDB data symbols): PlayingMovie 0x013B8AB4, PEngine 0x013B8AB8 (as the
//   landed FreeTextures 0x00547B90), MovieTexture 0x013B8AC0; the video system pointer g_pVideoSys 0x013B8ABC.
// Layout: Ego_r.pdb CMovie minus 0x30: Playing +0x15C, Handle +0x160, Width +0x168, Height +0x16C, PositionIsCenter +0x170, Movie
//   +0x174, Playback +0x178, Angle +0x190, AttempToPlay +0x194. CComponent::PParentViewport is Ego_r +0xF4, retail +0xDC (the
//   STLport set/map/vector/list members of CObserver/CComponent are 4-8 bytes shorter). engine/CMovie.h is a flattened global
//   struct, not the polymorphic NUISystem::CMovie (header debt), so the class is modelled here.
// Shape: user body `Handle.Clear(); if (PParentViewport) PParentViewport->ClearHandle(); if (PlayingMovie == this) { if (Playing)
//   g_pVideoSys->Close(); PlayingMovie = 0; FreeTextures(); delete g_pVideoSys; g_pVideoSys = 0; }`, then the implicit member
//   dtors in reverse order (CXMVPlaybackInfo has none, ~CWideString, the inline ~CEnginePrimitiveHandle) and the base dtor.
void __cdecl operator delete(void* block);                  // 0x00BFE9BC

class CCPPointerInfo { public:                              // CCountedPointer count block
    unsigned long RefCount;                                 // +0x00
    void (__fastcall* DeleteFunc)(void* data);              // +0x04
    void*         Data;                                     // +0x08
};
template <class T> class CCountedPointer { public:
    T*              Data;                                   // +0x00
    CCPPointerInfo* Info;                                   // +0x04
    __forceinline void ReleaseData()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
    }
    __forceinline ~CCountedPointer() { ReleaseData(); Data = 0; Info = 0; }
};

class CBaseClass { public:
    virtual ~CBaseClass();                                  // 0x0099A2E0
};
class CEnginePrimitiveHandle;
class CIVCountedPointeeBase { public:
    virtual ~CIVCountedPointeeBase();
    virtual void Delete() const;                            // slot 1 (+0x04)
    mutable unsigned long IVRefCount;                       // +0x04
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
protected:
    virtual void CUnprovenSlot2_012a3d60() = 0;             // slot 2: _purecall in the vtable 0x012A3D60 (no PDB name found)
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3 (+0x0C)
    virtual bool WasRendered() const;                       // slot 4
};
template <class T> class CIVCountedPointer { public:
    CIVCountedPointeeBase* Object;
    T* operator->() const { return static_cast<T*>(Object); }
    unsigned long GetRefCount() const { return Object ? Object->IVRefCount : 0; }
    __forceinline void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    __forceinline void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            ++object->IVRefCount;
    }
    __forceinline ~CIVCountedPointer() { Release(); }
};
class CEnginePrimitiveHandle : public CBaseClass { public:  // vtable 0x0122F598, 8 bytes
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data; // +0x04
    unsigned long GetReferenceCount() const { return Data.GetRefCount(); }
    __forceinline void Clear()
    {
        if (GetReferenceCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
    __forceinline virtual ~CEnginePrimitiveHandle() { Clear(); }
};


class CTexture;
class CIEngine { public:                                    // vtable 0x012A0D24
    virtual ~CIEngine();
    virtual void CUnprovenSlot1_012a0d24();
    virtual void CUnprovenSlot2_012a0d24();
    virtual void CUnprovenSlot3_012a0d24();
    virtual void CUnprovenSlot4_012a0d24();
    virtual void CUnprovenSlot5_012a0d24();
    virtual void CUnprovenSlot6_012a0d24();
    virtual void CUnprovenSlot7_012a0d24();
    virtual void CUnprovenSlot8_012a0d24();
    virtual void CUnprovenSlot9_012a0d24();
    virtual void CUnprovenSlot10_012a0d24();
    virtual void CUnprovenSlot11_012a0d24();
    virtual void CUnprovenSlot12_012a0d24();
    virtual void CUnprovenSlot13_012a0d24();
    virtual void CUnprovenSlot14_012a0d24();
    virtual void CUnprovenSlot15_012a0d24();
    virtual void CUnprovenSlot16_012a0d24();
    virtual void CUnprovenSlot17_012a0d24();
    virtual void CUnprovenSlot18_012a0d24();
    virtual void CUnprovenSlot19_012a0d24();
    virtual void CUnprovenSlot20_012a0d24();
    virtual void CUnprovenSlot21_012a0d24();
    virtual void CUnprovenSlot22_012a0d24();
    virtual void CUnprovenSlot23_012a0d24();
    virtual void CUnprovenSlot24_012a0d24();
    virtual void CUnprovenSlot25_012a0d24();
    virtual void CUnprovenSlot26_012a0d24();
    virtual void CUnprovenSlot27_012a0d24();
    virtual void CUnprovenSlot28_012a0d24();
    virtual void CUnprovenSlot29_012a0d24();
    virtual void CUnprovenSlot30_012a0d24();
    virtual void CUnprovenSlot31_012a0d24();
    virtual void CUnprovenSlot32_012a0d24();
    virtual void CUnprovenSlot33_012a0d24();
    virtual void CUnprovenSlot34_012a0d24();
    virtual void CUnprovenSlot35_012a0d24();
    virtual void CUnprovenSlot36_012a0d24();
    virtual void CUnprovenSlot37_012a0d24();
    virtual void CUnprovenSlot38_012a0d24();
    virtual void CUnprovenSlot39_012a0d24();
    virtual void CUnprovenSlot40_012a0d24();
    virtual void CUnprovenSlot41_012a0d24();
    virtual void CUnprovenSlot42_012a0d24();
    virtual void CUnprovenSlot43_012a0d24();
    virtual void CUnprovenSlot44_012a0d24();
    virtual void CUnprovenSlot45_012a0d24();
    virtual void CUnprovenSlot46_012a0d24();
    virtual void CUnprovenSlot47_012a0d24();
    virtual void CUnprovenSlot48_012a0d24();
    virtual void CUnprovenSlot49_012a0d24();
    virtual void CUnprovenSlot50_012a0d24();
    virtual void CUnprovenSlot51_012a0d24();
    virtual void CUnprovenSlot52_012a0d24();
    virtual void CUnprovenSlot53_012a0d24();
    virtual void CUnprovenSlot54_012a0d24();
    virtual void CUnprovenSlot55_012a0d24();
    virtual void CUnprovenSlot56_012a0d24();
    virtual void CUnprovenSlot57_012a0d24();
    virtual void CUnprovenSlot58_012a0d24();
    virtual void CUnprovenSlot59_012a0d24();
    virtual void CUnprovenSlot60_012a0d24();
    virtual void CUnprovenSlot61_012a0d24();
    virtual void CUnprovenSlot62_012a0d24();
    virtual void CUnprovenSlot63_012a0d24();
    virtual void CUnprovenSlot64_012a0d24();
    virtual void CUnprovenSlot65_012a0d24();
    virtual void CUnprovenSlot66_012a0d24();
    virtual void CUnprovenSlot67_012a0d24();
    virtual void CUnprovenSlot68_012a0d24();
    virtual void CUnprovenSlot69_012a0d24();
    virtual void FreeTextureFromEngineResourcePools(CTexture* texture);   // slot 70 (+0x118)
};
#define CVideoSys CVideoSysLayout
#include "engine/CVideoSys.h"
#undef CVideoSys
class CVideoSys : public CVideoSysLayout { public:
    void Close();                                           // 0x00A3B380
    ~CVideoSys();                                           // 0x00A3BC20
};
class CWideString { public:
    void* PStringData;
    ~CWideString();                                         // 0x0099B510
};
class XMVDecoder;
class CXMVPlaybackInfo { public:
    XMVDecoder*   PDecoder;                                 // +0x00
    CTexture*     PDraw;                                    // +0x04
    CTexture*     PShow;                                    // +0x08
    bool          SurfaceChanged;                           // +0x0C
    unsigned long Flags;                                    // +0x10
    bool          LazyInstallerNeedsUnpause;                // +0x14
};
extern CVideoSys* g_pVideoSys;                              // 0x013B8ABC

class CCharString;
namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };   // EventsToObserve (STLport set) +8, PreventObservation +0x14
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };       // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    virtual void Update(float time);                        // slot 1
    virtual void Draw(CIEngine& engine, CEnginePrimitiveHandle* handle, long a, long* b, CComponent* c);   // slot 2
    virtual CComponent* FindChild(CCharString name);        // slot 3 (Ego_r.pdb ?FindChild@CComponent@NUISystem@@UAEPAV12@VCCharString@@@Z)
    virtual void CUnprovenSlot4_0124608c();
    virtual void CUnprovenSlot5_0124608c();
    virtual void CUnprovenSlot6_0124608c();
    virtual void CUnprovenSlot7_0124608c();
    virtual void CUnprovenSlot8_0124608c();
    virtual void CUnprovenSlot9_0124608c();
    virtual void CUnprovenSlot10_0124608c();
    virtual void CUnprovenSlot11_0124608c();
    virtual void CUnprovenSlot12_0124608c();
    virtual void CUnprovenSlot13_0124608c();
    virtual void CUnprovenSlot14_0124608c();
    virtual void CUnprovenSlot15_0124608c();
    virtual void CUnprovenSlot16_0124608c();
    virtual void CUnprovenSlot17_0124608c();
    virtual void CUnprovenSlot18_0124608c();
    virtual void CUnprovenSlot19_0124608c();
    virtual void CUnprovenSlot20_0124608c();
    virtual void CUnprovenSlot21_0124608c();
    virtual void CUnprovenSlot22_0124608c();
    virtual void CUnprovenSlot23_0124608c();
    virtual void CUnprovenSlot24_0124608c();
    virtual void CUnprovenSlot25_0124608c();
    virtual void CUnprovenSlot26_0124608c();
    virtual void CUnprovenSlot27_0124608c();
    virtual void CUnprovenSlot28_0124608c();
    virtual void CUnprovenSlot29_0124608c();
    virtual void CUnprovenSlot30_0124608c();
    virtual void CUnprovenSlot31_0124608c();
    virtual void CUnprovenSlot32_0124608c();
    virtual void CUnprovenSlot33_0124608c();
    virtual void CUnprovenSlot34_0124608c();
    virtual void CUnprovenSlot35_0124608c();
    virtual void CUnprovenSlot36_0124608c();
    virtual void CUnprovenSlot37_0124608c();
    virtual void CUnprovenSlot38_0124608c();
    virtual void CUnprovenSlot39_0124608c();
    virtual void CUnprovenSlot40_0124608c();
    virtual void CUnprovenSlot41_0124608c();
    virtual void CUnprovenSlot42_0124608c();
    virtual void CUnprovenSlot43_0124608c();
    virtual void CUnprovenSlot44_0124608c();
    virtual void CUnprovenSlot45_0124608c();
    virtual void CUnprovenSlot46_0124608c();
    virtual void CUnprovenSlot47_0124608c();
    virtual void CUnprovenSlot48_0124608c();
    virtual void CUnprovenSlot49_0124608c();
    virtual void CUnprovenSlot50_0124608c();
    virtual void CUnprovenSlot51_0124608c();
    virtual void CUnprovenSlot52_0124608c();
    virtual void CUnprovenSlot53_0124608c();
    virtual void CUnprovenSlot54_0124608c();
    virtual void CUnprovenSlot55_0124608c();
    virtual void CUnprovenSlot56_0124608c();
    virtual void CUnprovenSlot57_0124608c();
    virtual void CUnprovenSlot58_0124608c();
    virtual void CUnprovenSlot59_0124608c();
    virtual void CUnprovenSlot60_0124608c();
    virtual void CUnprovenSlot61_0124608c();
    virtual void CUnprovenSlot62_0124608c();
    virtual void CUnprovenSlot63_0124608c();
    virtual void CUnprovenSlot64_0124608c();
    virtual void CUnprovenSlot65_0124608c();
    virtual void CUnprovenSlot66_0124608c();
    virtual void CUnprovenSlot67_0124608c();
    virtual void CUnprovenSlot68_0124608c();
    virtual void CUnprovenSlot69_0124608c();
    virtual void CUnprovenSlot70_0124608c();
    virtual void CUnprovenSlot71_0124608c();
    virtual void CUnprovenSlot72_0124608c();
    virtual void CUnprovenSlot73_0124608c();
    virtual void CUnprovenSlot74_0124608c();
    virtual void CUnprovenSlot75_0124608c();
    virtual void CUnprovenSlot76_0124608c();
    virtual void CUnprovenSlot77_0124608c();
    virtual void CUnprovenSlot78_0124608c();
    virtual void CUnprovenSlot79_0124608c();
    virtual void CUnprovenSlot80_0124608c();
    virtual void CUnprovenSlot81_0124608c();
    virtual void CUnprovenSlot82_0124608c();
    virtual void CUnprovenSlot83_0124608c();
    virtual void CUnprovenSlot84_0124608c();
    virtual void CUnprovenSlot85_0124608c();
    virtual void CUnprovenSlot86_0124608c();
    virtual void CUnprovenSlot87_0124608c();
    virtual void CUnprovenSlot88_0124608c();
    virtual void CUnprovenSlot89_0124608c();
    virtual void CUnprovenSlot90_0124608c();
    virtual void ClearHandle();                             // slot 91 (+0x16C)
    unsigned char _body0[0xdc - 0x20];                      // +0x20 .. +0xDC
    CComponent*   PParentViewport;                          // +0xDC
    unsigned char _body1[0x15c - 0xe0];                     // +0xE0 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CMovie : public CChangingStateComponent { public:
    bool                   Playing;                         // +0x15C
    CEnginePrimitiveHandle Handle;                          // +0x160
    unsigned long          Width;                           // +0x168
    unsigned long          Height;                          // +0x16C
    bool                   PositionIsCenter;                // +0x170
    CWideString            Movie;                           // +0x174
    CXMVPlaybackInfo       Playback;                        // +0x178
    float                  Angle;                           // +0x190
    bool                   AttempToPlay;                    // +0x194
    static CMovie*         PlayingMovie;                    // 0x013B8AB4
    static CIEngine*       PEngine;                         // 0x013B8AB8
    static CTexture*       MovieTexture;                    // 0x013B8AC0
    virtual ~CMovie();
protected:
    void FreeTextures()                                     // 0x00547B90 (inlined here)
    {
        if (MovieTexture) {
            PEngine->FreeTextureFromEngineResourcePools(MovieTexture);
            MovieTexture = 0;
        }
    }
};

CMovie::~CMovie()
{
    Handle.Clear();
    if (PParentViewport)
        PParentViewport->ClearHandle();
    if (PlayingMovie == this) {
        if (Playing)
            g_pVideoSys->Close();
        PlayingMovie = 0;
        FreeTextures();
        delete g_pVideoSys;
        g_pVideoSys = 0;
    }
}
} // namespace NUISystem