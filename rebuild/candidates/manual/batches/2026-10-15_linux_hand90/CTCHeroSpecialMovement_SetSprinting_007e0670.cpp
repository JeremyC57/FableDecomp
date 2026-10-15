// CTCHeroSpecialMovement::SetSprinting(bool) -- starts sprinting only when the hero is not already sprinting and may sprint; stops it unconditionally
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007E0670, 50 bytes.
// Callee: IsAvailableForHeroSprint(CThing*) @ 0x0085F470 (`__fastcall`, the parent thing in ecx).
//
// Shape: the argument byte and the Sprinting byte are both loaded before the first branch; the start path tests
// Sprinting (`jne` out), asks the parent thing and stores 1 on success with its own `pop esi ; ret 4`; the stop path
// clears the byte only when it was set (each store is guarded by its own test -- nested `if`s, not assignments).
// Layout: Sprinting +0x0C, ParentThing +0x04 (PDB = retail for this class).

struct CThing;
bool __fastcall IsAvailableForHeroSprint(CThing* thing);   // 0x0085F470

struct CTCHeroSpecialMovement_Retail {
    void*         __vftable;                   // +0x00
    CThing*       ParentThing;                 // +0x04 CTCBase
    unsigned char _pad_0x08[0x4];              // +0x08 CTCBase flags
    bool          Sprinting;                   // +0x0C
    bool          Running;                     // +0x0D

    void SetSprinting(bool sprint);
};

void CTCHeroSpecialMovement_Retail::SetSprinting(bool sprint)
{
    if (sprint) {
        if (!Sprinting && IsAvailableForHeroSprint(ParentThing))
            Sprinting = true;
    } else {
        if (Sprinting)
            Sprinting = false;
    }
}
