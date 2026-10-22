// CTCForcePushed::vfunc_2() -- when the owning thing is alive and is the player creature (type byte 1), call its animation-suspend hook with (false, 0)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798540, length 34 bytes (slot 2 of CTCForcePushed's vtable 0x0126C09C).
// Callees: the creature method @ 0x00833A10 (manifest "CThingAnimatable::SetSuspendAnimation"; a two-argument thiscall,
// bool first, working on the creature's +0x1cc member).
// Layout: CTCBase ParentThing +0x04; retail CThing (the PDB layout less 0x10 past the leading bases): the type byte +0x10
// (1 = the player creature, as the landed CGameScriptThing accessors), the Dead bit = bit 0 of +0x91.
// Shape: the owner read once into ecx; VC materialises the constant 1 in al and reuses it for the bit test
// (`test al,dl`) and the `== 1` compare; the call is the last statement, so nothing follows it but `ret`.
struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char m_type;                               // +0x10 (1 = the player creature)
    unsigned char _pad_0x11[0x80];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _flags_0x91 : 7;
    void SetSuspendAnimation(bool suspend, long reason);   // 0x00833A10
};

struct CTCForcePushed {
    unsigned char _pad_0x00[0x04];                      // vptr
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    void FinalCleanup();
};

void CTCForcePushed::FinalCleanup()
{
    CThing* thing = ParentThing;
    if (thing && !thing->Dead && thing->m_type == 1)
        thing->SetSuspendAnimation(false, 0);
}