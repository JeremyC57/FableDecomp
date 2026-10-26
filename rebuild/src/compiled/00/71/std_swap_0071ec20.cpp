// std::swap(pair<CSkeletalMorphs::CEntry,float>& a, pair<CSkeletalMorphs::CEntry,float>& b) -- exchange two 12-byte weighted skeletal-morph entries
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071EC20, length 55 bytes (+ int3 padding). `__fastcall` (a in ecx, b in edx),
//         plain `ret`.
// Callees: none.
// Identity: the manifest label "GFToScreen" is wrong. An unreferenced COMDAT (no call, jump or address anywhere in the
//   image) of std::swap for a 12-byte trivially copied T, in the CTCHeroMorph block next to the pair<CEntry,float> heap
//   helpers (_Push_heap 0x0071ECF0, landed). The bytes fix only the size and the trivial copy: T is named
//   pair<CSkeletalMorphs::CEntry, float> as the TU's 12-byte sort element (Ego_r.pdb instantiates
//   swap<CTCHeroMorph::CTextureLayer> here, a 20-byte type in Ego_r -- a different T).
// Shape: STLport `swap`: `_Tp __tmp = __a; __a = __b; __b = __tmp;` -- a's three words loaded first (the third spilled to
//   the 12-byte frame temporary), b copied into a, then the saved words stored into b.
class CDefString { public:
    long TablePos;                                      // +0x00
};
class CSkeletalMorphs {
public:
    class CEntry {                                      // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                           // +0x00
        short Priority;                                 // +0x04
        bool SupressesLowerMorphs;                      // +0x06
    };
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _Tp>
void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

template void __fastcall swap<pair<CSkeletalMorphs::CEntry, float> >(pair<CSkeletalMorphs::CEntry, float>&,
                                                                     pair<CSkeletalMorphs::CEntry, float>&);
}