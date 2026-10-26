// CTCPhysicalShieldSpell::Pause() -- when pausable and not yet paused: mark paused, stop the looping sound and kill the shield and swirly effect things that are still alive
// Leaf: Pause
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079E3A0, 126 bytes.
// Callees: CTCPhysicalShieldSpell::StopSound(unsigned long) @ 0x0079DDB0 (catalog "CCombatSequence_WillEnflame::OnStart" is
//   propagated; the body looks up the parent's sound interface 16 and stops the handle), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50 (three per effect), CThing::Kill(bool) @ 0x004C9B80.
// Layout (PDB CTCPhysicalShieldSpell): ShieldEffect +0x30, SwirlyEffect +0x38 (CIntelligentPointer<CThing>), LoopingSound +0x40,
//   Pausable +0x44, Paused +0x45; CThing +0x91 bit 0 = dead.
// Shape: the inline IsThingAlive(p) (GetPItem non-null and not dead) followed by p.GetPItem()->Kill(true), twice.
class CThing { public:
    unsigned char _pad00[0x91];
    unsigned char Flags91;                      // +0x91 bit 0 = dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void Kill(bool immediately);                // 0x004C9B80
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
class CTCPhysicalShieldSpell { public:
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x28];
    CIntelligentPointer<CThing> ShieldEffect;       // +0x30
    CIntelligentPointer<CThing> SwirlyEffect;       // +0x38
    unsigned long LoopingSound;                 // +0x40
    bool    Pausable;                           // +0x44
    bool    Paused;                             // +0x45
    bool    CutSceneCancelled;                  // +0x46
    void StopSound(unsigned long sound);        // 0x0079DDB0
    void Pause();
};

void CTCPhysicalShieldSpell::Pause()
{
    if (Pausable && !Paused) {
        Paused = true;
        StopSound(LoopingSound);
        if (IsThingAlive(ShieldEffect))
            ShieldEffect.GetPItem()->Kill(true);
        if (IsThingAlive(SwirlyEffect))
            SwirlyEffect.GetPItem()->Kill(true);
    }
}