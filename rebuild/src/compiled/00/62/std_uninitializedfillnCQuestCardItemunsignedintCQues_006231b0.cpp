// std::__uninitialized_fill_n<CQuestCardItem*, unsigned int, CQuestCardItem>(first, n, x, const __false_type&) -- STLport
// placement fill of n quest-card items (item index plus a wide-string copy); returns the end of the filled range
// Leaf: ??$__uninitialized_fill_n@PAVCQuestCardItem@@IV1@@std@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x006231B0, 50 bytes, __fastcall (first in ecx, n in edx, x and the tag reference on
//   the stack), `ret 8`.
// Renamed from the collision placeholder _global::UninitFillN_SPairLongCSecond_0099b720_006231b0 (descriptive, "one of 13
//   bodies"; the by-value allocator parameter it modelled is STLport's `const __false_type&`).
// Identity: the only caller is std::vector<CQuestCardItem>::_M_insert_overflow 0x00624B90 (call at 0x00624C24, its
//   fill_len != 1 branch), whose element is Ego_r.pdb CQuestCardItem (long ItemIndex +0, CWideString ItemText +4); no
//   identical-code folding, so this body is that vector's helper. Callee: CWideString copy ctor 0x0099B720 (ItemText).
// Shape: `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` with the unsigned count tested by `jbe`; the implicit
//   CQuestCardItem copy constructor stores the index, then copies the string; the placement new null-tests the slot.
inline void* operator new(unsigned int, void* p) { return p; }

class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);                         // 0x0099B720
};

class CQuestCardItem {                                             // 8 bytes (Ego_r.pdb)
public:
    long        ItemIndex;                                         // +0x00
    CWideString ItemText;                                          // +0x04
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template CQuestCardItem* __fastcall __uninitialized_fill_n<CQuestCardItem*, unsigned int, CQuestCardItem>(
    CQuestCardItem*, unsigned int, const CQuestCardItem&, const __false_type&);

}
