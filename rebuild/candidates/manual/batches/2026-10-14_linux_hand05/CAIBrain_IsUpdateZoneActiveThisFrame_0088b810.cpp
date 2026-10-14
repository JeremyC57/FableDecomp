// CAIBrain::IsUpdateZoneActiveThisFrame() -- true on the frames the current update zone schedules this brain, or whenever the active state group is timing critical
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B810, length 85 bytes.
// Callees: CMainGameComponent::GetFrameNumber @ 0x004167C8 (an incremental-link thunk to the
// global frame-counter read @ 0x0049D870; the manifest's "GFGetPreMainMemoryUsed" label is
// propagated), the CRT 64-bit unsigned remainder __aullrem @ 0x00BFEBB0, and the active state
// group's slot-7 virtual IsTimingCritical (+0x1c, rtti-vtables.tsv).
// Layout: the retail CAIBrain is the PDB layout (engine/CAIBrain.h) shifted down by one dword
// from +0x28 on (the retail CVectorMap/vector are 0x10/0x0c bytes): Component +0x14 and
// Creature +0x18 agree with the header, PActiveStateGroup is +0x38 (PDB +0x3c) and
// PCurrentUpdateZone +0xa0 (PDB +0xa8) -- modelled locally because the header disagrees.
// The zone's period (+4, unsigned long, zero-extended with `push 0`) divides the creature's
// 64-bit update offset (+0x18 of the creature) plus the frame number (zero-extended: `xor ecx`
// / `adc`); a zero remainder is "active". The group test keeps its explicit null check.
struct CBrainUpdateZone {
    unsigned char _pad[4];
    unsigned long m_updatePeriod;           // +0x04 frames between updates
};

struct CMainGameComponent {
    unsigned long GetFrameNumber();         // 0x004167C8 -> 0x0049D870
};

struct CThingAICreatureCalls {
    unsigned char    _pad[0x18];
    unsigned __int64 m_updateOffset;        // +0x18 per-creature frame offset (64-bit)
};

struct CAIStateGroupBaseCalls {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06();
    virtual bool IsTimingCritical();        // slot 7 (+0x1c)
};

struct CAIBrain {
    void*                   __vftable;      // +0x00
    unsigned char           _pad0[0x10];    // +0x04..+0x14
    CMainGameComponent*     Component;      // +0x14
    CThingAICreatureCalls*  Creature;       // +0x18
    unsigned char           _pad1[0x1c];    // +0x1c..+0x38
    CAIStateGroupBaseCalls* PActiveStateGroup;   // +0x38 (retail; PDB +0x3c)
    unsigned char           _pad2[0x64];    // +0x3c..+0xa0
    CBrainUpdateZone*       PCurrentUpdateZone;  // +0xa0 (retail; PDB +0xa8)
    bool IsUpdateZoneActiveThisFrame();
};

bool CAIBrain::IsUpdateZoneActiveThisFrame()
{
    if (PCurrentUpdateZone == 0)
        return false;
    unsigned long    period = PCurrentUpdateZone->m_updatePeriod;   // read into locals BEFORE the call
    unsigned __int64 offset = Creature->m_updateOffset;              // (member reads only stay ahead of a call as locals)
    if ((offset + Component->GetFrameNumber()) % period == 0)
        return true;
    if (PActiveStateGroup != 0 && PActiveStateGroup->IsTimingCritical())
        return true;
    return false;
}
