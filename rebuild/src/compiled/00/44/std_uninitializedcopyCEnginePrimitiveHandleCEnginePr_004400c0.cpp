// std::__uninitialized_copy<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const __false_type&) -- copy-construct [first, last) into raw storage, return the end
// Leaf: ??$__uninitialized_copy
// Retail: Fable.exe @ 0x004400C0, 51 bytes, __fastcall (first in ecx, last in edx, result + the __false_type tag on the
// stack, `ret 8`), returns the end in eax; callers 0x0044141F, 0x00441473, 0x004423BA, 0x00442417, 0x00BC5E79,
// 0x00BC5EE1. Catalog label std::_Uninit_copy<_Vector_const_iterator<CScriptThing>, ...> is wrong: the 8-byte elements
// carry vptr 0x0122F598 (COL .?AVCEnginePrimitiveHandle@@ : CBaseClass) and are built by the inline
// CEnginePrimitiveHandle copy constructor behind a placement-new null test -- the copy counterpart of the fill_n
// instance 0x0043F6C0.
// Layout: Ego_r.pdb CEnginePrimitiveHandle (8 bytes: vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04).
// Shape: /O2 (`lea esp,[esp]` loop-head pad; first == last tested before entry).
#include <new>
class CBaseClass {
public:
    virtual ~CBaseClass();
};
class CEnginePersistentPrimitiveBase {
public:
    virtual void v000();
    long RefCount;                                       // +0x04
};
class CIVCountedPointer {                                // CIVCountedPointer<CEnginePersistentPrimitiveBase>
public:
    CEnginePersistentPrimitiveBase* P;
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
class CEnginePrimitiveHandle : public CBaseClass {       // vtable 0x0122F598, 8 bytes
public:
    CIVCountedPointer Data;                              // +0x04
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), Data(x.Data) {}
    virtual void v001();
};
namespace std {
struct __false_type {};
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
template <class InputIter, class ForwardIter>
ForwardIter __fastcall __uninitialized_copy(InputIter first, InputIter last, ForwardIter result, const __false_type&)
{
    ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template CEnginePrimitiveHandle* __fastcall __uninitialized_copy<CEnginePrimitiveHandle*, CEnginePrimitiveHandle*>(CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, CEnginePrimitiveHandle*, const __false_type&);
}