// CArray<CMapInfo>::DeallocateMemory() -- release the storage of the world map's 72-byte map-info array: erase every element, detach and zero the three pointers, destroy the detached range and free the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051A350, length 123 bytes (+ int3 padding); the only caller is
// CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 (the manifest's CArray<std::pair<unsigned long,...>> label is propagated;
// the 0x38E38E39 / `sar 4` reciprocal is the 72-byte stride of CMapInfo, the CWorldMap::Maps element of landed 0x005022F0).
// Callees: SGI __copy<CMapInfo*> 0x005150E0 (landed; __fastcall: first in ecx, last in edx, result + the iterator tag + the
// Distance* on the stack), the range destroy 0x00515250 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the landed CArray<SElement12>::DeallocateMemory 0x009B5F30 form with a non-trivial element: erase(begin(), end())
// is `copy(end, end, begin)` + the destroy of the left-over tail + the finish store; the detach goes through `Exchange(slot, 0)`
// (a store through a `T*&` keeps the member reload after it); both empty tags are one-byte temporaries homed in the
// `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CMapInfo {                                       // 0x48 bytes
    unsigned char _body[0x48];
};
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
struct __false_type { __false_type() { } };

CMapInfo* __fastcall Copy_CMapInfo(CMapInfo* first, CMapInfo* last, CMapInfo* result,
                                   const SRandomAccessIteratorTag& category, long* distance);   // 0x005150E0
void __fastcall DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type& tag);           // 0x00515250

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void erase(T* f, T* l)
    {
        T* i = Copy_CMapInfo(l, _M_finish, f, SRandomAccessIteratorTag(), (long*)0);
        DestroyRange(i, _M_finish, __false_type());
        _M_finish = i;
    }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<CMapInfo>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        CMapInfo* p = Exchange(_M_start, (CMapInfo*)0);
        CMapInfo* l = Exchange(_M_finish, (CMapInfo*)0);
        _M_end_of_storage = 0;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}