// CTCGhostSword::FrameUpdate() -- fade the ghost swords in (alpha from each minion's fade-in timer), update the particle effects, and remove the spell component (0x9A) once no minion is alive
// Grade: DIFFER(240v240) -- pure register rename: i/8i in ebp/ebx v retail ebx/ebp (fade loop), j/minion in edi/ebx v retail ebx/edi (alive scan); chunk 609 tried: operator-> for the appearance read, pre-decrement, IS_THING_ALIVE as a macro (232, worse), one shared `long i` for both loops (same). Earlier: pointer timer, named count, operator[] timer (31->18), separate i declaration, parent local dropped (worse), minion reference dropped.
// History: Grade: DIFFER(240v240): length matches, 18 diff lines (was 31) -- the timer is read and decremented through the vector's operator[] (retail's lea eax,[ecx+ebx*4] and the copy-then-dec); residue is a pure register rename (i/8i in ebp/ebx v ebx/ebp, j/minion in edi/ebx v ebx/edi)
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
struct CTCBase;

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
};

struct CTCGraphicAppearance {
    void SetAlpha(unsigned char alpha);             // 0x004BFAC0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                 // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                     // +0x44
    unsigned char _pad4c[0x64 - 0x4c];
    CTCGraphicAppearance* PTCGraphicAppearance;     // +0x64
    unsigned char _pad68[0x91 - 0x68];
    unsigned char m_flags91;                        // +0x91 bit 0: dead
    void RemoveTC(long interfaceType);              // 0x004C9840
};

struct CBaseIntelligentPointer {                    // CIntelligentPointer<CThingAICreature>: 8 bytes
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CThing* GetPItem() const;                       // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};

struct CTCGhostSword {
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    unsigned char _pad08[4];
    void* PDef;                                     // +0x0C CDefPointer<const CSpecialAbilitiesGhostSwordDef>
    CRetailVector<CBaseIntelligentPointer> Minions; // +0x10 vector<CIntelligentPointer<CThingAICreature>>
    CRetailVector<CBaseIntelligentPointer> MinionEffects;   // +0x1C
    long MinionAliveTimer;                          // +0x28
    long FadeInTimer;                               // +0x2C
    CRetailVector<long> CurFadeInTimer;             // +0x30
    void UpdateGhostSwordParticleEffects();         // 0x007C60A0
    void FrameUpdate();
};

void CTCGhostSword::FrameUpdate()
{
    for (long i = 0; i < (long)CurFadeInTimer.size(); ++i) {
        if (CurFadeInTimer[i] > 0) {
            CurFadeInTimer[i]--;
            if (IS_THING_ALIVE(Minions[i])) {
                CTCGraphicAppearance* appearance = Minions[i].GetPItem()->PTCGraphicAppearance;
                if (appearance)
                    appearance->SetAlpha((unsigned char)((1.0f - (float)CurFadeInTimer._M_start[i] / FadeInTimer) * 255.0f));
            }
        }
    }
    UpdateGhostSwordParticleEffects();
    CThing* parent = ParentThing;
    for (unsigned long j = 0; j < Minions.size(); ++j) {
        CBaseIntelligentPointer& minion = Minions[j];
        if (IS_THING_ALIVE(minion))
            return;
    }
    parent->RemoveTC(0x9a);
}
