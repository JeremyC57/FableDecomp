// CArray<pair<CCharString,CSymbolInfo>>::DeallocateMemory() -- release the symbol-map entry array's storage: erase every element, detach and zero the three pointers, destroy the detached range and free the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7680, length 111 bytes.
// Label correction: the catalog's "Vector_Clear" is CArray<pair<CCharString, CSymbolMap::CSymbolInfo> >::DeallocateMemory, called
//   only by the landed CArray::DeallocateUnusedMemory 0x009E7C70 (which names 0x009E7680 so).
// Callees: STLport __copy @ 0x009E6600 (landed; `__fastcall` first/last in ecx/edx, result + random_access_iterator_tag + a
//   null Distance* on the stack), __destroy_aux @ 0x006FBCC0 (`__fastcall`, three CCharString destructors per element), free
//   @ 0x00BFEA14.
// Shape: the landed CArray::DeallocateMemory template (0x006FC620 / 0x009D3D90 form): `capacity() > 0` (`jbe`), erase(begin,
//   end) = `copy(end, end, begin)` + the destroy of the left-over tail + the finish store, the detach through `Exchange(slot,
//   0)` (a store through `T*&` keeps the member reload after it), then destroy + null-tested free; both empty tags are
//   one-byte temporaries homed in the `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

class CCharString {
public:
    void* PStringData;
};

class CSymbolMap {
public:
    class CSymbolInfo {                           // 12 bytes
    public:
        CCharString GroupName;                    // +0x00
        CCharString EntryName;                    // +0x04
        long        Val;                          // +0x08
    };
};

namespace std {
template <class _T1, class _T2>
struct pair {
    _T1 first;
    _T2 second;
};
struct random_access_iterator_tag { random_access_iterator_tag() {} };
struct __false_type { __false_type() {} };

template <class _RandomAccessIter, class _OutputIter, class _Distance>
_OutputIter __fastcall __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                              const random_access_iterator_tag&, _Distance*);              // 0x009E6600
template <class _ForwardIterator>
void __fastcall __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&);   // 0x006FBCC0
}

typedef std::pair<CCharString, CSymbolMap::CSymbolInfo> SSymbolEntry;

template <class T>
class CArray {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void erase(T* f, T* l)
    {
        T* i = std::__copy(l, _M_finish, f, std::random_access_iterator_tag(), (int*)0);
        std::__destroy_aux(i, _M_finish, std::__false_type());
        _M_finish = i;
    }
    void DeallocateMemory();
};

template <class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template <> void CArray<SSymbolEntry>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        SSymbolEntry* p = Exchange(_M_start, (SSymbolEntry*)0);
        SSymbolEntry* l = Exchange(_M_finish, (SSymbolEntry*)0);
        _M_end_of_storage = 0;
        std::__destroy_aux(p, l, std::__false_type());
        if (p)
            free(p);
    }
}