// _global::_Adjust_heap(CMatchProbability*, long, long, CMatchProbability, bool (__fastcall*)(const CMatchProbability&, const CMatchProbability&)) -- heap sift-down of speech match probabilities under a function-pointer predicate, then _Push_heap @ 0x00AD0920
// Flags: /Gr
// Leaf: _Adjust_heap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD1DD0, 128 bytes.
// Callee: 0x00AD0920 _Push_heap (same instantiation).  Link-time __fastcall: first in ecx, hole
// in edx, then bottom, the 8-byte CSpeechAnalyser::CMatchProbability by value (pushed as two dwords:
// trivial copy) and the predicate; `ret 0x10`.
// CMatchProbability (PDB, 8 bytes): Index +0 (ulong), Probability +4 (float).  The predicate is a
// plain function pointer (`call dword ptr [esp+0x24]` with both element addresses in ecx/edx, so the
// comparison function itself is __fastcall); `top` lives in the frame slot reserved by `push ecx`.
// Shape: VC7.1 <algorithm> `_Adjust_heap`; the bottom case reuses idx (`first[idx - 1]`).
struct CMatchProbability {
    unsigned long Index;               // +0x0
    float Probability;                 // +0x4
};

typedef bool (__fastcall* TMatchCompare)(const CMatchProbability& a, const CMatchProbability& b);

void __fastcall _Push_heap(CMatchProbability* first, long hole, long top, CMatchProbability value, TMatchCompare pred);   // 0x00AD0920

void __fastcall _Adjust_heap(CMatchProbability* first, long hole, long bottom, CMatchProbability value, TMatchCompare pred)
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