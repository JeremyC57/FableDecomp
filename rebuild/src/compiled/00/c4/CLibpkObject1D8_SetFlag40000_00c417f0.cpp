// CLibpkObject1D8::SetFlag40000(bool on) -- descriptive: switch state bit 0x40000 of the word at +0x10; on a change raise the dirty bits 0xBF8000 (word +0xC bit 0x2000 set) or 0x1FF8000 in the flags word at +0x1D8
// Leaf: ?SetFlag40000@CLibpkObject1D8@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C417F0, 67 bytes, thiscall `ret 4`.
// Identity: called from the libpk `__stdcall` wrapper at 0x00C33F6E (the accepts-the-object API shape of the landed
//   SetRangeOnAcceptingObject 0x00C33F20); the class follows the landed CLibpkObject1D8 setters (dirty word +0x1D8).
//   Descriptive (libpk, no PDB/RTTI): the bit's meaning is not proven.
// Shape: each arm reads the state word itself and returns early when the bit is already as asked (VC hoists the two reads
//   into one `mov eax,[ecx+10h]` after `mov al,[esp+4]`; one shared read before the `if` gives `mov dl` instead); the
//   arms' stores merge into one. The dirty mask is `mask = 0xBF8000; if (!(w & 0x2000)) mask = 0x1FF8000;` (a
//   ternary becomes neg/sbb/and/add).
class CLibpkObject1D8 {
public:
    unsigned char _pad00[0xc];
    unsigned long Word0C;         // +0x0C
    unsigned long StateFlags;     // +0x10
    unsigned char _pad14[0x1D8 - 0x14];
    unsigned long DirtyFlags;     // +0x1D8
    void SetFlag40000(bool on);
};

void CLibpkObject1D8::SetFlag40000(bool on)
{
    if (on) {
        unsigned long flags = StateFlags;
        if (flags & 0x40000)
            return;
        StateFlags = flags | 0x40000;
    } else {
        unsigned long flags = StateFlags;
        if (!(flags & 0x40000))
            return;
        StateFlags = flags & ~0x40000;
    }
    unsigned long mask = 0xBF8000;
    if (!(Word0C & 0x2000))
        mask = 0x1FF8000;
    DirtyFlags |= mask;
}