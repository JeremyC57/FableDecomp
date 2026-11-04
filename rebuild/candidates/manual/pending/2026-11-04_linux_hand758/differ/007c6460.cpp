// CTCGhostSword::FrameUpdate() -- fade the ghost swords in (alpha from each minion's fade-in timer), update the particle effects, and remove the spell component (0x9A) once no minion is alive
// Grade: DIFFER(240v240) -- rewritten on the landed CancelForCutscene 0x007C5480 model (CIntelligentPointer<CThing>, IsThingAlive, begin()+n index): only the ebx/ebp swap of the fade index and its 8-scaled minion offset is left; 10 grades (shared index variable, unsigned second loop, prefix/explicit decrement, raw index, template IsThingAlive)
// Leaf: ?FrameUpdate@CTCGhostSword@@UAEXXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6460, length 240 bytes. __thiscall, `ret`. Ego_r.pdb `?FrameUpdate@CTCGhostSword@@UAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (inlined IS_THING_ALIVE pairs), CTCGraphicAppearance::SetAlpha
//          (unsigned char) @ 0x004BFAC0, _ftol2 @ 0x00BFEA70, CTCGhostSword::UpdateGhostSwordParticleEffects @ 0x007C60A0,
//          CThing::RemoveTC(long) @ 0x004C9840. Float pool 0x0122DED8 = 1.0f, 0x01230014 = 255.0f.
// Shape: the fade loop is a signed index over `(long)CurFadeInTimer.size()` (`test eax,0FFFFFFFCh ; jle`), the timer
//        decremented through a reference and re-read for the alpha `(1 - timer / FadeInTimer) * 255`; the minion is
//        `*(begin() + i)` (scaled index in ebp); the alive scan is an unsigned index loop that returns on the first live
//        minion, the owner read once before it (ebp).
// Layout: retail CTCGhostSword (12-byte STLport vectors): Minions +0x10, MinionEffects +0x1C, MinionAliveTimer +0x28,
//         FadeInTimer +0x2C, CurFadeInTimer +0x30; retail CThing PTCGraphicAppearance +0x64.
class CTCGraphicAppearance { public:
    void SetAlpha(unsigned char alpha);                     // 0x004BFAC0
};
class CThing { public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* PTCGraphicAppearance;             // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
    void RemoveTC(long interfaceType);                      // 0x004C9840
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
};
inline bool IsThingAlive(const CIntelligentPointer<CThing>& p) { return p.GetPItem() && !p.GetPItem()->IsDead(); }
template <class T> struct CRetailVector {                   // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    T& operator[](unsigned int n) const { return *(begin() + n); }
};
class CTCGhostSword { public:
    virtual void v00();
    CThing*       ParentThing;                              // +0x04 (CTCBase)
    bool          ToBeRemoved;                              // +0x08
    bool          IsInGlobalUpdateTCs;                      // +0x09
    void*         PDef;                                     // +0x0c CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CIntelligentPointer<CThing> > Minions;         // +0x10
    CRetailVector<CIntelligentPointer<CThing> > MinionEffects;   // +0x1c
    long          MinionAliveTimer;                         // +0x28
    long          FadeInTimer;                              // +0x2c
    CRetailVector<long> CurFadeInTimer;                     // +0x30
    void UpdateGhostSwordParticleEffects();                 // 0x007C60A0
    virtual void FrameUpdate();
};

void CTCGhostSword::FrameUpdate()
{
    for (long i = 0; i < (long)CurFadeInTimer.size(); ++i) {
        if (CurFadeInTimer[i] > 0) {
            CurFadeInTimer[i]--;
            if (IsThingAlive(Minions[i])) {
                CTCGraphicAppearance* appearance = Minions[i].GetPItem()->PTCGraphicAppearance;
                if (appearance)
                    appearance->SetAlpha((unsigned char)((1.0f - (float)CurFadeInTimer[i] / FadeInTimer) * 255.0f));
            }
        }
    }
    UpdateGhostSwordParticleEffects();
    CThing* parent = ParentThing;
    for (unsigned int j = 0; j < Minions.size(); ++j) {
        if (IsThingAlive(Minions[j]))
            return;
    }
    parent->RemoveTC(0x9a);
}
