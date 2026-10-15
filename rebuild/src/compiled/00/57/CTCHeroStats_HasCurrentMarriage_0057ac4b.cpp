// CTCHeroStats::HasCurrentMarriage() const -- true when any marriage record is in one of the four "current" marriage states (1..4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AC4B, 62 bytes.
// Callee: CTCHeroStats::HasMarriageInState(EMarriageState) @ 0x0057AC29 (landed; returns bool).
//
// Shape: a size-optimised TU: four short-circuit calls with `this` kept in esi, every success jumping to the shared
// `xor eax,eax ; inc eax` tail -- a 32-bit (BOOL/long) result, not `bool` (that would be `mov al,1`); the all-false
// path has its own `xor eax,eax ; pop esi ; ret`.

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x1D8];
    void*         Marriages_Head;              // +0x1D8 (PDB +0x1F0)

    bool HasMarriageInState(long state) const; // 0x0057AC29
    long HasCurrentMarriage() const;
};

#pragma optimize("s", on)

long CTCHeroStats_Retail::HasCurrentMarriage() const
{
    return HasMarriageInState(1) || HasMarriageInState(2) || HasMarriageInState(3) || HasMarriageInState(4);
}