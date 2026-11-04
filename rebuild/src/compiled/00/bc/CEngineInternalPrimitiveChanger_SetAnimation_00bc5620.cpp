// CEngineInternalPrimitiveChanger::SetAnimation(const CEnginePrimitiveAnimation& animation) -- forward the animation to every child primitive the changer holds
// Leaf: ?SetAnimation@CEngineInternalPrimitiveChanger@@UAEXABVCEnginePrimitiveAnimation@@@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC5620, 64 bytes, thiscall, `ret 4`.
// Identity (proven): slot 23 of vtable 0x012A662C (RTTI .?AVCEngineInternalPrimitiveChanger@@ : CEngineInternalPrimitiveBase :
//   CEnginePersistentPrimitiveBase : CIVCountedPointeeBase); Ego_r.pdb ?SetAnimation@CEngineInternalPrimitiveChanger@@UAEXABVCEnginePrimitiveAnimation@@@Z
//   (donor order SetAnimation, SetPosition, GetWorldPosition matches slots 23, 24 and retail 0x00BC5620 / 0x00BC5660).
// Callees: each child's own virtual slot 23 (+0x5C) -- the same SetAnimation -- through its CEnginePrimitiveHandle.
// Layout (Ego_r.pdb): Primitives CArray<CEnginePrimitiveHandle> at +0x48 (8-byte handles: CBaseClass vptr, Data object at +4).
// Shape: the element count computed once into a local, an unsigned up-counting index, a reference to the element
//   (retail keeps its dead `lea eax,[eax+edi*8]`), the call under the handle's inline IsNull() through GetData() (FableWin.pdb
//   ?IsNull@CEnginePrimitiveHandle@@QBE_NXZ, ?GetData@CEnginePrimitiveHandle@@QBEAAVCEnginePersistentPrimitiveBase@@XZ).
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

#include "engine/CArray.h"

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase();
    mutable unsigned long IVRefCount;                // +0x04
};
template <class T> class CIVCountedPointer {
public:
    bool IsNull() const { return Object == 0; }
    T* Get() const { return static_cast<T*>(Object); }
    T& operator*() const { return *Get(); }
    CIVCountedPointeeBase* Object;
};
class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
public:
    long Type;                                       // +0x08
};
class CEnginePrimitiveHandle : public CBaseClass {
public:
    bool IsNull() const { return Data.IsNull(); }
    CEnginePersistentPrimitiveBase& GetData() const { return *Data; }
    CIVCountedPointer<CEnginePersistentPrimitiveBase> Data;   // +0x04
};

class CEnginePrimitiveAnimation;
class CEnginePrimitivePosition;

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    // virtual slots 1..22 (not used here; slot 0 is the destructor)
    virtual void VirtualSlot01(); virtual void VirtualSlot02(); virtual void VirtualSlot03(); virtual void VirtualSlot04();
    virtual void VirtualSlot05(); virtual void VirtualSlot06(); virtual void VirtualSlot07(); virtual void VirtualSlot08();
    virtual void VirtualSlot09(); virtual void VirtualSlot10(); virtual void VirtualSlot11(); virtual void VirtualSlot12();
    virtual void VirtualSlot13(); virtual void VirtualSlot14(); virtual void VirtualSlot15(); virtual void VirtualSlot16();
    virtual void VirtualSlot17(); virtual void VirtualSlot18(); virtual void VirtualSlot19(); virtual void VirtualSlot20();
    virtual void VirtualSlot21(); virtual void VirtualSlot22();
    virtual void SetAnimation(const CEnginePrimitiveAnimation& animation);   // slot 23 (+0x5C)
    virtual void SetPosition(const CEnginePrimitivePosition& position);      // slot 24 (+0x60)
    unsigned char _pad0C[0x48 - 0x0C];               // the rest of CEngineInternalPrimitiveBase (0x48 bytes)
};

class CEngineInternalPrimitiveChanger : public CEngineInternalPrimitiveBase {
public:
    virtual void SetAnimation(const CEnginePrimitiveAnimation& animation);
    virtual void SetPosition(const CEnginePrimitivePosition& position);
    CArray<CEnginePrimitiveHandle> Primitives;       // +0x48
};

void CEngineInternalPrimitiveChanger::SetAnimation(const CEnginePrimitiveAnimation& animation)
{
    unsigned long count = Primitives.size();
    for (unsigned long i = 0; i < count; ++i) {
        CEnginePrimitiveHandle& handle = Primitives[i];
        if (!handle.IsNull())
            static_cast<CEngineInternalPrimitiveBase&>(handle.GetData()).SetAnimation(animation);
    }
}