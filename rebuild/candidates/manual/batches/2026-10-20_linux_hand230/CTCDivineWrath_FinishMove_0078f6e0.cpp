// CTCDivineWrath::FinishMove() -- finish the spell: start the level's fade-out wait, fade the environment back to its default theme, reset the caster's interface-0xA7 component, fade or kill the build-up and hand effects and stop the looping sound
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078F6E0, 319 bytes (+ int3 padding), plain `ret`.
// Callees: __ftol2 @ 0x00BFEA70, CTCBase::GetWorld @ 0x00686A70, CEnvironment::TransitionToDefaultThemeAllChannels(long,
//   float) @ 0x006B3950, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020 (inlined lookups of interface 0xA7
//   = word 5 bit 7, and 0x10 = the sound emitter, word 0 bit 16), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x6),
//   CTCSpecialAbilities::FadeOrKillEffect(CIntelligentPointer<CThing>&) @ 0x0072ABD0 (__fastcall, the pointer in ecx;
//   manifest "CCreatureAction_CastWillReclaimUndeadFollower::PerformAction" is propagated), the emitter's StopSample(ulong)
//   @ 0x0066ADD0 (manifest "CGameScriptThing::SetToKillOnLevelUnload" is propagated).
// Globals: g_framesPerSecond @ 0x01375550.
// Layout: Ego_r.pdb CTCDivineWrath with the retail containers (fields after Timer 4 lower, after the map/vector 0xc
//   lower): PDef +0x0c, SpellLevel +0x50, Finished +0x64, NumFramesToWaitForFadeOut +0x68, BuildUpEffect +0x9c, HandEffect1
//   +0xac, HandEffect2 +0xb4, LoopingSound +0xbc; CSpecialAbilitiesDivineWrathDef per-level fade-out seconds (vector<float>
//   first pointer) +0xc4; CWorld environment +0x18; the 0xA7 component's flag +0x0c and counter +0x10 (names descriptive).
// Shape: `Finished = true` loads 1 into bl, which VC7.1 reuses as the dead-bit mask of the three inline IS_THING_ALIVE
//   tests; the per-level time is read inline for the frame count, then through a named `times` pointer local (its load
//   lands before GetWorld, the level after it) with the world in a named local (GetWorld before the argument, the
//   environment read straight into ecx); the 0xA7 component comes from HasTC + GetTC (one ternary entry pick), the
//   sound emitter from the out-parameter GetInterface (`mov eax,[eax+4]` ... `mov ecx,eax`).
extern long g_framesPerSecond;                  // 0x01375550
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
struct CEnvironment {
    void TransitionToDefaultThemeAllChannels(long channels, float time);   // 0x006B3950
};
struct CWorld {
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
struct CSpecialAbilitiesDivineWrathDef {
    unsigned char _pad00[0xc4];
    float* FadeOutTimes;                        // +0xc4 vector<float> first pointer (per spell level)
};
struct CTCDivineWrath {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x0c - 0x08];
    const CSpecialAbilitiesDivineWrathDef* PDef;    // +0x0c CDefPointer
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
    unsigned long HeroDefaultChannel;           // +0xa4
    unsigned long SpellExclusionChannel;        // +0xa8
    CBaseIntelligentPointer HandEffect1;        // +0xac
    CBaseIntelligentPointer HandEffect2;        // +0xb4
    unsigned long LoopingSound;                 // +0xbc
    CWorld* GetWorld();                         // 0x00686A70 (CTCBase)
    void FinishMove();
};

void CTCDivineWrath::FinishMove()
{
    Finished = true;
    NumFramesToWaitForFadeOut = (long)(g_framesPerSecond * PDef->FadeOutTimes[SpellLevel]);
    const float* times = PDef->FadeOutTimes;
    CWorld* world = GetWorld();
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
    unsigned long sound = LoopingSound;
    CThing* caster = ParentThing;
    CTCSoundEmitter* emitter;
    if (caster->GetInterface(&emitter, 0x10))
        emitter->StopSample(sound);
}
