// _global::_Adjust_heap(std::pair<unsigned long,CSmallVector<NAnimationEvents::CEvent,32> >*, long, long, pair, CKeyPairCompareLess) -- Dinkumware heap sift-down of (unsigned long key, animation-event small vector) pairs by unsigned key, then _Push_heap @ 0x006FB9C0; destroys the by-value pair
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006fbd60, 209 bytes.
// Callees: CSmallVector<CEvent,32>::Copy 0x006FB5C0 (landed), _Push_heap 0x006FB9C0, operator
// delete[] 0x00BFEB1C (MSVCR71 import thunk).  first in ecx, hole in edx (link-time __fastcall);
// bottom, the 12-byte pair by value and the empty comparator follow (`ret 14h`); `top` lives in the
// `push ecx` slot.
// Element: Ego_r.pdb CSmallVector<NAnimationEvents::CEvent,32> (8 bytes) = CEvent* Start +0,
// int Size +4, so the pair is 12 bytes, as labelled.  Model as the landed
// __unguarded_linear_insert 0x006FB960 of the same pair.
// Shape: element assignment = key move + CSmallVector::operator= (`&other != this` self-test around
// the out-of-line Copy); the argument copy is the small vector's copy constructor (zero both
// members, then Copy, no self-test); the by-value pair's destructor (`delete[] Start`) runs here.
// The key test is an UNSIGNED `jae`.
void __cdecl operator delete[](void* block);   // MSVCR71 import thunk 0x00BFEB1C (the lane <new> lacks it)

namespace FableHeap_006fbd60 {

namespace NAnimationEvents { struct CEvent { long m_a, m_b; }; }

template<class T, int N> struct CSmallVector {
    T* Start;                                   // +0x00
    int Size;                                   // +0x04
    void Copy(const CSmallVector& other);       // 0x006FB5C0
    CSmallVector(const CSmallVector& other) : Start(0), Size(0) { Copy(other); }
    CSmallVector& operator=(const CSmallVector& other)
    {
        if (&other != this)
            Copy(other);
        return *this;
    }
    ~CSmallVector() { delete[] Start; }
};

template<class K, class V> struct pair {
    K first;
    V second;
};

typedef pair<unsigned long, CSmallVector<NAnimationEvents::CEvent, 32> > SKeyedEvents;

struct CKeyPairCompareLess {
    char m_tag;
    bool operator()(const SKeyedEvents& a, const SKeyedEvents& b) const { return a.first < b.first; }
};

void __fastcall _Push_heap(SKeyedEvents* first, long hole, long top, SKeyedEvents value, CKeyPairCompareLess pred);   // 0x006FB9C0

void __fastcall _Adjust_heap(SKeyedEvents* first, long hole, long bottom, SKeyedEvents value, CKeyPairCompareLess pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    _Push_heap(first, hole, top, value, pred);
}

}