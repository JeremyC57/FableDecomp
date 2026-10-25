// std::__destroy_aux<CShopItemDef*>(first, last, const __false_type&) -- destroy each CShopItemDef in [first, last)
// Leaf: ??$__destroy_aux@PAVCShopItemDef@@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004DA5BD, 28 bytes, `ret 4` (/Gr: ecx = first, edx = last, the tag on the stack).
// Identity: STLport `__destroy_aux(first, last, __false_type)`: one `_Destroy(&*first)` per element, i.e. a call of
// the virtual destructor (slot 0, the scalar deleting destructor with flag 0) at a 0x48 stride. Callers: the
// vector<CShopItemDef> _M_clear 0x004E9BB7 and that vector's _M_insert_overflow; the element is CShopItemDef (the vector's
// copy-construct helper 0x004E9C39 stores vtable 0x0123B644 = `.?AVCShopItemDef@@`). The old DestroyRange_Elem / vector _Destroy labels
// were descriptive / propagated.
#pragma optimize("s", on)

class CShopItemDef {
public:
    virtual ~CShopItemDef();
    unsigned char _body[0x44];
};

namespace std {
struct __false_type { };

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template void __fastcall __destroy_aux<CShopItemDef*>(CShopItemDef*, CShopItemDef*, const __false_type&);
}
