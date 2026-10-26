// _global::_Adjust_heap(const CAnimationEntry**, long, long, const CAnimationEntry*, CTurningAnimCompare) -- heap sift-down of turning animations by how close their rotation is to the wanted turn, then _Push_heap @ 0x0087DCA0
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0087DE50, 161 bytes.
// Callees: 0x005DBFD0 CAnimationEntry::GetRotationInAnimation(float*) const (landed), 0x0087DCA0
// _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole in edx, then bottom,
// the entry pointer and the 4-byte comparator by value; `ret 0xc`.
// CTurningAnimCompare (anonymous namespace) holds the wanted rotation; its inline operator() fetches
// both rotations into frame floats and compares |wanted - rotation| with x87 fabs.  Retail calls
// GetRotationInAnimation twice on the SAME entry (ecx = ebp = first[idx] for both calls), so the
// shipped comparator reads its first argument twice -- reproduced as written; the heap order it
// produces is therefore arbitrary (the comparison is always false), see the test.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; `top` and the two rotations are the three frame slots.
#include <math.h>

class CAnimationEntry { public:
    long BankIndex;                             // +0x00
    void GetRotationInAnimation(float* pRotation) const;   // 0x005DBFD0
};

namespace {
struct CTurningAnimCompare {
    float m_WantedRotation;                     // +0x0
    bool operator()(const CAnimationEntry* a, const CAnimationEntry* b) const
    {
        float rotationA;
        float rotationB;
        a->GetRotationInAnimation(&rotationA);
        a->GetRotationInAnimation(&rotationB);
        return fabs(m_WantedRotation - rotationA) < fabs(m_WantedRotation - rotationB);
    }
};
}

void __fastcall _Push_heap(const CAnimationEntry** first, long hole, long top, const CAnimationEntry* value, CTurningAnimCompare pred);   // 0x0087DCA0

void __fastcall _Adjust_heap(const CAnimationEntry** first, long hole, long bottom, const CAnimationEntry* value, CTurningAnimCompare pred)
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