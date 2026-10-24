// std::__uninitialized_fill_n<CModifierGraphic*,unsigned int,CModifierGraphic>(first, n, x, const __false_type&) -- placement-copy one hero appearance modifier graphic into n raw slots, return the end of the built range
// Leaf: ??$__uninitialized_fill_n@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708BA0, length 53 bytes. `__fastcall` (/Gr algorithm TU): first in ecx,
//         n in edx, the value + the tag reference on the stack, `ret 8`.
// Callees: none (the CDefPointer copy constructor is inline).
// Identity: the manifest label `_Fill_n<unsigned_long*,unsigned_int,unsigned_long>` is propagated across a byte-identical
//   body. Its caller is vector<CModifierGraphic>::_M_insert_overflow (0x00709840, the fill_len != 1 branch), whose
//   element is the 12-byte Ego_r.pdb CTCHeroAttachableAppearanceModifiers::CModifierGraphic (CDefPointer<const
//   CTextureReplacementDef> +0, BankIndex +4, RenderSizeX +8); sibling of the __uninitialized_copy row 0x00708B60.
// Shape: STLport 4.x `__uninitialized_fill_n(first, n, x, __false_type)`: `for (; n > 0; --n, ++cur) _Construct(&*cur, x)`
//   with an unsigned size (`test edx,edx ; jbe`, then the `dec ; jne` count), placement new keeps its null test, return
//   cur. The implicit copy constructor runs the counted def pointer's copy (null test, `inc [obj+4]`) + two plain words.
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

struct CDefCountedBase {                        // counted def: vtable +0, reference count +4
    virtual void Destroy(int flags);
    unsigned long RefCount;                     // +0x04
};

template <class T>
struct CDefPointer {                            // 4-byte intrusive pointer
    CDefCountedBase* Object;                    // +0x00
    CDefPointer(const CDefPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->RefCount;
    }
};

struct CTextureReplacementDef;

namespace CTCHeroAttachableAppearanceModifiers {
struct CModifierGraphic {                       // Ego_r.pdb, 12 bytes
    CDefPointer<const CTextureReplacementDef> PTextureReplacement;   // +0x00
    long  BankIndex;                            // +0x04
    float RenderSizeX;                          // +0x08
};
}
using CTCHeroAttachableAppearanceModifiers::CModifierGraphic;

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

namespace std {
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template CModifierGraphic* __fastcall __uninitialized_fill_n(CModifierGraphic*, unsigned int, const CModifierGraphic&, const __false_type&);
}