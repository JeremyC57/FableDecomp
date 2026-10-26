// _global::__find_if(CThing** first, CThing** last, CIsCreatureIsIntersecting pred, const random_access_iterator_tag&) -- STLport random-access find_if: return the first thing the owner creature intersects, testing four per pass then the remainder
// Leaf: __find_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724A90, 198 bytes, `ret 8`.
// Label correction: the catalog "Position_FindSortedInsertPoint" is descriptive; the body is STLport's unrolled
//   `__find_if(first, last, pred, random_access_iterator_tag)` (trip count (last - first) >> 2, four tests per pass,
//   then `switch (last - first) { case 3: case 2: case 1: }` falling through, default/0 returning last) for
//   CIsCreatureIsIntersecting (Ego_r.pdb `_Find_if<CThing**, CIsCreatureIsIntersecting>`); caller 0x00724240.
// Callee: CIsCreatureIsIntersecting::operator()(CThing*) const @ 0x007249C0 (this chunk; the predicate is passed by
//   value and called in place on its argument slot: `lea ecx,[esp+14h]`).
// Convention: /Gr (link-time __fastcall): first in ecx (esi), last in edx (ebx), the 4-byte predicate and the tag
//   reference on the stack.  The switch lowers to the `dec eax ; je` chain.
class CThing;

class CIsCreatureIsIntersecting { public:              // 4 bytes (PDB)
    CThing* Owner;                              // +0x00
    bool operator()(CThing* thing) const;       // 0x007249C0
};

struct random_access_iterator_tag { random_access_iterator_tag() {} };

CThing** __fastcall __find_if(CThing** first, CThing** last, CIsCreatureIsIntersecting pred, const random_access_iterator_tag&)
{
    int trip_count = (last - first) >> 2;

    for (; trip_count > 0; --trip_count) {
        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;

        if (pred(*first)) return first;
        ++first;
    }

    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}