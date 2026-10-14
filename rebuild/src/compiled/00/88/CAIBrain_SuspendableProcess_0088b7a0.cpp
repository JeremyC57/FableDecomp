// CAIBrain::SuspendableProcess() -- runs the active state group's Process() unless it has terminated, flagging UpdatingStateGroup around the call
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088B7A0, length 34 bytes.
// Callees: CAIStateGroupBase::Process @ 0x008FCF30 (a non-inlined forwarder to the state
// group's slot-12 virtual; called with the group still in ecx).
// Layout: the retail CAIBrain is the PDB layout (engine/CAIBrain.h) shifted DOWN by one dword
// from +0x28 on -- the retail CVectorMap StateGroupList is 0x10 bytes, not 0x14 -- so
// PActiveStateGroup sits at +0x38 (PDB +0x3c) and UpdatingStateGroup at +0x8d (PDB +0x91);
// CAIStateGroupBase::Terminated is +0x10 (engine/CAIStateGroupBase.h). Modelled locally because
// the header's offsets disagree with the bytes. The group pointer is read once into a local
// so the call reuses the loaded register.
struct CAIStateGroupBase {
    void*         __vftable;                // +0x00
    void*         PBrain;                   // +0x04
    unsigned char ScriptAIPriority[4];      // +0x08
    void*         Name_PStringData;         // +0x0c
    bool          Terminated;               // +0x10
    unsigned char Priority;                 // +0x11
    bool          Enabled;                  // +0x12
    void Process();                         // 0x008FCF30 (forwards to slot 12)
};

struct CAIBrain {
    void*              __vftable;           // +0x00
    unsigned char      _pad0[0x34];         // +0x04..+0x38
    CAIStateGroupBase* PActiveStateGroup;   // +0x38 (retail; PDB +0x3c)
    unsigned char      _pad1[0x51];         // +0x3c..+0x8d
    bool               UpdatingStateGroup;  // +0x8d (retail; PDB +0x91)
    void SuspendableProcess();
};

void CAIBrain::SuspendableProcess()
{
    CAIStateGroupBase* group = PActiveStateGroup;
    if (!group->Terminated) {
        UpdatingStateGroup = true;
        group->Process();
        UpdatingStateGroup = false;
    }
}