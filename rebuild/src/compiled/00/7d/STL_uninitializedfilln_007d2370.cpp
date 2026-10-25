// std::__uninitialized_fill_n(CShoveHaPennyRegionDef* first, unsigned int n, const CShoveHaPennyRegionDef& x, const __false_type&) -- copy-construct n shove-ha'penny region definitions into raw storage at first and return first + n
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007D2370, 60 bytes, __fastcall (first ecx, n edx; x and the empty tag on the stack, `ret 8`).
// Callers: 0x007D245D, 0x007D24DA (the vector's grow path 0x007D23B0) and 0x007D25C0 (its fill-insert 0x007D24F0).
// Element: Ego_r.pdb CShoveHaPennyRegionDef (0x30 bytes): the CDefClassBase base (vptr, base subobject, PDefManager +0x1C,
//   GlobalIndex +0x20, the DefaultValsApplied bitfield +0x24; 0x28 bytes with padding) then float Start +0x28, float Score
//   +0x2C. Its implicit copy constructor calls CDefClassBase's out-of-line copy constructor 0x0046644C, stores the
//   CShoveHaPennyRegionDef vtable (0x0124383C) and copies the two floats as dwords.
// Model: local -- the generated engine/CDefClassBase.h is pack(1) at 0x25 bytes, which would put Start at +0x25; retail
//   has it at +0x28.
// Label: catalog "vector<unsigned __int64>::_Ufill" is propagated across the byte-identical-shaped STLport helpers.
// Shape: STLport 4.6 __uninitialized_fill_n verbatim (`for (; __n > 0; --__n, ++__cur) _Construct(&*__cur, __x);`, unsigned
//   n so `jbe`), the placement new keeping its null test (`test esi,esi ; je`).
inline void* operator new(unsigned int, void* p) { return p; }

class CDefinitionManager;

class CDefClassBase {
public:
    virtual ~CDefClassBase();                   // +0x00 vptr
    unsigned char       BaseData[0x18];         // +0x04 (base-class subobject)
    CDefinitionManager* PDefManager;            // +0x1C
    unsigned long       GlobalIndex;            // +0x20
    unsigned char       DefaultValsApplied : 1; // +0x24
    CDefClassBase(const CDefClassBase& other);  // 0x0046644C
};

class CShoveHaPennyRegionDef : public CDefClassBase {
public:
    float Start;                                // +0x28
    float Score;                                // +0x2C
};

namespace std {
struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CShoveHaPennyRegionDef* __fastcall __uninitialized_fill_n(CShoveHaPennyRegionDef*, unsigned int,
                                                                   const CShoveHaPennyRegionDef&, const __false_type&);
}