// std::__unguarded_insertion_sort_aux(SStringTriple* first, SStringTriple* last, SStringTriple*, SStringTripleLess comp) -- unguarded insertion sort of {CCharString, {CCharString, CCharString}} records
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004BA100, length 81 bytes.
// Leaf: __unguarded_insertion_sort_aux
// Label correction: the catalog "_Destroy_range<std::allocator<CSpeechFrame>>" is propagated. The body is SGI/STLport
// `__unguarded_insertion_sort_aux(first, last, _Tp*, comp)`: for every element it calls `__unguarded_linear_insert(i,
// _Tp(*i), comp)` @ 0x004B9480 (that callee walks back with three CCharString::operator= @ 0x0099EFB0 per step, compares
// the leading strings through 0x00429950 and destroys its by-value copy, `ret 10h` = 12-byte value + comparator).
// Element: pair<CCharString, pair<CCharString, CCharString> > -- the by-value copy is built in the argument slot by the
// STLport memberwise pair copy constructors (the inner pair's `this` = `add edi,4`, its source = `lea ebx,[esi+4]`).
// Convention: LTCG register form -- first in ecx, last in edx; the unused value-type pointer and the 4-byte empty
// comparator on the stack (`ret 8`); the callee takes `i` in ecx and the value and comparator on the stack.
// Callees: CCharString::CCharString(const CCharString&) @ 0x0099EC30, std::__unguarded_linear_insert @ 0x004B9480.
// Shape: the comparator slot is re-read and pushed first every iteration (arguments right to left); a flat struct of
// three strings addresses the third source from the second (`lea ebx,[esi+8] ; lea ecx,[ebx-4]`, 79 v 81 bytes).
class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
    ~CCharString();                         // 0x0099EAE0
};
namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const pair& o) : first(o.first), second(o.second) { }
};
}
typedef std::pair<CCharString, std::pair<CCharString, CCharString> > SStringTriple;
struct SStringTripleLess { };               // empty comparator: orders by `first`
namespace std {
void __fastcall __unguarded_linear_insert(SStringTriple* last, SStringTriple val, SStringTripleLess comp);   // 0x004B9480
void __fastcall __unguarded_insertion_sort_aux(SStringTriple* first, SStringTriple* last, SStringTriple*, SStringTripleLess comp)
{
    for (SStringTriple* i = first; i != last; ++i)
        __unguarded_linear_insert(i, SStringTriple(*i), comp);
}
}