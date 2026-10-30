// NUISystem::CMesh::~CMesh() -- 3D mesh component dtor: clear the engine primitive handle, then the handle's own destructor and ~CChangingStateComponent
// Leaf: ??1CMesh@NUISystem@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00550230, 163 bytes, tail jump to the base destructor.
// Identity: the vtable stores 0x01249F9C / 0x01249F74 / 0x01249F6C carry RTTI `.?AVCMesh@NUISystem@@` (CUIInterface @0, CObserver @4,
//   CActionParam @0x18); rtti_bases: CMesh : CChangingStateComponent : CComponent; Ego_r.pdb has `??1CMesh@NUISystem@@UAE@XZ`.
// Callees: ~CBaseClass 0x0099A2E0 (the handle's base, out of line), NUISystem::CChangingStateComponent::~CChangingStateComponent
//   0x0052CCF0 (tail jump); virtual calls: the primitive's NotifyReleased (slot 3) and CIVCountedPointeeBase::Delete (slot 1).
// Layout: Ego_r.pdb CMesh (0x1D4): MeshIndex +0x18C, MeshType +0x190, Handle +0x194, Height +0x19C, Width +0x1A0, Orientation
//   +0x1A4, RotationAxis +0x1BC, RotationSpeed +0x1C8, AnimationIndex +0x1CC, NoRenderNextFrame +0x1D0; retail is 0x30 lower
//   (Handle +0x164). No engine header exists for NUISystem::CMesh (header debt), so the polymorphic class is modelled here.
// Shape: the landed NUISystem::CSprite::~CSprite 0x0041AB40 model: the user body is `Handle.Clear();` (null-safe
//   GetReferenceCount() == 1 test, NotifyReleased(*this), `Data.Reset(0)`), then the inline ~CEnginePrimitiveHandle (handle vtable
//   0x0122F598, Clear() again, the CIVCountedPointer destructor, ~CBaseClass) and the out-of-line base destructor.
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

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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


class C3DVector { public:
    float X;                                                // +0x0
    float Y;                                                // +0x4
    float Z;                                                // +0x8
};
class CRightHandedSet { public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};
enum EEngineGraphicType { };                                // values not needed here

namespace NUISystem {
class CUIInterface { public: virtual ~CUIInterface(); };
class CObserver { public: virtual ~CObserver(); unsigned char _body[0x14 - 4]; };   // EventsToObserve (STLport set) +8, PreventObservation +0x14
class CActionParam { public: virtual ~CActionParam(); long ActionParamType; };       // +0x18, type +0x1C
class CComponent : public CUIInterface, public CObserver, public CActionParam { public:
    virtual ~CComponent();
    unsigned char _body[0x15c - 0x20];                      // +0x20 .. +0x15C: CComponent / CChangingStateComponent state
};
class CChangingStateComponent : public CComponent { public:
    virtual ~CChangingStateComponent();                     // 0x0052CCF0
};
class CMesh : public CChangingStateComponent { public:
    unsigned long          MeshIndex;                       // +0x15C
    EEngineGraphicType     MeshType;                        // +0x160
    CEnginePrimitiveHandle Handle;                          // +0x164
    unsigned long          Height;                          // +0x16C
    unsigned long          Width;                           // +0x170
    CRightHandedSet        Orientation;                     // +0x174
    C3DVector              RotationAxis;                    // +0x18C
    float                  RotationSpeed;                   // +0x198
    unsigned long          AnimationIndex;                  // +0x19C
    unsigned char          NoRenderNextFrame;               // +0x1A0
    virtual ~CMesh();
};

CMesh::~CMesh()
{
    Handle.Clear();
}
} // namespace NUISystem