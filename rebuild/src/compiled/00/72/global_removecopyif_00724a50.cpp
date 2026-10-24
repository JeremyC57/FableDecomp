// _global::remove_copy_if(CThing** first, CThing** last, CThing** result, CIsCreatureIsIntersecting pred) -- STLport remove_copy_if: copy every thing the owner creature does NOT intersect down to `result`, return the new end
// Leaf: remove_copy_if
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00724A50, 61 bytes (64 with padding), `ret 8`.
// Label correction: the catalog "Position_CopyNonIntersecting" is descriptive; the body is STLport
//   `remove_copy_if(first, last, result, pred) { for (; first != last; ++first) if (!pred(*first)) { *result = *first;
//   ++result; } return result; }`, called by remove_if (inlined in 0x00724240, call site 0x00724267) as
//   `remove_copy_if(++next, last, first, pred)` (Ego_r.pdb `_Remove_if<CThing**, CIsCreatureIsIntersecting>`).
// Callee: CIsCreatureIsIntersecting::operator()(CThing*) const @ 0x007249C0 (called in place on the by-value
//   predicate's argument slot).
// Convention: /Gr (link-time __fastcall): first in ecx (esi), last in edx (ebx), result and the 4-byte predicate on
//   the stack; the empty-range exit returns result straight from its slot.
class CThing;

struct CIsCreatureIsIntersecting {              // 4 bytes (PDB)
    CThing* Owner;                              // +0x00
    bool operator()(CThing* thing) const;       // 0x007249C0
};

CThing** __fastcall remove_copy_if(CThing** first, CThing** last, CThing** result, CIsCreatureIsIntersecting pred)
{
    for (; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
    return result;
}