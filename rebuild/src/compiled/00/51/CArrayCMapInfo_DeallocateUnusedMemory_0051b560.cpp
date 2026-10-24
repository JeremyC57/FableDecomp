// CArray<CMapInfo>::DeallocateUnusedMemory() -- shrink the map-info array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and swap the temporary in
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0051B560, length 180 bytes (+ int3 padding); called from the CWorldMap TU (0x0050997B).
// The manifest's CArray<std::pair<unsigned long,...>> label is propagated; the 0x38E38E39 / `sar 4` reciprocal is the 72-byte
// CMapInfo stride (landed CWorldMap::DeleteAllLevels 0x005022F0 models the same vector).
// Callees: vector<CMapInfo>::reserve 0x00519800 (on the temporary), vector<CMapInfo>::_M_range_insert(pos, first, last,
// forward_iterator_tag) 0x00519CD0, CArray<CMapInfo>::DeallocateMemory 0x0051A350, the range destroy 0x00515250 (__fastcall
// first, last, tag; the temporary's inline destructor), free 0x00BFEA14.
// Shape: `capacity() > size()` computes the size first (ecx) and compares unsigned (`jbe`); the temporary's inline constructor
// zeroes its three words (immediate stores) after the size push. The hand-over is written as the swap-plus-destroy it
// compiles to: the old start/finish are detached through `Exchange(slot, new)` (old start in edi, old finish in edx), the old
// range is destroyed and freed. A real `swap(temp)` + inline temporary destructor gives the same behaviour but VC7.1 keeps
// the dead stores back into the (escaped) temporary (189v180), so the model's temporary carries no destructor.
// Both empty tags share frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CMapInfo {                                       // 0x48 bytes
    unsigned char _body[0x48];
};
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };

void __fastcall DestroyRange(CMapInfo* first, CMapInfo* last, const __false_type& tag);   // 0x00515250

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x00519800
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x00519CD0
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x0051A350
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<CMapInfo>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<CMapInfo> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        CMapInfo* p = Exchange(_M_start, temp._M_start);
        CMapInfo* l = Exchange(_M_finish, temp._M_finish);
        _M_end_of_storage = temp._M_end_of_storage;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}