// NCreatureMode::CModeJackOfBladesFloat::OnDeactivate() -- kill the two live emitters behind the intelligent pointers at +0x74 / +0x7c (CThing::Kill(true) unless already dead)
// Leaf: OnDeactivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D7730, 99 bytes (`ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three calls per pointer), CThing::Kill(bool) @ 0x004C9B80.
// Layout: Ego_r.pdb CModeJackOfBladesFloat: Filter +0x68, PEmitter +0x74, PEmitter2 +0x7c; the CThing dead bit is +0x91
// bit 0 (as the landed CModeScreamerSuperAttack::OnDeactivate 0x008D79A0).
// Shape: two copies of `if (p.GetPItem() && !p.GetPItem()->m_dead) p.GetPItem()->Kill(true);` (`this` kept in edi, the
// member address in esi).
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

struct CModeJackOfBladesFloat : CModeStandBase {
    unsigned char           _filter[0xc];          // +0x68 CAnimFilterOfGroup Filter
    CBaseIntelligentPointer PEmitter;              // +0x74
    CBaseIntelligentPointer PEmitter2;             // +0x7c
    virtual void OnDeactivate();
};

void CModeJackOfBladesFloat::OnDeactivate()
{
    if (PEmitter.GetPItem() && !PEmitter.GetPItem()->m_dead)
        PEmitter.GetPItem()->Kill(true);
    if (PEmitter2.GetPItem() && !PEmitter2.GetPItem()->m_dead)
        PEmitter2.GetPItem()->Kill(true);
}
}