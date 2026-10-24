// CArray<std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> > >::DeallocateMemory() -- release the array's storage: erase every element, detach and zero the three pointers, destroy the detached range and free the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006FC620, length 122 bytes (+ int3 padding); the only caller is CArray<...>::DeallocateUnusedMemory 0x006FD0B0; the element type is named by the manifest label of
// the copy 0x006FBC50 (`_Move<std::pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> >*,...>`); `imul 2AAAAAABh ;
// sar 1` is the 12-byte stride.
// Callees: the element copy-down 0x006FBC50 (__fastcall: first in ecx, last in edx, result + an empty tag + a null word on the
// stack), the range destroy 0x006FB8A0 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateMemory 0x0051A350 form (same TU-independent template): erase(begin(), end()) is
// `copy(end, end, begin)` + the destroy of the left-over tail + the finish store; the detach goes through `Exchange(slot, 0)`
// (a store through a `T*&` keeps the member reload after it); both empty tags are one-byte temporaries homed in the
// `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SEventListPair {                         // std::pair<unsigned long, CSmallVector<NAnimationEvents::CEvent,32> >, 0x0c bytes
    unsigned char _body[0x0c];
};
struct SCopyTag { SCopyTag() { } };
struct __false_type { __false_type() { } };

SEventListPair* __fastcall CopyDown(SEventListPair* first, SEventListPair* last, SEventListPair* result, const SCopyTag& tag, long* distance);   // 0x006FBC50
void __fastcall DestroyRange(SEventListPair* first, SEventListPair* last, const __false_type& tag);   // 0x006FB8A0

template<class T> struct CArray {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    unsigned long capacity() const { return (unsigned long)(_M_end_of_storage - _M_start); }
    void erase(T* f, T* l)
    {
        T* i = CopyDown(l, _M_finish, f, SCopyTag(), (long*)0);
        DestroyRange(i, _M_finish, __false_type());
        _M_finish = i;
    }
    void DeallocateMemory();
};

template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }

template<> void CArray<SEventListPair>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        SEventListPair* p = Exchange(_M_start, (SEventListPair*)0);
        SEventListPair* l = Exchange(_M_finish, (SEventListPair*)0);
        _M_end_of_storage = 0;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}