// CCountedPointer::DeleteData(void* data) -- (CCountedPointer<CRegionConnectionGraph>) the counted pointer's per-type deleter: destroy the graph (its node map's counted pointers, then the vector block) and free the object
// Leaf: ?DeleteData@?$CCountedPointer@VCRegionConnectionGraph@@@@SIXPAX@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051CCD0, 50 bytes, `ret` (static `__fastcall`, data in ecx; `push ecx` frame
//   for the __false_type tag).
// Identity (proven): stored as the count block's deleter by the landed CCountedPointer<...>::Reset 0x0051CEA0 (0x0051CEED),
//   whose only caller is CWorldMap::LoadRegionGraph 0x00506D40 -- Ego_r.pdb CWorldMap::PRegionGraph is
//   CCountedPointer<CRegionConnectionGraph>; CRegionConnectionGraph's only member is GraphNodes, a
//   CVectorMap<long, CCountedPointer<CRegionConnectionNode>, CKeyPairCompareLess<...> >, whose element destroyer is the
//   landed std::__destroy_aux<pair<long, CCountedPointer<CRegionConnectionNode> >*> 0x00519050 called here. Ego_r.pdb names
//   ?DeleteData@?$CCountedPointer@VCRegionConnectionGraph@@@@SIXPAX@Z.
// Callees: std::__destroy_aux<pair<long, CCountedPointer<CRegionConnectionNode> >*> 0x00519050 (__fastcall: first ecx, last
//   edx, the tag by address), free 0x00BFEA14, operator delete 0x00BFE9BC.
// Shape: `delete (T*)data` (as the landed CCountedPointer<CCardinalSpline>::DeleteData 0x0075F830): the null test, the
//   implicit ~CRegionConnectionGraph -> ~CVectorMap -> ~vector (`_Destroy` through the out-of-line __destroy_aux, then
//   ~_Vector_base's null-tested free), then operator delete. The empty __false_type tag needs a user constructor (else
//   VC7.1 zero-stores its byte).
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

namespace std {
struct __true_type { };
struct __false_type { __false_type() { } };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00519050
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp>
class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
} // namespace std

template <class T> class CArray : public std::vector<T> { };

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    ~CCountedPointer();                                    // the release (inline in __destroy_aux)
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template <class K, class V> class CKeyPairCompareLess { public: };

template <class K, class V, class TCompare> class CVectorMap {   // sorted-on-demand vector map, retail 0x10 bytes
public:
    CArray<std::pair<K, V> > Data;                         // +0x00
    TCompare                  Compare;                     // +0x0C (empty comparator)
    bool                      Dirty;                       // +0x0D
};

class CRegionConnectionNode;

class CRegionConnectionGraph {                             // retail 0x10 bytes (Ego_r.pdb 0x14 with a Dinkumware vector)
public:
    CVectorMap<long, CCountedPointer<CRegionConnectionNode>,
               CKeyPairCompareLess<long, CCountedPointer<CRegionConnectionNode> > > GraphNodes;   // +0x00
};

template class CCountedPointer<CRegionConnectionGraph>;