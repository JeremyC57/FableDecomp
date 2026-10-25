// _global::Copy_SListHandle04_00517720_00a82650(const list<C2DLineF>* first, const list<C2DLineF>* last, list<C2DLineF>* result, const random_access_iterator_tag&, int*) -- STLport __copy over list<C2DLineF>: counted forward assignment of the one-word lists, returns the advanced result
// Leaf: Copy_SListHandle04_00517720_00a82650
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A82650, length 53 bytes, `ret 0Ch`.
// Callees: list<C2DLineF>::operator= @ 0x00517720 (catalog "std::list::operator=").
// Shape: STLport 4.x `__copy(first, last, result, const random_access_iterator_tag&, _Distance*)` -- the signed element
// count `last - first` is computed once (sar 2 (/4)), then one out-of-line assignment per element (`push esi ; mov ecx,edi ; call`),
// both pointers stepped by 0x4; the advanced result is returned (the untouched result for an empty range).
// /Gr algorithm TU: __fastcall, first in ecx, last in edx, result + the tag reference + the null distance pointer on
// the stack (callers push `0`, `lea` of a dead slot, result).
// Element: list<C2DLineF>, the one-word STLport list of CNavigationLayer::LineLists (Ego_r.pdb vector<list<C2DLineF> >);
//   0x00517720 is its operator= (agent422 NOTES: list<C2DLineF> operator= / destructor / range insert are 0x00517720 /
//   0x005127A0 / 0x005152C0). No direct callers, so whether `first` is the const or the non-const iterator instantiation is
//   not pinned and the descriptive function name is kept.
// Label correction: the catalog's `_Copy_impl<std::pair<CCharString,CCharString>*,...>` is propagated.
class C2DVector {
public:
    float X, Y;
};
class C2DLineF {                                   // 16 bytes (Ego_r.pdb)
public:
    C2DVector Point1;                              // +0x00
    C2DVector Point2;                              // +0x08
};

namespace std {
template <class _Tp> class allocator { };

template <class _Tp, class _Alloc = allocator<_Tp> >
class list {                                       // STLport 4.6: one word, the sentinel node pointer
public:
    void* _M_node;
    list<_Tp, _Alloc>& operator=(const list<_Tp, _Alloc>& __x);   // 0x00517720
};

struct random_access_iterator_tag { };
}

typedef std::list<C2DLineF> CLineList;

CLineList* __fastcall Copy_SListHandle04_00517720_00a82650(const CLineList* first, const CLineList* last, CLineList* result, const std::random_access_iterator_tag& category, int* distance)
{
    for (int n = int(last - first); n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
