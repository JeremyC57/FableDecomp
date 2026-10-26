// CThingCreatureBase::UpdateLastAttackingCreature() -- forget the last attacker once 300 seconds' worth of frames have passed since it attacked
// Retail: Fable.exe (TLC, VC7.1) @ 0x006624A0, 66 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (on PLastAttackedByCreature), CThing::GetWorld @ 0x004C7AB0
//          (propagated "CViewBase::GetDisplayEngine"), static CWorld::GetWorldFrame @ 0x0049D870 (returns the global
//          @ 0x013B89BC), CThingCreatureBase::ClearLastAttackedByCreature @ 0x006691C0 (tail jump; it points the
//          intelligent pointer at 0 through a temporary; the "CAIStateGroup_MinionPutOutFires::Cleanup" label is
//          propagated). Global: g_GameFramesPerSecond (long @ 0x01375550).
// Layout: PLastAttackedByCreature (CIntelligentPointer<CThingCreatureBase>, 8 bytes) at retail +0xf4 and
//   WFLastAttackedByCreature (unsigned long) at +0xfc (PDB +0x110/+0x118).
// Shape: the stored frame is loaded into edi AFTER the GetWorld call but BEFORE GetWorldFrame: it is the argument
//   of an inline CWorld helper on a named world local (`world = GetWorld(); world->GetFramesSince(WF)`; written as one
//   chained expression the lane loads the argument before the GetWorld call), then the inline
//   body's static call); the difference is unsigned (`jbe`) because WFLastAttackedByCreature is unsigned long.
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseObject* GetPItem();                        // 0x00A01B50
};

class CWorld { public:
    static long GetWorldFrame();                    // 0x0049D870
    unsigned long GetFramesSince(unsigned long frame) { return GetWorldFrame() - frame; }
};

extern long g_GameFramesPerSecond;                  // 0x01375550

class CThingCreatureBase { public:
    unsigned char _pad000[0xf4];
    CBaseIntelligentPointer PLastAttackedByCreature;    // +0xf4
    unsigned long WFLastAttackedByCreature;             // +0xfc
    CWorld* GetWorld();                             // 0x004C7AB0 (CThing)
    void ClearLastAttackedByCreature();             // 0x006691C0
    void UpdateLastAttackingCreature();
};

void CThingCreatureBase::UpdateLastAttackingCreature()
{
    if (PLastAttackedByCreature.GetPItem()) {
        CWorld* world = GetWorld();
        if (world->GetFramesSince(WFLastAttackedByCreature) > (unsigned long)(g_GameFramesPerSecond * 300))
            ClearLastAttackedByCreature();
    }
}