// _global::_Adjust_heap(std::pair<unsigned long,unsigned long>*, long, long, std::pair<unsigned long,unsigned long>, std::less<pair>) -- Dinkumware heap sift-down of (unsigned long, unsigned long) pairs under the lexicographic std::pair operator<, then _Push_heap @ 0x00A6B1E0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00a6b4f0, 122 bytes.
// Callee: 0x00A6B1E0 _Push_heap (same instantiation).  first in ecx, hole in edx (link-time
// __fastcall); bottom, the pair by value (built in its slot by the pair's user copy constructor:
// `sub esp,8 ; mov eax,esp`) and the empty std::less<> (4-byte slot) follow (`ret 10h`).
// Shape: the child test is Dinkumware's pair operator<
//   `x.first < y.first || (!(y.first < x.first) && x.second < y.second)`:
// UNSIGNED `cmp left,right ; ja dec ; jb skip` on the first, then an unsigned `jae` on the
// second.  Matches the label pair<unsigned long,unsigned long> exactly.
namespace FableHeap_00a6b4f0 {

template<class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& other) : first(other.first), second(other.second) { }
};

template<class T1, class T2> inline bool operator<(const pair<T1, T2>& x, const pair<T1, T2>& y)
{
    return x.first < y.first || (!(y.first < x.first) && x.second < y.second);
}

template<class T> struct less {
    char m_tag;
    bool operator()(const T& a, const T& b) const { return a < b; }
};

typedef pair<unsigned long, unsigned long> SPair;
typedef less<SPair> SLess;

void __fastcall _Push_heap(SPair* first, long hole, long top, SPair value, SLess pred);   // 0x00A6B1E0

void __fastcall _Adjust_heap(SPair* first, long hole, long bottom, SPair value, SLess pred)
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