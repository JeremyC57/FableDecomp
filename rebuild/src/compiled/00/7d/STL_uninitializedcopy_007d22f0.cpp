// _STL::__uninitialized_copy(const CShoveHaPennyRegionDef* first, const CShoveHaPennyRegionDef* last, CShoveHaPennyRegionDef* result, const __false_type&) -- STLport copy-construct a range of shove-ha'penny region defs into raw storage (base def copy, vptr, Start, Score); returns the end of the copies
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D22F0, 64 bytes, /Gr __fastcall (first in ecx, last in edx, result and the
//   tag reference on the stack, `ret 8`).
// Identity: the 48-byte element's copy writes vtable 0x0124383C, which rtti-vtables.tsv gives to
//   CShoveHaPennyRegionDef (Ego_r.pdb: 48 bytes, CDefClassBase then float Start +0x28, float Score +0x2c). Callers are
//   the vector<CShoveHaPennyRegionDef> growth paths 0x007D23B0 / 0x007D24F0. The catalog label
//   "_Copy_backward<CBrainUpdateZone*,CBrainUpdateZone*>" (and the callers' "CBrainUpdateZone" labels) are propagated.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C.
// Layout: local model (engine/CDefClassBase.h is the flattened POD view; the copy needs the polymorphic class).
// Shape: STLport `for (; __first != __last; ++__first, ++__cur) _Construct(&*__cur, *__first); return __cur;` (no EH:
//   the try/unwind is compiled out) -- placement-new null test, the implicit copy constructor (base copy call, vptr,
//   the two floats moved as dwords).
inline void* operator new(unsigned int, void* p) { return p; }

class CDefClassBase {
public:
    virtual ~CDefClassBase();
    unsigned char _pad04[0x24];                         // +0x04 .. +0x27 (PDefManager +0x1c, GlobalIndex +0x20, flags +0x24)
    CDefClassBase(const CDefClassBase& other);          // 0x0046644C
};

class CShoveHaPennyRegionDef : public CDefClassBase {   // 48 bytes, vtable 0x0124383C
public:
    virtual ~CShoveHaPennyRegionDef();
    float Start;                                        // +0x28
    float Score;                                        // +0x2c
};

namespace _STL {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template CShoveHaPennyRegionDef* __fastcall __uninitialized_copy<const CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*>(
    const CShoveHaPennyRegionDef*, const CShoveHaPennyRegionDef*, CShoveHaPennyRegionDef*, const __false_type&);
}