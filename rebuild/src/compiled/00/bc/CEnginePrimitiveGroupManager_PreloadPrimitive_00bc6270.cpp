// CEnginePrimitiveGroupManager::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags) -- preload every child of a primitive group through its own type's manager; true if any child reported work
// Leaf: ?PreloadPrimitive@CEnginePrimitiveGroupManager@@UAE_NAAVCEngineInternalPrimitiveBase@@ABVC3DVector@@K@Z
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BC6270, 95 bytes, thiscall (`this` unused), `ret 0Ch`.
// Identity (proven): slot 9 of vtable 0x012A66AC (RTTI .?AVCEnginePrimitiveGroupManager@@); Ego_r.pdb
//   ?PreloadPrimitive@CEnginePrimitiveGroupManager@@UAE_NAAVCEngineInternalPrimitiveBase@@ABVC3DVector@@K@Z.
// Callees: each child's manager's virtual slot 9 (the same PreloadPrimitive), the manager taken from the
//   CEnginePrimitiveTypeInfo global 0x01436E84 (PrimitiveManagers[Type]).
// Layout (Ego_r.pdb): CEngineInternalPrimitiveGroup::Primitives CArray<CEnginePrimitiveHandle> at +0x50.
// Shape: as RenderPrimitiveToShadowBuffer 0x00BC6210, with a bool result OR-ed with each child's answer. The result is
//   declared first: it then gets its own `push ecx` frame byte (declared after the group reference, VC7.1 homes it in the
//   dead `primitive` argument slot).
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

class C3DVector;
enum ERendered { };
enum EEngineRenderLayer { };
class CEngineSubPrimitiveChain;

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    unsigned char _pad0C[0x48 - 0x0C];               // CEngineInternalPrimitiveBase members (0x48 bytes)
};
class CEngineInternalPrimitiveGroup : public CEngineInternalPrimitiveBase {
public:
    unsigned char _pad48[0x50 - 0x48];
    CArray<CEnginePrimitiveHandle> Primitives;       // +0x50
    unsigned long PrimitiveFlags;                    // +0x5C
};

class CBaseClassNonCopyable : public CBaseClass {
protected:
    CBaseClassNonCopyable();                         // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                         // 0x0099A300 (protected virtual: Ego_r.pdb ??1CBaseClassNonCopyable@@MAE@XZ)
};
class CEnginePrimitiveManager : public CBaseClassNonCopyable {
public:
    // virtual slots 1..7 (not used here; slot 0 is the destructor)
    virtual void VirtualSlot01(); virtual void VirtualSlot02(); virtual void VirtualSlot03(); virtual void VirtualSlot04();
    virtual void VirtualSlot05(); virtual void VirtualSlot06(); virtual void VirtualSlot07();
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive);                          // slot 8 (+0x20)
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);  // slot 9 (+0x24)
};
class CEnginePrimitiveManagerOcclusionBase : public CEnginePrimitiveManager { };
class CEnginePrimitiveGroupManager : public CEnginePrimitiveManagerOcclusionBase {
public:
    virtual ERendered RenderPrimitiveToShadowBuffer(CEngineInternalPrimitiveBase& primitive);
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);
};

class CEngineComponent {
public:
    virtual ~CEngineComponent();
    void** GlobalPointerAddress;                     // +0x04
    bool RenderEnabled;                              // +0x08
    bool KeepAlive;                                  // +0x09
};
class CEnginePrimitiveTypeInfo : public CEngineComponent, public CBaseClassNonCopyable {
public:
    CEnginePrimitiveManager* PrimitiveManagers[47];  // +0x10
};
extern CEnginePrimitiveTypeInfo* g_pEnginePrimitiveTypeInfo;   // 0x01436E84

bool CEnginePrimitiveGroupManager::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags)
{
    bool result = false;
    CEngineInternalPrimitiveGroup& group = static_cast<CEngineInternalPrimitiveGroup&>(primitive);
    unsigned long count = group.Primitives.size();
    for (unsigned long i = 0; i < count; ++i) {
        CEnginePrimitiveHandle& handle = group.Primitives[i];
        if (!handle.IsNull()) {
            CEngineInternalPrimitiveBase& child = static_cast<CEngineInternalPrimitiveBase&>(handle.GetData());
            result |= g_pEnginePrimitiveTypeInfo->PrimitiveManagers[child.Type]->PreloadPrimitive(child, position, flags);
        }
    }
    return result;
}