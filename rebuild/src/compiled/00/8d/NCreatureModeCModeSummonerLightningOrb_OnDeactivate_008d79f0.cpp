// NCreatureMode::CModeSummonerLightningOrb::OnDeactivate() -- the CModeStandBase deactivation, then (live parent) resume a player's animation and remove the parent's 0xee component, then kill the live charge effects behind +0x84 and +0x7c
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D79F0, 151 bytes (`ret`).
// Callees: CModeStandBase::OnDeactivate @ 0x00861D00, CThingAnimatable::SetSuspendAnimation @ 0x00833A10,
// CThing::RemoveTC(long) @ 0x004C9840 (GOTCHAS chunk 140 label), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
// CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CModeSummonerLightningOrb: Filter +0x68, PTarget +0x74, PChargeEffect +0x7c, POrbChargeEffect +0x84,
// Timer +0x8c.
// Shape: the constant 1 lives in ebx (bit tests, the type compare and the Kill argument share it).
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
    unsigned char _pad11[0x5b];
    unsigned char m_animSuspended : 1;             // +0x6c bit 0
    unsigned char m_6c_1 : 7;
    unsigned char _pad6d[0x24];
    unsigned char m_dead : 1;                      // +0x91 bit 0
    unsigned char m_91_1 : 7;
    void SetSuspendAnimation(bool suspend, CModeBase* mode);   // 0x00833A10
    void Kill(bool flag);                          // 0x004C9B80
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CThing* GetPItem() const;                      // 0x00A01B50
};

struct CModeBase {                                 // CTCCreatureModeManager::CModeBase, vtable 0x012673CC
    virtual ~CModeBase();
    CThing*       m_pThing;                        // +0x04
    unsigned char _pad08[0x10];
    bool          Active;                          // +0x18
};

struct CModeStandBase : CModeBase {
    unsigned char _pad1c[0x4c];                    // +0x1c .. +0x68
    void OnDeactivate();                           // 0x00861D00
};

struct CThingTC : CThing { void RemoveTC(long type); };   // 0x004C9840 (CThing::RemoveTC)

struct CModeSummonerLightningOrb : CModeStandBase {
    unsigned char           _filter[0xc];          // +0x68 CAnimFilterOfGroup Filter
    CBaseIntelligentPointer PTarget;               // +0x74
    CBaseIntelligentPointer PChargeEffect;         // +0x7c
    CBaseIntelligentPointer POrbChargeEffect;      // +0x84
    long                    Timer;                 // +0x8c
    virtual void OnDeactivate();
};

void CModeSummonerLightningOrb::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    if (m_pThing && !m_pThing->m_dead) {
        if (m_pThing->m_type == 1)
            m_pThing->SetSuspendAnimation(false, this);
        ((CThingTC*)m_pThing)->RemoveTC(0xee);
    }
    if (POrbChargeEffect.GetPItem() && !POrbChargeEffect.GetPItem()->m_dead)
        POrbChargeEffect.GetPItem()->Kill(true);
    if (PChargeEffect.GetPItem() && !PChargeEffect.GetPItem()->m_dead)
        PChargeEffect.GetPItem()->Kill(true);
}
}