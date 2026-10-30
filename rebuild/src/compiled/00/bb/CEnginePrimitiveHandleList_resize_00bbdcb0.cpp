// CEnginePrimitiveHandleList::resize(unsigned long newSize) -- STLport list<CEnginePrimitiveHandle>::resize(n): forwards to resize(n, CEnginePrimitiveHandle()) with a default-constructed handle temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BBDCB0, length 134 bytes (+ int3 padding).
// Label correction: the catalog calls this CEnginePrimitiveRenderer::AddPrimitive (propagated from the
// byte-identical shape 0x00B32E90). Its only caller, CEngineInternalPrimitiveParticleGroup::ClearAllChildPrimitives
// (0x00BBD200), calls it with ecx = the group's 4-byte STLport list at +0x48 and an argument of 0, and the callee
// 0x00BBDC60 walks that list to the requested index and then either inserts copies of its second argument or
// erases the tail (0x00BBD6B0): STLport `resize(size_type, const T&)`. So this body is the one-argument
// `resize(size_type __new_size) { resize(__new_size, _Tp()); }` of a list of primitive handles.
// Callees: 0x0099A2D0 CBaseClass::CBaseClass(), 0x00BBDC60 list::resize(size_type, const CEnginePrimitiveHandle&),
// 0x0099A2E0 CBaseClass::~CBaseClass(); the primitive's virtual slots 3 and 1 in the temporary's destructor.
// Leaf: ?resize@CEnginePrimitiveHandleList@@QAEXK@Z
// Shape: identical to 0x00B32E90 -- the handle temporary is an 8-byte frame object built by the inline
// constructor (vtable and null Data stored after the argument pushes) and destroyed by the inline destructor
// (IsUnique -> slot 3, one null-tested release through the DecRef/GetRefCount accessors, one zero store,
// out-of-line base destructor).
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void DecRef() { --RefCount; }
    long GetRefCount() const { return RefCount; }
    void Release()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    CIVCountedPointer() : Object(0) { }
    CEnginePersistentPrimitiveBase* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    ~CIVCountedPointer()
    {
        if (Object)
            Object->Release();
        Object = 0;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    CEnginePrimitiveHandle() { }
    ~CEnginePrimitiveHandle()
    {
        if (Data.IsUnique())
            Data->OnLastHandleReleased(this);
    }
};

struct SHandleListNode;
struct CEnginePrimitiveHandleList {                 // STLport list<CEnginePrimitiveHandle>: the node pointer only
    SHandleListNode* m_node;
    void resize(unsigned long newSize, const CEnginePrimitiveHandle& value);   // 0x00BBDC60
    void resize(unsigned long newSize);
};

void CEnginePrimitiveHandleList::resize(unsigned long newSize)
{
    resize(newSize, CEnginePrimitiveHandle());
}