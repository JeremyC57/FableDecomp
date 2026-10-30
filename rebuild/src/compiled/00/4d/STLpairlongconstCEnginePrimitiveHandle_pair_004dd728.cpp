// std::pair<const long, CEnginePrimitiveHandle>::~pair() -- the primitive-handle map's value pair: destroy the handle at +4 (tell the primitive it loses its last handle, drop the counted Data pointer, CBaseClass base)
// Leaf: ??1?$pair@$$CBJVCEnginePrimitiveHandle@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DD728, 90 bytes, thiscall, size TU; tail jump to CBaseClass::~CBaseClass 0x0099A2E0.
// Identity (sweep1, 2026-10-25; FINDINGS chunk 455 "0x4DD728 is one ... body for ~CTCShotDefinitionProjectile and
//   ~pair<const long,CEnginePrimitiveHandle>"; hand455 README "also serves as ~pair<const long, CEnginePrimitiveHandle>
//   (handle at +4)"): every caller destroys that pair -- the landed map<long, CEnginePrimitiveHandle>::operator[] 0x005BBFD6
//   on its value_type(k, T()) temporary at 0x005BC030 (and it binds this Leaf -> 004dd728 in reloc_bindings), the tree
//   code on node +0x10 (`lea ecx,[esi+10h] ; call`): the recursive _M_erase 0x004E35D4 (at 0x004E35F1) and erase(iterator)
//   0x005BBAF9 (at 0x005BBB1C, after _Rb_tree_rebalance_for_erase 0x0042A34E), the pair's
//   deleting destructor 0x004DFC0B (0x004DFC0E) and a tail-jump stub 0x004E1C38. No caller passes a
//   CTCShotDefinitionProjectile (that class keeps its handle at +0x9C): the manifest name is propagated, and one COMDAT
//   per mangled name explains the callers in several TUs. The landed SPrimitiveHandleHolder row had the same layout.
// Model: the landed row's handle classes (vtable 0x0122F598 = CEnginePrimitiveHandle) unchanged.
// Shape: `and dword ptr [..],0` = a size TU (`#pragma optimize("s", on)`); the last-handle test here is a ref-count
// getter (`Object ? Object->RefCount : 0`, `xor eax,eax` arm) compared with 1, and the virtual call reloads the member
// straight into ecx; `Data = 0` (release plus a second unconditional zero store) and the member destructor (whose null
// test reuses the flags of the `and`) are `__forceinline`.
#pragma optimize("s", on)
class CEnginePrimitiveHandle;

class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    virtual void Destroy();                                          // slot 1 (+0x04)
    virtual void v002();
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle* handle); // slot 3 (+0x0C)
    long RefCount;                                                   // +0x04
    void Release()
    {
        if (--RefCount == 0)
            Destroy();
    }
};

class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* Object;
    long GetRefCount() const { return Object ? Object->RefCount : 0; }
    __forceinline ~CIVCountedPointer()
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
    }
    __forceinline CIVCountedPointer& operator=(int)
    {
        if (Object) {
            Object->Release();
            Object = 0;
        }
        Object = 0;
        return *this;
    }
};

#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class CEnginePrimitiveHandle : public CBaseClass { // vtable 0x0122F598
public:
    CIVCountedPointer Data;                        // +0x04
    virtual void v001();
    __forceinline ~CEnginePrimitiveHandle()
    {
        if (Data.GetRefCount() == 1)
            Data.Object->OnLastHandleReleased(this);
        Data = 0;
    }
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;                                     // +0x00
    _T2 second;                                    // +0x04
    ~pair() { }                                    // the implicit destructor, spelled out so this TU emits it
};
template struct pair<const long, CEnginePrimitiveHandle>;
}
