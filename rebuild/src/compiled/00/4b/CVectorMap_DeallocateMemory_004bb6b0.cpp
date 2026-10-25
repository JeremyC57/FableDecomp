// CVectorMap::DeallocateMemory() -- empty the quest manager's persistence map (quest name -> persist info) and give its storage back: erase every entry, swap the array with an empty one, clear Dirty and free the detached block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BB6B0, 103 bytes (+ int3 padding), thiscall `ret`.
// Identity: Ego_r.pdb ?DeallocateMemory@?$CVectorMap@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@
//   V?$CKeyPairCompareLess@VCCharString@@VCPersistInfo@CRuntimeQuestPersistence@NQuestManager@@@@@@QAEXXZ; the two
//   callers are NQuestManager::CRuntimeQuestPersistence::ClearAll 0x004B41F0 (landed; 0x004B421F, 0x004B4252). Byte twin
//   of the landed CVectorMap<long, CCountedPointer<CNavConnectorNode> >::DeallocateMemory 0x00A83FD0 (same 12-byte entry
//   size). The catalog's "std::vector_Swap_And_Deallocate" is descriptive.
// Callees: __copy(first, last, result, random_access_iterator_tag, int*) 0x004B84F0 and __destroy_aux(first, last,
//   __false_type) 0x004B93E0 (both /Gr `__fastcall`; manifest `_Move<pair<CCharString,CPersistInfo>*>` / `_Destroy`), free
//   0x00BFEA14.
// Layout: CVectorMap = STLport vector<pair<K, V> > (12 bytes) + empty Compare +0x0C + Dirty +0x0D.
// Shape: STLport `clear()` = `erase(begin(), end())` (`copy(end, end, begin)` -> __copy, `_Destroy(i, finish)`,
//   `_M_finish = i`), then `vector empty; swap(empty);` (the three _STL::swap exchanges leave this zeroed and the old
//   block in registers), `Dirty = false`, and the temporary's destructor at scope end (`_Destroy(start, finish)` and the
//   allocator's `if (p) free(p)`). Every empty tag shares one byte of the `push ecx` frame word.
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (MSVCR71 import thunk)

class CCharString {
public:
    void* PStringData;                                  // the shared string block
    CCharString(const CCharString& other);              // 0x0099EC30
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

namespace NQuestManager {
class CRuntimeQuestPersistence {
public:
    class CPersistInfo {                                // Ego_r.pdb, 8 bytes
    public:
        CCharString QuestName;                          // +0x00
        CCharString PersistScript;                      // +0x04
    };
};
}

namespace _STL {

struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> inline void swap(_Tp& __a, _Tp& __b) { _Tp __tmp = __a; __a = __b; __b = __tmp; }

template <class _T1, class _T2> struct pair {
    _T1 first;                                          // +0x00
    _T2 second;                                         // +0x04
};

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);                 // 0x004B84F0
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x004B93E0

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

template <class K, class V> class CKeyPairCompareLess { // Ego_r.pdb: empty
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

typedef NQuestManager::CRuntimeQuestPersistence::CPersistInfo TPersistInfo;
template void CVectorMap<CCharString, TPersistInfo, CKeyPairCompareLess<CCharString, TPersistInfo> >::DeallocateMemory();