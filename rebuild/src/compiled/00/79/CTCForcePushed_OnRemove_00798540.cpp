// CTCForcePushed::OnRemove() -- CTCBase virtual (vtable 0x0126C09C slot 2): the inlined FinalCleanup -- when the owning thing is alive and is the player creature (type byte 1), take this component off its animation-suspender set
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00798540, length 34 bytes (slot 2 of CTCForcePushed's vtable 0x0126C09C).
// Callees: the creature method @ 0x00833A10 (landed "CThingAnimatable::SetSuspendAnimation(bool, void*)"; a two-argument
// thiscall, bool first, working on the creature's +0x1cc suspender set).
// Naming: Ego_r.pdb has ?OnRemove@CTCForcePushed@@UAEXXZ and ?OnKill@CTCForcePushed@@UAEXXZ, both next to the private,
//   non-virtual ?FinalCleanup@CTCForcePushed@@AAEXXZ; a vtable slot cannot hold the non-virtual FinalCleanup, and
//   retail carries this same body twice, at slot 14 (the landed CTCForcePushed::OnKill 0x00798510) and here at slot 2,
//   the CTCBase OnRemove slot (see the CTCAssassinRush OnRemove 0x007AA830 / OnKill 0x007AA820 pair, which tail-call
//   their out-of-line FinalCleanup). Was landed as "CTCForcePushed::FinalCleanup".
// Layout: CTCBase ParentThing +0x04; retail CThing (the PDB layout less 0x10 past the leading bases): the type byte +0x10
// (1 = the player creature, as the landed CGameScriptThing accessors), the Dead bit = bit 0 of +0x91. A local model:
// engine/CTCBase.h is a flattened struct that cannot carry the virtual.
// Shape: the owner read once into ecx; VC materialises the constant 1 in al and reuses it for the bit test
// (`test al,dl`) and the `== 1` compare; the call is the last statement, so nothing follows it but `ret`.
// Leaf: ?OnRemove@CTCForcePushed@@UAEXXZ
struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char m_type;                               // +0x10 (1 = the player creature)
    unsigned char _pad_0x11[0x80];
    unsigned char Dead : 1;                             // +0x91 bit 0
    unsigned char _flags_0x91 : 7;
    void SetSuspendAnimation(bool suspend, void* suspender);   // 0x00833A10
};

class CTCForcePushed {
public:
    virtual ~CTCForcePushed();
    virtual void OnRemove();                            // CTCBase slot 2
    virtual void OnKill();                              // CTCBase slot 14 (0x00798510)
private:
    void FinalCleanup()                                 // inlined into both slots in retail
    {
        CThing* thing = ParentThing;
        if (thing && !thing->Dead && thing->m_type == 1)
            thing->SetSuspendAnimation(false, 0);
    }
    CThing*       ParentThing;                          // +0x04 (CTCBase)
};

void CTCForcePushed::OnRemove()
{
    FinalCleanup();
}
