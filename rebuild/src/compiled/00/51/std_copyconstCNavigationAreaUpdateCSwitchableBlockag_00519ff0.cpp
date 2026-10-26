// std::__copy<const CNavigationAreaUpdate::CSwitchableBlockage*, CNavigationAreaUpdate::CSwitchableBlockage*, int>(first, last, result, const random_access_iterator_tag&, int*) -- STLport random-access copy of switchable blockages: UniqueID words then list::operator=, n = last - first times
// Leaf: ??$__copy@PBVCSwitchableBlockage@CNavigationAreaUpdate@@PAV12@H@std@@YIPAVCSwitchableBlockage@CNavigationAreaUpdate@@PBV12@0PAV12@ABUrandom_access_iterator_tag@0@PAH@Z
//
// Retail 0x00519FF0, 67 bytes (+ int3 padding to 80), `ret 0Ch` (first in ecx, last in edx; result, the iterator-category tag and the
//   _Distance* tag on the stack).  Callee: 0x00517720 list<C2DLineF>::operator=(const list&) (thiscall, `ret 4`).
// Element: Ego_r.pdb CNavigationAreaUpdate::CSwitchableBlockage {unsigned __int64 UniqueID +0; list<C2DLineF> LineList +8}
//   (16 bytes, +0xC padding). Earlier landed as the CNavQuadTree pair<unsigned __int64, list<C2DLineF> >, whose instance
//   is this body's byte twin 0x00A82000 in the CNavQuadTree TU; this one is in the area-update TU with the blockage vector's
//   code (its callers are vector<CSwitchableBlockage>::operator= 0x0051BCD0 x2, which binds
//   `__copy<const CSwitchableBlockage*, CSwitchableBlockage*, int>` here -- STLport's operator= copies from
//   `(const_pointer)__x._M_start` --, and the __copy_ptrs thunk 0x0051ADA0). No ICF in retail: two bodies, two instances.
// Shape: STLport `for (_Distance n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result;`
//   with the blockage's implicit operator= (two UniqueID dwords, then the list's out-of-line operator=).  `last - first` is the
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
template <class T> class allocator { };
template <class T, class _Alloc = allocator<T> > class list {
public:
    _List_node_base* _M_node;                  // the sentinel (alloc proxy data)
    list<T, _Alloc>& operator=(const list<T, _Alloc>& x);   // 0x00517720
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

class CNavigationAreaUpdate {
public:
    class CSwitchableBlockage {                     // Ego_r.pdb (retail 0x10 bytes)
    public:
        unsigned __int64     UniqueID;              // +0x00
        std::list<C2DLineF>  LineList;              // +0x08
    };
};
typedef CNavigationAreaUpdate::CSwitchableBlockage CSwitchableBlockage;
template CSwitchableBlockage* __fastcall std::__copy<const CSwitchableBlockage*, CSwitchableBlockage*, int>(
    const CSwitchableBlockage*, const CSwitchableBlockage*, CSwitchableBlockage*, const std::random_access_iterator_tag&, int*);