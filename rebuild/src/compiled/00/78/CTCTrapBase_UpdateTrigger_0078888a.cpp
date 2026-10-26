#pragma optimize("s", on)
// CTCTrapBase::UpdateTrigger() -- while the trap is active, every other 8-frame window, a proximity-triggered trap re-checks its trigger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078888A, length 42 bytes. Size TU (`cmp byte ptr [esi+58h],0`
// flag test, as the landed CTCTrapBase::ManualReset 0x00787C0C): `optimize("s",on)`.
// Callees: CTCBase::GetWorld() @ 0x00686A70 (returns g->+0x24; manifest label "CViewBase::GetDisplayEngine" is
// propagated), the static world-frame getter @ 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated) called
// through the world object (so the GetWorld call stays, its result unused), CTCTrapBase::UpdateTriggerOnProximity
// @ 0x007887BE (tail jump).
// Shape: three nested early-outs folded into one `je/jne` chain to the shared `pop esi ; ret`; the trap def is
// reached through the CDefPointer object pointer at +0x54 and its trigger type (+0x34) compared with 1.
// Layout: CTCTrapBase.h is the PDB layout (PTrapDef_Object +0x60, Active +0x64); the retail constructor 0x0078918D
// shows them at +0x54 / +0x58 (12-byte STLport vectors), so a local model is used.
class CTrapDef { public:
    unsigned char _pad[0x34];
    long          TriggerType;              // +0x34 (1 = on proximity)
};

class CWorld { public:
    static long GetWorldFrame();            // 0x0049D870
};

class CTCTrapBase { public:
    void*         __vftable;                // +0x00
    unsigned char _pad_0x04[0x50];
    CTrapDef*     PTrapDef_Object;          // +0x54 CDefPointer<const CTrapDef>::Object
    bool          Active;                   // +0x58
    CWorld* GetWorld();                     // 0x00686A70 (CTCBase)
    void UpdateTriggerOnProximity();        // 0x007887BE
    void UpdateTrigger();
};

void CTCTrapBase::UpdateTrigger()
{
    if (Active) {
        if ((GetWorld()->GetWorldFrame() & 8) == 0) {
            if (PTrapDef_Object->TriggerType == 1)
                UpdateTriggerOnProximity();
        }
    }
}