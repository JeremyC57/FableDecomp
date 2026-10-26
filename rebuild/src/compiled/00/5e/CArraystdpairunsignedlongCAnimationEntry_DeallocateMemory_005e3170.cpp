// CArray<std::pair<unsigned long, CAnimationEntry> >::DeallocateMemory() -- release the array's storage: erase every element, detach and zero the three pointers, destroy the detached range and free the block
// Leaf: ?DeallocateMemory@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005E3170, length 123 bytes (+ int3 padding); the only caller is CArray<...>::DeallocateUnusedMemory 0x005E3920 (tail-called by the landed CVectorMap<unsigned long,
// CAnimationEntry>::SortAndTrim 0x005DEDC0); `imul 66666667h ; sar 3` is the 20-byte stride.
// Callees: the element copy-down 0x005E18B0 (__fastcall: first in ecx, last in edx, result + an empty tag + a null word on the
// stack), the range destroy 0x00454404 (__fastcall first, last, tag), free 0x00BFEA14.
// Shape: the CArray<CMapInfo>::DeallocateMemory 0x0051A350 form (same TU-independent template): erase(begin(), end()) is
// `copy(end, end, begin)` + the destroy of the left-over tail + the finish store; the detach goes through `Exchange(slot, 0)`
// (a store through a `T*&` keeps the member reload after it); both empty tags are one-byte temporaries homed in the
// `push ecx` frame word.
extern "C" void __cdecl free(void* p);                                           // 0x00BFEA14

struct SAnimationPair {                         // std::pair<unsigned long, CAnimationEntry>, 0x14 bytes
    unsigned long first;
    long          second[4];                    // CAnimationEntry (four words, landed 0x005DEDC0)
};
struct SCopyTag { SCopyTag() { } };
struct __false_type { __false_type() { } };

SAnimationPair* __fastcall CopyDown(SAnimationPair* first, SAnimationPair* last, SAnimationPair* result, const SCopyTag& tag, long* distance);   // 0x005E18B0
void __fastcall DestroyRange(SAnimationPair* first, SAnimationPair* last, const __false_type& tag);   // 0x00454404

template<class T> class CArray { public:
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

template<> void CArray<SAnimationPair>::DeallocateMemory()
{
    if (capacity() > 0) {
        erase(_M_start, _M_finish);
        SAnimationPair* p = Exchange(_M_start, (SAnimationPair*)0);
        SAnimationPair* l = Exchange(_M_finish, (SAnimationPair*)0);
        _M_end_of_storage = 0;
        DestroyRange(p, l, __false_type());
        if (p)
            free(p);
    }
}