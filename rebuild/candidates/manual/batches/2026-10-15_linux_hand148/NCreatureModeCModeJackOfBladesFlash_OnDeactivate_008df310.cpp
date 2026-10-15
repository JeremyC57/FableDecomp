// NCreatureMode::CModeJackOfBladesFlash::OnDeactivate() -- kill the live emitter (+0x88) and spotlight (+0x90), put out the flames, resume a suspended creature animation, clear Active
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008DF310, 134 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::Kill(bool) @ 0x004C9B80,
// CModeJackOfBladesFlash::ExtinguishFlames @ 0x008DCB60, CThingAnimatable::SetSuspendAnimation @ 0x00833A10.
// Layout: Ego_r.pdb CModeJackOfBladesFlash (offsets equal retail): Timer +0x68, BurnCounter +0x6c, Filter +0x70,
// PCreature +0x7c, PTarget +0x80, PEmitter +0x88, PSpotLight +0x90; CModeBase::Active +0x18.
// Shape: the two kill blocks, then the out-of-line ExtinguishFlames, then SetSuspendAnimation(false, 0) on the creature
// when its +0x6c bit 0 is set; Active cleared last.
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

struct CModeJackOfBladesFlash : CModeStandBase {
    long                    Timer;                 // +0x68
    long                    BurnCounter;           // +0x6c
    unsigned char           _filter[0xc];          // +0x70 CAnimFilterOfGroup Filter
    CThing*                 PCreature;             // +0x7c
    CBaseIntelligentPointer PTarget;               // +0x80
    CBaseIntelligentPointer PEmitter;              // +0x88
    CBaseIntelligentPointer PSpotLight;            // +0x90
    void ExtinguishFlames();                       // 0x008DCB60
    virtual void OnDeactivate();
};

void CModeJackOfBladesFlash::OnDeactivate()
{
    if (PEmitter.GetPItem() && !PEmitter.GetPItem()->m_dead)
        PEmitter.GetPItem()->Kill(true);
    if (PSpotLight.GetPItem() && !PSpotLight.GetPItem()->m_dead)
        PSpotLight.GetPItem()->Kill(true);
    ExtinguishFlames();
    if (PCreature->m_animSuspended)
        PCreature->SetSuspendAnimation(false, 0);
    Active = false;
}
}
