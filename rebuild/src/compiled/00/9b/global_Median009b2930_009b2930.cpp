// Median_009b2930(const CWideString& a, const CWideString& b, const CWideString& c, SWideStringLess comp) -- the SGI `__median` of three wide strings (the comparator functor passed by value) (CWideString::operator< @ 0x0099AF70): b when a < b < c, else c when a < c, else a; and symmetrically when !(a < b)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009B2930, length 94 bytes.
// Leaf: Median_009b2930
// The chunk label is propagated; named by address. Shape: a `__fastcall` function (a in ecx -> edi, b in edx -> esi, c on the stack -> ebx, the empty comparator in the second stack slot), the five compare calls in the SGI order, `ret 8`.
class CWideString { public:
    void* PStringData;
    bool operator<(const CWideString& other) const;   // 0x0099AF70
};

struct SWideStringLess_009b2930 {                  // the empty comparator, passed by value (the fourth, unread argument slot)
    SWideStringLess_009b2930() { }
    bool operator()(const CWideString& a, const CWideString& b) const { return a < b; }
};

const CWideString* __fastcall Median_009b2930(const CWideString& a, const CWideString& b, const CWideString& c, SWideStringLess_009b2930 comp)
{
    if (comp(a, b)) {
        if (comp(b, c))
            return &b;
        else if (comp(a, c))
            return &c;
        else
            return &a;
    } else if (comp(a, c)) {
        return &a;
    } else if (comp(b, c)) {
        return &c;
    } else {
        return &b;
    }
}