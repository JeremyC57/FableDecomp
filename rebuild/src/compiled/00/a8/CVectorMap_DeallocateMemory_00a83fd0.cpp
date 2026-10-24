// CVectorMap::DeallocateMemory() -- empty the navigation quad-tree's connector-node map and give its storage back: erase every (id, counted node) entry, swap the array with an empty one, clear Dirty and free the detached block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A83FD0, 103 bytes, thiscall `ret`.
// Identity: Ego_r.pdb `?DeallocateMemory@?$CVectorMap@JV?$CCountedPointer@VCNavConnectorNode@@@@V?$CKeyPairCompareLess@J
//   V?$CCountedPointer@VCNavConnectorNode@@@@@@@@QAEXXZ`; the only caller is CNavQuadTree::ClearNodes 0x00A792E0
//   (`lea ecx,[ebx+78h]`: ConnectorNodes), whose landed source already calls 0x00A83FD0 the connector map's clear. The
//   element is 12 bytes {long id, CCountedPointer<CNavConnectorNode>} (the __destroy_aux 0x00A80E40 releases the counted
//   pointer at +4). The catalog's "Vector_DestroyAndClear_ParticleEmitter" is propagated.
// Callees: __copy(first, last, result, random_access_iterator_tag, int*) 0x00A80DA0 and __destroy_aux(first, last,
//   __false_type) 0x00A80E40 (both /Gr `__fastcall`), free 0x00BFEA14.
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D.
// Shape: STLport `clear()` = `erase(begin(), end())` (`copy(end, end, begin)` -> __copy, `_Destroy(i, finish)`,
//   `_M_finish = i`), then `vector empty; swap(empty);` (the three _STL::swap exchanges leave this zeroed and the old
//   block in registers), `Dirty = false`, and the temporary's destructor at scope end (`_Destroy(start, finish)` and the
//   allocator's `if (p) free(p)`). Every empty tag shares one byte of the `push ecx` frame word.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14

struct CCPPointerInfo {
    unsigned long RefCount;                             // +0x00
    void (__fastcall* DeleteFunc)(void* data);          // +0x04
    void*         Data;                                 // +0x08
};

template <class T> class CCountedPointer {
public:
    T*              Data;                               // +0x00
    CCPPointerInfo* Info;                               // +0x04
};

class CNavConnectorNode;

namespace _STL {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }

template <class _T1, class _T2> class pair {
public:
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                 // 0x00A80DA0
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x00A80E40

template <class _Tp> inline _Tp* copy(_Tp* __first, _Tp* __last, _Tp* __result)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }

template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    vector() : _Vector_base<_Tp, _Alloc>(_Alloc()) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    _Tp* begin() { return this->_M_start; }
    _Tp* end() { return this->_M_finish; }
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy(__last, this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
    void clear() { erase(begin(), end()); }
    void swap(vector& __x)
    {
        _STL::swap(this->_M_start, __x._M_start);
        _STL::swap(this->_M_finish, __x._M_finish);
        _STL::swap(this->_M_end_of_storage, __x._M_end_of_storage);
    }
};
}

template <class K, class V> class CKeyPairCompareLess {
public:
    bool operator()(const _STL::pair<K, V>& a, const _STL::pair<K, V>& b) const { return a.first < b.first; }
};

template <class K, class V, class C> class CVectorMap : public _STL::vector<_STL::pair<K, V> > {
public:
    C    Compare;                                       // +0x0c
    bool Dirty;                                         // +0x0d
    void DeallocateMemory()
    {
        this->clear();
        _STL::vector<_STL::pair<K, V> > empty;
        this->swap(empty);
        Dirty = false;
    }
};

template void CVectorMap<long, CCountedPointer<CNavConnectorNode>, CKeyPairCompareLess<long, CCountedPointer<CNavConnectorNode> > >::DeallocateMemory();