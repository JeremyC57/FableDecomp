// CCompare_FurthestFromInitPos2D::Median(CThing* const& a, CThing* const& b, CThing* const& c, CCompare_FurthestFromInitPos2D comp) -- SGI `__median` of three thing pointers under the "furthest from initial position (2D)" ordering
// Leaf: __median
// Retail: Fable.exe (TLC, VC7.1) @ 0x00748760, 132 bytes.
// Callees: CCompare_FurthestFromInitPos2D::operator()(CThing*, CThing*) 0x007483E0 (out of line, __fastcall).
//
// Shape: the SGI algorithm TUs are /Gr (GOTCHAS: every free template function is __fastcall), as the
// landed Median_009b2930: a in ecx -> edi, b in edx -> esi, c on the stack -> ebx, the comparator
// passed BY VALUE in the second stack slot; the elements are CThing pointers, so each compare pushes
// the two dereferenced pointers and calls the functor's out-of-line operator() with ecx = &comp (the
// argument slot). Returns a pointer to the median element (`ret 8`). The comparator carries state
// (it is not empty: `lea ecx,[esp+1Ch]` is its own slot), modelled as one word.
class CThing;

struct CCompare_FurthestFromInitPos2D {
    long m_reference;                       // comparator state (4-byte functor)
    bool operator()(CThing* a, CThing* b) const;   // 0x007483E0
};

CThing* const* __fastcall __median(CThing* const& a, CThing* const& b, CThing* const& c, CCompare_FurthestFromInitPos2D comp)
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