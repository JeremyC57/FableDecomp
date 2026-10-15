// NCreatureMode::CModeScreamerSuperAttack::OnDeactivate() -- the CModeStandBase deactivation (0x00861D00), the animation resumed on a player creature (type byte +0x10 == 1: SetSuspendAnimation(false, this) @ 0x00833A10), then the live target behind the intelligent pointer at +0x7c killed (CThing::Kill(true) @ 0x004C9B80)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D79A0, length 73 bytes.
// Callees: CModeStandBase::OnDeactivate @ 0x00861D00, CThingAnimatable::SetSuspendAnimation @ 0x00833A10, CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls), CThing::Kill @ 0x004C9B80. Vtable slot 6 of CModeScreamerSuperAttack. Shape: `this` in esi, advanced to the pointer member (+0x7c) once `this` is dead, the dead bit +0x91 tested, `ret`.
namespace NCreatureMode {
struct CModeBase;
struct CThing {
    unsigned char _pad00[0x10];
    unsigned char m_type;                          // +0x10 (1 = the player creature)
    unsigned char _pad11[0x80];
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
    CThing* m_pThing;                              // +0x04
};

struct CModeStandBase : CModeBase {
    unsigned char _pad08[0x60];                    // +0x08 .. +0x68
    void OnDeactivate();                           // 0x00861D00
};

struct CModeScreamerSuperAttack : CModeStandBase {
    unsigned char           _pad68[0x14];          // +0x68 .. +0x7c
    CBaseIntelligentPointer PTarget;               // +0x7c
    virtual void OnDeactivate();
};

void CModeScreamerSuperAttack::OnDeactivate()
{
    CModeStandBase::OnDeactivate();
    if (m_pThing->m_type == 1)
        m_pThing->SetSuspendAnimation(false, this);
    if (PTarget.GetPItem() && !PTarget.GetPItem()->m_dead)
        PTarget.GetPItem()->Kill(true);
}
}