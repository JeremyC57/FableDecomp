// CTCTurncoatSpell::StopTurncoatingEffects() -- stops the spell's looping sound on the caster's sound player and fades out the two turncoating effect things that are still alive
// Leaf: StopTurncoatingEffects
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C6DA0, 167 bytes (plain `ret`). Ego_r.pdb
// `?StopTurncoatingEffects@CTCTurncoatSpell@@AAEXXZ`.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (interface 0x10, the sound player: word 0 bit 16);
//          CTCSoundPlayer::StopSample(unsigned long) 0x0066ADD0 (landed spelling of CTCFireballSpell::CancelMove 0x007C8FD0);
//          CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (three per effect: IS_THING_ALIVE, then the target);
//          the effect thing's fade-out 0x0072AC40 (CThing::FadeOutEffect(), as the landed sibling FinalCleanup
//          0x007C75E0 declares it).
// Layout: engine/CTCTurncoatSpell.h carries Ego_r.pdb offsets; retail members after the timer sit 4 bytes lower (the
//         landed FinalCleanup model): LoopingSound +0x50, TurncoatingEffectOnMe +0x74, TurncoatingEffectOnTarget +0x7c.
// Shape: the out-parameter GetInterface in an `if` (presence test straight on the bitset word, `test dword ptr [eax+20h]`)
//        with the sound stop and the `LoopingSound = 0` clear as its body; two IS_THING_ALIVE blocks on &member in esi.
struct CTCBase;

struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*>
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad_0x4c[0x45];
    unsigned char m_flags;                      // +0x91 bit 0: dead
    bool IsDead() const { return (m_flags & 1) != 0; }
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
    void FadeOutEffect();                       // 0x0072AC40
};

struct CTCSoundPlayer {                         // interface 0x10
    void StopSample(unsigned long sampleID);    // 0x0066ADD0
};

struct CBaseIntelligentPointer {
    void* __vftable;                            // +0x00
    void* PData;                                // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

struct CTCTurncoatSpell {
    void*                   __vftable;          // +0x00
    CThing*                 ParentThing;        // +0x04 (CTCBase)
    unsigned char           _pad_0x08[0x48];
    unsigned long           LoopingSound;       // +0x50
    unsigned char           _pad_0x54[0x20];
    CBaseIntelligentPointer TurncoatingEffectOnMe;       // +0x74
    CBaseIntelligentPointer TurncoatingEffectOnTarget;   // +0x7c
    CBaseIntelligentPointer TurncoatingEffectOnMeOutOfRange;   // +0x84
    void StopTurncoatingEffects();
};

void CTCTurncoatSpell::StopTurncoatingEffects()
{
    CTCSoundPlayer* sound;
    if (ParentThing->GetInterface(&sound, 0x10)) {
        sound->StopSample(LoopingSound);
        LoopingSound = 0;
    }
    if (IS_THING_ALIVE(TurncoatingEffectOnMe))
        TurncoatingEffectOnMe.GetPItem()->FadeOutEffect();
    if (IS_THING_ALIVE(TurncoatingEffectOnTarget))
        TurncoatingEffectOnTarget.GetPItem()->FadeOutEffect();
}
