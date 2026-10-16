// std::__median<CThing*, CThingCompare_BestForTargeting2>(CThing* a, CThing* b, CThing* c, CThingCompare_BestForTargeting2 comp) -- STLport median-of-three for the introsort of targeting candidates
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) @ 0x0080A140, 105 bytes (`ret 38h`: c plus the 52-byte comparator by value).
// Identity: the manifest name "CThingCompare_BestForTargeting2::SelectBest" is an auto label. The body is STLport's
//   __median(a, b, c, comp) (comp(a,b) ? (comp(b,c) ? b : comp(a,c) ? c : a) : (comp(a,c) ? a : comp(b,c) ? c : b))
//   over the comparator CThingCompare_BestForTargeting2::operator() @ 0x008098B0 (manifest), in the same TU as
//   make_heap<CThing**, CThingCompare_BestForTargeting2> @ 0x00809E40. Retail passes a and b in ecx/edx (the
//   whole-program register convention), reproduced with an explicit __fastcall.
// Layout: CThingCompare_BestForTargeting2 (Ego_r.pdb 52 bytes): Hero +0x00, VectorCompare (CVectorCompare_BestForTargeting,
//   44 bytes) +0x04, EnemyPreferenceFactor +0x30.
// Shape: the values are kept in edi (a), esi (b), ebx (c); every comparator call takes `lea ecx,[comp]`.
struct CThing;
struct CVectorCompare_BestForTargeting {        // 44 bytes
    unsigned long m_words[11];
};
struct CThingCompare_BestForTargeting2 {
    CThing* Hero;                               // +0x00
    CVectorCompare_BestForTargeting VectorCompare;  // +0x04
    float EnemyPreferenceFactor;                // +0x30
    bool operator()(CThing* a, CThing* b);      // 0x008098B0
};

namespace std {
template <class _Tp, class _Compare>
_Tp __fastcall __median(_Tp __a, _Tp __b, _Tp __c, _Compare __comp)
{
    if (__comp(__a, __b)) {
        if (__comp(__b, __c))
            return __b;
        else if (__comp(__a, __c))
            return __c;
        else
            return __a;
    }
    else if (__comp(__a, __c))
        return __a;
    else if (__comp(__b, __c))
        return __c;
    else
        return __b;
}
template CThing* __fastcall __median(CThing*, CThing*, CThing*, CThingCompare_BestForTargeting2);
}
