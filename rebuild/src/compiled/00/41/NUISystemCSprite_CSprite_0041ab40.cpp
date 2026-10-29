// NUISystem::CSprite::~CSprite() -- sprite component dtor: clear the engine primitive handle, then the member dtors (the texture counted pointer, the handle) and ~CChangingStateComponent
// Leaf: ??1CSprite@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0041AB40, 218 bytes, tail jump to the base destructor.
// Identity: the three vtable stores 0x0122F5D4 / 0x0122F5AC / 0x0122F5A4 all carry RTTI `.?AVCSprite@NUISystem@@` (CUIInterface @0,
//   CObserver @4, CActionParam @0x18 per rtti_bases: CSprite : CChangingStateComponent : CComponent); Ego_r.pdb has
//   `??1CSprite@NUISystem@@UAE@XZ`, and the landed caller 0x0054C360 binds that spelling. The catalog label `CSprite::~CSprite` and the
//   landed 0x0055C510 binding `??1CSprite@@QAE@XZ` drop the namespace.
// Callees: ~CBaseClass 0x0099A2E0 (the handle's base, out of line), operator delete 0x00BFE9BC (the counted pointer's info block),
//   NUISystem::CChangingStateComponent::~CChangingStateComponent 0x0052CCF0 (tail jump); virtual calls: the primitive's
//   NotifyReleased (slot 3) and CIVCountedPointeeBase::Delete (slot 1), the info block's DeleteData.
// Layout: Ego_r.pdb CSprite: Handle (CEnginePrimitiveHandle) +0x15C, Angle +0x164, Width +0x168, Height +0x16C, NoRenderNextFrame +0x170,
//   Sprite2DFlag +0x174, GraphicIndex +0x178, PTexture (CCountedPointer<CTexture>) +0x17C/+0x180 -- the same offsets as the generated
//   engine/CSprite.h, which is flattened (global `struct CSprite`, vptr as data), so the polymorphic class is modelled here (header debt).
//   engine/CIVCountedPointeeBase.h and engine/CObserver.h (global, not NUISystem::) flatten their vptrs the same way, so the
//   virtual bases are declared here too.
// Shape: the landed NUISystem::CText::~CText 0x0054F6E0 model: the user body is `Handle.Clear();` (null-safe GetReferenceCount() == 1
//   test, NotifyReleased(*this), `Data.Reset(0)` = release plus the unconditional null store); the implicit member dtors follow in
//   reverse order: ~CCountedPointer<CTexture> (ReleaseData, then Data = 0, Info = 0) and the inline ~CEnginePrimitiveHandle (handle vtable
//   0x0122F598, Clear() again, the CIVCountedPointer destructor, ~CBaseClass), then the out-of-line base destructor.
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
enum EEngineSprite2DFlag { };                               // values not needed here

namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };   // EventsToObserve (STLport set) +4, PreventObservation +0x10
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };       // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    unsigned char _body[0x15c - 0x20];                      // +0x20 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CSprite : public CChangingStateComponent { public:
    CEnginePrimitiveHandle    Handle;                       // +0x15C
    float                     Angle;                        // +0x164
    unsigned long             Width;                        // +0x168
    unsigned long             Height;                       // +0x16C
    unsigned char             NoRenderNextFrame;            // +0x170
    EEngineSprite2DFlag       Sprite2DFlag;                 // +0x174
    unsigned long             GraphicIndex;                 // +0x178
    CCountedPointer<CTexture> PTexture;                     // +0x17C
    virtual ~CSprite();
};

CSprite::~CSprite()
{
    Handle.Clear();
}
} // namespace NUISystem