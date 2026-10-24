// __copy(pair<unsigned __int64, list<C2DLineF> >* first, pair<unsigned __int64, list<C2DLineF> >* last, pair<unsigned __int64, list<C2DLineF> >* result, const random_access_iterator_tag&, int*) -- STLport random-access copy of CNavigationLayer line-list records: key words then list::operator=, n = last - first times
// Leaf: ??$__copy@PAU?$pair@_KV?$list@VC2DLineF@@
//
// Retail 0x00519FF0, 67 bytes (+ int3 padding to 80), `ret 0Ch` (first in ecx, last in edx; result, the iterator-category tag and the
//   _Distance* tag on the stack).  Callee: 0x00517720 list<C2DLineF>::operator=(const list&) (thiscall, `ret 4`).
// Element: the 16-byte (unsigned __int64 key, list<C2DLineF>) record of Ego_r.pdb CNavigationLayer's
//   CVectorMap<unsigned __int64, list<C2DLineF>, ...> (see 0x005199E0; +0xC is padding after the list).
// Shape: STLport `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`
//   with pair's implicit operator= (two key dwords, then the list's out-of-line operator=).  `last - first` is the
//   `sar 4` byte difference; n counts down in ebx (`dec ebx / jne`).  The retail TU is /Gr (explicit __fastcall here).
// Label: catalog "_Move<CEngineTextureMorph*,CEngineTextureMorph*>" (propagated Dinkumware label).
class C2DVector {
public:
    float X;                                   // +0x0
    float Y;                                   // +0x4
    C2DVector(const C2DVector& v) : X(v.X), Y(v.Y) {}   // FableWin.pdb ??0C2DVector@@QAE@ABV0@@Z
};
class C2DLineF {
public:
    C2DVector Point1;                          // +0x0
    C2DVector Point2;                          // +0x8
};

namespace std {
struct random_access_iterator_tag {};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class T> class list {
public:
    _List_node_base* _M_node;                  // the sentinel (alloc proxy data)
    list<T>& operator=(const list<T>& x);      // 0x00517720
};
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter first, _RandomAccessIter last, _OutputIter result,
                              const random_access_iterator_tag&, _Distance*)
{
    for (_Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
}

typedef std::pair<unsigned __int64, std::list<C2DLineF> > CLineListRecord;
template CLineListRecord* __fastcall std::__copy<CLineListRecord*, CLineListRecord*, int>(
    CLineListRecord*, CLineListRecord*, CLineListRecord*, const std::random_access_iterator_tag&, int*);