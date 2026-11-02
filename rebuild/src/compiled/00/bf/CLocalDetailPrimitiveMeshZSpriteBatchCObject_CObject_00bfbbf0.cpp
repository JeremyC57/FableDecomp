// CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject() -- destroy one sprite-batch object: its PrimitiveHandle member (notify the primitive when this is its last handle, release the counted pointer) and the handle's CBaseClass base
// Leaf: ??1CObject@CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BFBBF0, 94 bytes, thiscall, tail `jmp ~CBaseClass` on the member (this + 0x44).
// Identity (proven): (1) its callers walk CLocalDetailPrimitiveMeshZSpriteBatch::CObject arrays: ~CLocalDetailPrimitiveMeshZSpriteBatch
//   0x00BFB590 destroys `count` objects at stride 0x4C (0x00BFB5C2), the array deleter 0x00BFBD20 (next to AllocArrayOfObjects
//   <CLocalDetailPrimitiveMeshZSpriteBatch::CObject> 0x00BFBC80) and 0x00BFBD5A call it per element; (2) Ego_r.pdb
//   ??1CObject@CLocalDetailPrimitiveMeshZSpriteBatch@@QAE@XZ (and FableWin.pdb, next to ??0 / ??_G), and Ego_r's 76-byte CObject
//   {CMatrix3x4 Matrix, float Scale +0x30, C3DBoundingSphere BoundingSphere +0x34, CEnginePrimitiveHandle PrimitiveHandle +0x44}
//   is the 0x4C stride with the only non-trivial member at +0x44 (it stores the CEnginePrimitiveHandle vtable 0x0122F598 there).
//   The catalog label "CObject::~CObject" lacks the enclosing class.
// Callees: CEnginePersistentPrimitiveBase::NotifyReleased (virtual slot 3, [eax+0Ch]) and CIVCountedPointeeBase::Delete (slot 1,
//   [eax+4]) through the inline CEnginePrimitiveHandle destructor (Clear() then the CIVCountedPointer member), CBaseClass::~CBaseClass
//   0x0099A2E0.
// Shape: CObject has no vtable and no user destructor: the implicit destructor is the member's inline virtual
//   ~CEnginePrimitiveHandle (`{ Clear(); }`, the landed NUISystem::CIconText 0x005424B0 model: Clear = `if (Data.GetRefCount() == 1)
//   Data->NotifyReleased(*this); Data.Reset(0);`, then ~CIVCountedPointer's Release), so the PrimitiveHandle release runs twice.
//   The handle destructor is `__forceinline` as in the landed includers (0x005424B0, 0x0054F6E0, 0x005483C0, 0x00AFBBD0, ...):
//   retail's own ??_G 0x0041C500 expands it, Ego_r.pdb has no out-of-line ??1CEnginePrimitiveHandle (only FableWin.pdb, the debug
//   build), and with plain `inline` VC7.1 calls it out of line here (8 bytes: `add ecx,44h ; jmp`), with or without a user ~CObject.
// Layout: CIVCountedPointeeBase from engine/CIVCountedPointeeBase.h (retail opt-in); engine/CEnginePersistentPrimitiveBase.h is a
//   flattened layout, so the polymorphic base is local (vtable 0x012A3D60 order {dtor, Delete, CollectStats, NotifyReleased,
//   WasRendered}, as 0x005424B0).
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"
#include "engine/CBaseClass.h"                              // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;                          // +0x00
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return (T*)Object; }
    unsigned long GetRefCount() const { return Object ? Object->GetRefCount() : 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    void Reset(T* object)
    {
        Release();
        Object = object;
        if (object)
            object->AddRef();
    }
};

class CEnginePrimitiveHandle;
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
    friend class CEnginePrimitiveHandle;
public:
    virtual void CollectStats() const = 0;                  // slot 2 (signature not needed here)
protected:
    virtual void NotifyReleased(const CEnginePrimitiveHandle& handle);   // slot 3
    virtual bool WasRendered() const;                       // slot 4
public:
    long Type;                                              // +0x08
};

class CEnginePrimitiveHandle : public CBaseClass {          // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;    // +0x04
    virtual __forceinline ~CEnginePrimitiveHandle() { Clear(); }
    void Clear()                                            // 0x0041BE70 out of line
    {
        if (Data.GetRefCount() == 1)
            Data->NotifyReleased(*this);
        Data.Reset(0);
    }
};

class C3DVector {
public:
    float X;
    float Y;
    float Z;
};
class CMatrix3x4 {
public:
    float M[12];                                            // 0x30 bytes
};
class C3DBoundingSphere {
public:
    C3DVector Center;                                       // +0x00
    float     Radius;                                       // +0x0C
};

class CLocalDetailPrimitiveMeshZSpriteBatch {
public:
    class CObject {                                         // 0x4C bytes
    public:
        CMatrix3x4             Matrix;                      // +0x00
        float                  Scale;                       // +0x30
        C3DBoundingSphere      BoundingSphere;              // +0x34
        CEnginePrimitiveHandle PrimitiveHandle;             // +0x44
    };
};

void DestroyOne(CLocalDetailPrimitiveMeshZSpriteBatch::CObject* object)
{
    object->CLocalDetailPrimitiveMeshZSpriteBatch::CObject::~CObject();
}