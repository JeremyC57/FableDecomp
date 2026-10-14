// CAIStateGroup_MinionBehaviourCrowdBase::Cleanup() -- clear the crowd flag and hand over to the crowd cleanup
// hook (virtual slot 16, +0x40) with a sibling jump through the vtable.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957BE0, length 9 bytes (the chunk row's 80 bytes run on past
// the int3 padding into the NEXT function at 0x00957BF0, a subtitle helper; the oracle row is trimmed to the
// real body). The vtable load precedes the byte store (scheduling), then `jmp [eax+40h]`.
// No engine header for this class; the layout is local (flag at +0x38 descriptive, slot names descriptive
// because rtti-vtables lists only slots 0/1/9 of vtable 0x0128C494).
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void Initialise();              // slot 1
    virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void v07(); virtual void v08();
    virtual void Cleanup();                 // slot 9
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15();
    virtual void CleanupCrowd();            // slot 16 (+0x40)
    virtual void InitialiseCrowd();         // slot 17 (+0x44)
    unsigned char _pad[0x10];               // +0x04..+0x13
};

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {
    unsigned char _pad14[0x24];             // +0x14 behaviour base + crowd state
    bool m_inCrowd;                         // +0x38
    virtual void Cleanup();
};

void CAIStateGroup_MinionBehaviourCrowdBase::Cleanup()
{
    m_inCrowd = false;
    CleanupCrowd();
}