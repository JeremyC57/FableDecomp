// CTCUnholyPower::FinishMove() -- finish the spell: start the level's fade-out wait, fade the environment back to its default theme, reset the caster's interface-0xA7 component, fade or kill the build-up and hand effects, kill the dark-faces effect and stop the looping sound
// Leaf: FinishMove
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00794C10, length 369 bytes (plain `ret`, no frame pointer; Ego_r.pdb
//   `?FinishMove@CTCUnholyPower@@QAEXXZ`).
// Callees: as the landed sibling CTCDivineWrath::FinishMove 0x0078F6E0: __ftol2 0x00BFEA70, CTCBase::GetWorld 0x00686A70,
//   CEnvironment::TransitionToDefaultThemeAllChannels(long, float) 0x006B3950, CVectorMap LowerBound 0x0040F020
//   (interfaces 0xA7 and 0x10), CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCSpecialAbilities::FadeOrKillEffect
//   (CIntelligentPointer<CThing>&) 0x0072ABD0 (__fastcall), the emitter's StopSample(unsigned long) 0x0066ADD0; plus
//   CThing::Kill(bool) 0x004C9B80 (as landed) on the dark-faces effect.
// Globals: g_framesPerSecond 0x01375550.
// Layout: Ego_r.pdb CTCUnholyPower with the retail containers (as the sibling: after Timer 4 lower, after the map/vector
//   0xc lower): PDef +0x0c, SpellLevel +0x50, Finished +0x64, NumFramesToWaitForFadeOut +0x68, BuildUpEffect +0x9c,
//   DarkFacesEffect +0xa4, HandEffect1 +0xb4, HandEffect2 +0xbc, LoopingSound +0xc4; the def's per-level fade-out seconds
//   (vector<float> first pointer) +0xc4 as the sibling's def (descriptive); CWorld environment +0x18.
// Shape: the sibling's body (bl = 1 from `Finished = true` reused as the dead-bit mask and the Kill argument; the named
//   `times` pointer and world locals; HasTC + GetTC for 0xA7); the dark-faces effect is killed through its item
//   (`DarkFacesEffect.GetPItem()->Kill(true)`, the argument pushed before the third GetPItem); the sound stops through
//   an inline StopSound(sound) member (Ego_r.pdb `?StopSound@CTCUnholyPower@@AAEXK@Z`) whose argument loads into edi
//   before the lookup.
extern long g_framesPerSecond;                  // 0x01375550
class CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool immediately);                // 0x004C9B80
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline T* GetTC(long wanted)
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {                // CIntelligentPointer<T>: vptr + item
    void*   __vftable;                          // +0x00
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}
void __fastcall FadeOrKillEffect(CBaseIntelligentPointer& effect);   // 0x0072ABD0
class CEnvironment { public:
    void TransitionToDefaultThemeAllChannels(long channels, float time);   // 0x006B3950
};
class CWorld { public:
    unsigned char _pad00[0x18];
    CEnvironment* PEnvironment;                 // +0x18
};
struct CTCSoundEmitter {                        // interface 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};
struct CTCInterfaceA7 {                         // interface 0xA7 component
    unsigned char _pad00[0x0c];
    bool m_active;                              // +0x0c
    long m_count;                               // +0x10
};
class CSpecialAbilitiesUnholyPowerDef { public:
    unsigned char _pad00[0xc4];
    float* FadeOutTimes;                        // +0xc4 vector<float> first pointer (per spell level; descriptive)
};
class CTCBase
{
public:
    virtual void v00();
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCUnholyPower : public CTCBase { public:
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    const CSpecialAbilitiesUnholyPowerDef* PDef;    // +0x0c CDefPointer
    unsigned char _pad10[0x50 - 0x10];          // PItemRemoved .. Timer
    long  SpellLevel;                           // +0x50
    long  NumFramesBetweenDamageHit;            // +0x54
    long  DurationOfSpellFrames;                // +0x58
    long  NumFramesForEnvChange;                // +0x5c
    long  CurFramesForEnvChange;                // +0x60
    bool  Finished;                             // +0x64
    long  NumFramesToWaitForFadeOut;            // +0x68
    unsigned char _pad6c[0x9c - 0x6c];          // CutSceneCancel .. StaminaModify
    CBaseIntelligentPointer BuildUpEffect;      // +0x9c
    CBaseIntelligentPointer DarkFacesEffect;    // +0xa4
    unsigned long HeroDefaultChannel;           // +0xac
    unsigned long SpellExclusionChannel;        // +0xb0
    CBaseIntelligentPointer HandEffect1;        // +0xb4
    CBaseIntelligentPointer HandEffect2;        // +0xbc
    unsigned long LoopingSound;                 // +0xc4
    void StopSound(unsigned long sound)
    {
        CTCSoundEmitter* emitter;
        if (ParentThing->GetInterface(&emitter, 0x10))
            emitter->StopSample(sound);
    }
    void FinishMove();
};

void CTCUnholyPower::FinishMove()
{
    Finished = true;
    NumFramesToWaitForFadeOut = (long)(g_framesPerSecond * PDef->FadeOutTimes[SpellLevel]);
    const float* times = PDef->FadeOutTimes;
    CWorld* world = (&GetWorld());
    world->PEnvironment->TransitionToDefaultThemeAllChannels(2, times[SpellLevel]);
    if (ParentThing->HasTC(0xa7)) {
        CTCInterfaceA7* tc = ParentThing->GetTC<CTCInterfaceA7>(0xa7);
        tc->m_active = false;
        tc->m_count = 0;
    }
    if (IS_THING_ALIVE(BuildUpEffect))
        FadeOrKillEffect(BuildUpEffect);
    if (IS_THING_ALIVE(HandEffect1))
        FadeOrKillEffect(HandEffect1);
    if (IS_THING_ALIVE(HandEffect2))
        FadeOrKillEffect(HandEffect2);
    if (IS_THING_ALIVE(DarkFacesEffect))
        DarkFacesEffect.GetPItem()->Kill(true);
    StopSound(LoopingSound);
}