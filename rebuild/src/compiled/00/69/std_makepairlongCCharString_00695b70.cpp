// std::make_pair<long,CCharString>(const long& x, const CCharString& y) -- return a (key, name) pair by value: copy the long, copy-construct the string into the hidden result
// Leaf: make_pair
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00695B70, length 30 bytes (`ret`: __cdecl, the hidden result pointer is the
//   first stack argument and comes back in eax).
// Caller: 0x00694AA4 (`push name ; lea eax,[key] ; push eax ; lea ecx,[result] ; push ecx ; call ; add esp,0Ch`, the key
//   being a long converted from a float by _ftol just before), the result then handed to an insert at 0x006951A0.
// Label correction: the manifest's `_Pair_base<NUISystem::CComponent* const, std::pair<CWideString,long> >` is propagated;
//   the body copy-constructs a CCharString (0x0099EC30) and is not a constructor (no `this` in ecx).
// Callee: CCharString::CCharString(const CCharString&) @ 0x0099EC30.
// Shape: STLport `make_pair(const T1& x, const T2& y) { return pair<T1, T2>(x, y); }` with the pair constructor inlined
//   straight into the hidden result (no copy: the temporary is constructed in place).
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

template <class T1, class T2>
inline pair<T1, T2> make_pair(const T1& x, const T2& y)
{
    return pair<T1, T2>(x, y);
}
}

template std::pair<long, CCharString> std::make_pair<long, CCharString>(const long&, const CCharString&);