// CArray<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > >::DeallocateUnusedMemory() -- shrink the array to fit: when it has spare capacity, copy the elements into an exactly reserved temporary, release the old storage and adopt the temporary's
// Leaf: ?DeallocateUnusedMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FD0B0, length 178 bytes (+ int3 padding); called from the NAnimationEvents manager load (0x006FA9BE, inside CManager::Load 0x006FA4E0); `imul 2AAAAAABh ; sar 1` is
// the 12-byte stride of std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > (the copy 0x006FBC50 label).
// Callees: vector::reserve 0x006FC2D0 (on the temporary), vector::_M_range_insert(pos, first, last, forward_iterator_tag)
// 0x006FC950, CArray::DeallocateMemory 0x006FC620, the range destroy 0x006FB8A0 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateUnusedMemory 0x0051B560 form: `capacity() > size()` computes the size first (ecx) and
// compares unsigned (`jbe`); the temporary's inline constructor zeroes its three words (immediate stores) after the size push;
// the hand-over detaches the old start/finish through `Exchange(slot, new)` and destroys + frees the old range (a real
// `swap(temp)` + inline temporary destructor keeps dead stores into the escaped temporary). Both empty tags share frame byte 3.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SEventListPair {                         // std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >, 0x0c bytes
    unsigned char _body[0x0c];
};
struct forward_iterator_tag { forward_iterator_tag() { } };
struct __false_type { __false_type() { } };

void __fastcall DestroyRange(SEventListPair* first, SEventListPair* last, const __false_type& tag);   // 0x006FB8A0

template<class T> class CArray { public:
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    CArray() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
    void reserve(unsigned long n);                                                         // 0x006FC2D0
    void _M_range_insert(T* pos, T* first, T* last, const forward_iterator_tag& tag);      // 0x006FC950
    void insert(T* pos, T* first, T* last) { _M_range_insert(pos, first, last, forward_iterator_tag()); }
    void DeallocateMemory();                                                               // 0x006FC620
    void DeallocateUnusedMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SEventListPair>::DeallocateUnusedMemory()
{
    if (capacity() > size()) {
        CArray<SEventListPair> temp;
        temp.reserve(size());
        temp.insert(temp.begin(), begin(), end());
        DeallocateMemory();
        SEventListPair* p = Exchange(_M_start, temp._M_start);
        SEventListPair* l = Exchange(_M_finish, temp._M_finish);
        _M_end_of_storage = temp._M_end_of_storage;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}