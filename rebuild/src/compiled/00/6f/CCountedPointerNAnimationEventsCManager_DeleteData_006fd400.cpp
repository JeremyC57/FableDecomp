// CCountedPointer<NAnimationEvents::CManager>::DeleteData(void* data) -- the counted pointer's per-type deleter: destroy the animation-event manager (extra-events map, symbol map) and free it
// Leaf: ?DeleteData@?$CCountedPointer@VCManager@NAnimationEvents@@@@SIXPAX@Z
// Retail: Fable.exe @ 0x006FD400, 60 bytes, static __fastcall (data in ecx), `ret`.
// Identity (probable): the CCountedPointer<T>::DeleteData shape (landed CCountedPointer<CCardinalSpline>::DeleteData
//   0x0075F830: `delete (T*)data`); it is stored as the count block's deleter by the landed CCountedPointer Reset
//   0x006FD460 (`mov [eax+4], 6FD400h`). The object is Ego_r NAnimationEvents::CManager (0x4C; retail 0x40):
//   +4 MeshDataBank, CSymbolMap EventSymbolMap +8 (out-of-line ~CSymbolMap 0x006FD040, landed as
//   CStringKeyedTable_006fd040), then AnimToExtraEventsMap +0x2C = CVectorMap<unsigned long, CSmallVector<CEvent,32>>
//   whose element destructor runs through the out-of-line __destroy_aux 0x006FB8A0 (landed: pair<unsigned long,
//   CSmallVector<NAnimationEvents::CEvent,32> >) before the storage is freed. Members destroy in reverse order.
// Callees: __destroy_aux 0x006FB8A0, free 0x00BFEA14, ~CSymbolMap 0x006FD040, operator delete 0x00BFE9BC.
extern "C" void __cdecl free(void*);                       // 0x00BFEA14
void __cdecl operator delete(void* p);                     // 0x00BFE9BC

namespace std {
struct __false_type { __false_type() {} };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x006FB8A0
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    __forceinline ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    __forceinline ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    ~pair();
};
}

namespace NAnimationEvents {
class CEvent;
}
template <class T, int N> class CSmallVector { public:
    ~CSmallVector();
};
template <class K, class V> class CKeyPairCompareLess { };
template <class K, class V, class C> class CVectorMap { public:
    std::vector<std::pair<K, V> > Vector;                  // +0x00
    C    Compare;                                          // +0x0C
    bool Dirty;                                            // +0x0D
};
class CSymbolMap { public:                                 // retail 0x24 (Ego_r 0x2C)
    unsigned char _body[0x24];
    ~CSymbolMap();                                         // 0x006FD040
};
class CMeshDataBank;

namespace NAnimationEvents {
typedef CSmallVector<CEvent, 32> TEvents;
class CManager { public:                                   // retail 0x40 (Ego_r.pdb 0x4C)
    unsigned long  _word00;                                // +0x00 (unnamed in Ego_r)
    CMeshDataBank* MeshDataBank;                           // +0x04
    CSymbolMap     EventSymbolMap;                         // +0x08
    CVectorMap<unsigned long, TEvents, CKeyPairCompareLess<unsigned long, TEvents> > AnimToExtraEventsMap;   // +0x2C
    bool           DifferentToSavedCopy;                   // +0x3C
    bool           UseEmbeddedEvents;                      // +0x3D
};
}

class CCPPointerInfo;
template <class T>
class CCountedPointer {
public:
    T*              Data;                                  // +0x00
    CCPPointerInfo* Info;                                  // +0x04
    static void __fastcall DeleteData(void* data) { delete (T*)data; }
};

template class CCountedPointer<NAnimationEvents::CManager>;