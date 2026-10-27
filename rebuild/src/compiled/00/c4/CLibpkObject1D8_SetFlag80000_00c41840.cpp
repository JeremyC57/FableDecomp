// CLibpkObject1D8::SetFlag80000(bool on) -- descriptive: switch state bit 0x80000 of the word at +0x10; on a change raise dirty bit 0x40000 in the flags word at +0x1D8
// Leaf: ?SetFlag80000@CLibpkObject1D8@@QAEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C41840, 53 bytes, thiscall `ret 4`.
// Identity: called from the libpk `__stdcall` wrapper at 0x00C33F9E (the accepts-the-object API shape of the landed
//   SetRangeOnAcceptingObject 0x00C33F20); sibling of SetFlag40000 0x00C417F0. Descriptive (libpk, no PDB/RTTI).
// Shape: each arm reads the state word itself and returns early when the bit is already as asked (VC hoists the two reads
//   into one `mov eax,[ecx+10h]` after `mov al,[esp+4]`; one shared read before the `if` gives `mov dl` instead); the
//   arms' stores merge into one.
class CLibpkObject1D8 {
public:
    unsigned char _pad00[0x10];
    unsigned long StateFlags;     // +0x10
    unsigned char _pad14[0x1D8 - 0x14];
    unsigned long DirtyFlags;     // +0x1D8
    void SetFlag80000(bool on);
};

void CLibpkObject1D8::SetFlag80000(bool on)
{
    if (on) {
        unsigned long flags = StateFlags;
        if (flags & 0x80000)
            return;
        StateFlags = flags | 0x80000;
    } else {
        unsigned long flags = StateFlags;
        if (!(flags & 0x80000))
            return;
        StateFlags = flags & ~0x80000;
    }
    DirtyFlags |= 0x40000;
}