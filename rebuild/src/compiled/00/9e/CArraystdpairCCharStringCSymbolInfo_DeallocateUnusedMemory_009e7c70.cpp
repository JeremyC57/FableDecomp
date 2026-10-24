// CArray<std::pair<CCharString, CSymbolInfo> >::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009E7C70, length 152 bytes (+ int3 padding); reached from the landed CSymbolMap::RemoveUnusedMemory 0x009E5350 (`add ecx,4 ; jmp`, the
// CVectorMap<CCharString, CSymbolInfo> Data array) and two other CSymbolMap rows (0x009E5C52, 0x009E5DB5).
// Callees: vector::reserve 0x009E72C0 (on the temporary), vector::_M_range_insert(pos, first, last, forward_iterator_tag)
// 0x009E73F0, CArray::DeallocateMemory 0x009E7680, the range destroy 0x006FBCC0 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 form: `capacity() > size()` computes the size first (ecx) and
// compares unsigned (`jbe`); the temporary's inline constructor zeroes its three words (immediate stores) after the size push;
// the hand-over detaches the old start/finish through `Exchange(slot, new)` and destroys + frees the old range (a real
// `swap(temp)` + inline temporary destructor keeps dead stores into the escaped temporary). Both empty tags share frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct CCharString { void* PStringData; };
struct CSymbolInfo { unsigned long words[3]; };
struct SSymbolPair {                            // std::pair<CCharString, CSymbolInfo>, 0x10 bytes (`sar 4`)
    CCharString first;
    CSymbolInfo second;
};
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };

void __fastcall DestroyRange(SSymbolPair* first, SSymbolPair* last, const __false_type& tag);   // 0x006FBCC0

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x009E72C0
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x009E73F0
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x009E7680
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SSymbolPair>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<SSymbolPair> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        SSymbolPair* p = Exchange(_M_start, temp._M_start);
        SSymbolPair* l = Exchange(_M_finish, temp._M_finish);
        _M_end_of_storage = temp._M_end_of_storage;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}