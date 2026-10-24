// std::__uninitialized_copy<CModifierGraphic*,CModifierGraphic*>(first, last, result, const __false_type&) -- placement-copy [first, last) of hero appearance modifier graphics into raw storage, return the end of the built range
// Leaf: ??$__uninitialized_copy@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00708B60, length 57 bytes (+ int3 padding). `__fastcall` (/Gr algorithm TU):
//         first in ecx, last in edx, result + the tag reference on the stack, `ret 8`.
// Callees: none (the CDefPointer copy constructor is inline).
// Identity: the manifest label `_Copy_impl<CNavigationPosition*,CNavigationPosition*>` is propagated across a byte-identical
//   body. Both callers sit in vector<CModifierGraphic>::_M_insert_overflow (0x00709840: copy the head, then the tail when
//   not appending), whose element is the 12-byte Ego_r.pdb CTCHeroAttachableAppearanceModifiers::CModifierGraphic
//   (CDefPointer<const CTextureReplacementDef> +0, BankIndex +4, RenderSizeX +8) -- the same model as the landed
//   `__copy` sibling 0x00708C20.
// Shape: STLport 4.x `__uninitialized_copy(first, last, result, __false_type)`: `for (; first != last; ++first, ++cur)
//   _Construct(&*cur, *first)` (placement new keeps its null test), return cur. The implicit copy constructor runs the
//   counted def pointer's user copy constructor (copy, null test, `inc [obj+4]`) and copies the two plain words.
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
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template CModifierGraphic* __fastcall __uninitialized_copy(CModifierGraphic*, CModifierGraphic*, CModifierGraphic*, const __false_type&);
}