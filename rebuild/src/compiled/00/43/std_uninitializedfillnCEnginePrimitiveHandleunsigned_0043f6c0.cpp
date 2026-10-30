// std::__uninitialized_fill_n<CEnginePrimitiveHandle*, unsigned int, CEnginePrimitiveHandle>(CEnginePrimitiveHandle*, unsigned int, const CEnginePrimitiveHandle&, const __false_type&) -- copy-construct n handles, return the end
// Leaf: ??$__uninitialized_fill_n
// Retail: Fable.exe @ 0x0043F6C0, 49 bytes, __fastcall (first in ecx, n in edx, &x and the __false_type tag on the stack,
// `ret 8`), returns first + n; callers 0x00441459 and 0x00485D66 (the latter builds the x temporary with vptr 0x0122F598
// right before the call). Catalog label std::_Uninit_fill_n<pair<unsigned __int64, CCountedPointer<..>>*, ...> is wrong:
// the 8-byte element is CEnginePrimitiveHandle (vtable 0x0122F598, COL .?AVCEnginePrimitiveHandle@@ : CBaseClass), built
// by its inline copy constructor behind a placement-new null test -- vptr, then the CIVCountedPointer copy (Data +0x04,
// `++Data->RefCount` at +0x04 when non-null).
// Layout: Ego_r.pdb CEnginePrimitiveHandle (8 bytes: vptr, CIVCountedPointer<CEnginePersistentPrimitiveBase> Data +0x04),
// as the landed models of the class (vtable 0x0122F598).
// Shape: /O2 (`lea ecx,[ecx]` loop-head pad); n is unsigned (`test edx,edx ; jbe`).
#include <new>
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
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
template <class ForwardIter, class Size, class T>
ForwardIter __fastcall __uninitialized_fill_n(ForwardIter first, Size n, const T& x, const __false_type&)
{
    ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
template CEnginePrimitiveHandle* __fastcall __uninitialized_fill_n<CEnginePrimitiveHandle*, unsigned int, CEnginePrimitiveHandle>(CEnginePrimitiveHandle*, unsigned int, const CEnginePrimitiveHandle&, const __false_type&);
}