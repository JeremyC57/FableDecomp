// std::pair<long,CCharString>::pair(const long& a, const CCharString& b) -- build a (count, name) pair: copy the long, copy-construct the string
// Leaf: ??0?$pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695250, length 30 bytes (`ret 8`).
// Caller: 0x0074EBC7 in CTCContainerRewardHero (the potion reward path: `value_type(count, CCharString("..."))` built in a
//   frame temporary from the GetDynamicPotionDefIndex result and a literal-built string, then pushed into a vector).
// Label correction: the manifest's `_Pair_base<NUISystem::CComponent* const, std::pair<CWideString,long> >` is propagated;
//   the second member's constructor is CCharString's copy constructor 0x0099EC30, so this is pair<long, CCharString>.
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport's `pair(const T1& a, const T2& b) : first(a), second(b)`: both references are loaded first, the long is
//   copied through ecx, then the string reference is pushed for the out-of-line copy constructor on `lea ecx,[esi+4]`.
class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);              // 0x0099EC30
};

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};
}

template struct std::pair<long, CCharString>;