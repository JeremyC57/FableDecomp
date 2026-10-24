// _global::_Adjust_heap(SPair*, long, long, SPair, SLess) -- Dinkumware heap sift-down for 8-byte (float key, EConversationAttitude) pairs, then _Push_heap @ 0x004CFD80
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004D04F0, 111 bytes.
// Callee: 0x004CFD80 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in
// edx, then bottom, the pair by value (8 bytes, built in its slot by the user copy constructor)
// and the empty CKeyPairCompareLess<float,EConversationAttitude> (4-byte slot); `ret 0x10`.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the key compare is `fld [idx] ; fcomp [idx-1] ;
// test ah,5 ; jp` = `first[idx].first < first[idx-1].first` on floats.  The bottom case reuses
// idx (`first[hole] = first[idx - 1]`), as in the sibling int-key rows 0x00680750/0x004CBED0.
enum EConversationAttitude { CONVERSATION_ATTITUDE_NONE };

struct SPair {
    float m_first;
    EConversationAttitude m_second;
    SPair(const SPair& other) : m_first(other.m_first), m_second(other.m_second) { }
};

struct SLess {
    char m_tag;
    bool operator()(const SPair& a, const SPair& b) const { return a.m_first < b.m_first; }
};

void __fastcall _Push_heap(SPair* first, long hole, long top, SPair value, SLess pred);   // 0x004CFD80

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