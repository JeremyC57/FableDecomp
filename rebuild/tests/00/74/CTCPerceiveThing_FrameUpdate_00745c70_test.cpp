// Behaviour test for CTCPerceiveThing::FrameUpdate (0x00745C70): counter ageing, the every-fourth-frame / melee-entry update
// (reset when nothing is perceived), the automatic player perception and PerceptionCombat tracking the melee state.
#include <cstdio>
class CTCBase;
static int g_log[8]; static int g_n = 0; static long g_frame = 0; static const void* g_perceived = 0;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const { SEntry* it = first; while (it != last && it->type < *type) ++it; return it; }
};
class CThing { public:
    unsigned long m_presentBits[9];
    CInterfaceMap m_interfaces;
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
};
class CTCCombat { public: bool m_melee; bool IsInMelee() const { return m_melee; } };
static CThing g_hero;
class CPlayer { public: CThing* GetControlledCreature() const { return &g_hero; } };
static CPlayer g_player;
class CWorld { public:
    static long GetWorldFrame() { return g_frame; }
    const CPlayer* PeekMyPlayer() const { return &g_player; }
};
static CWorld g_world;
struct CBaseIntelligentPointer { CThing* m_pItem; CThing* GetPItem() const { return m_pItem; } };
class CPerceivedThingDef { public: long Type; };
class CTCPerceiveThing { public:
    CThing*       ParentThing;
    CBaseIntelligentPointer PerceivedThing;
    long          FramesSinceDirectlyPerceivedThing;
    bool          Perceiving;
    bool          SearchingForThingToPerceive;
    const CPerceivedThingDef* PDef;
    long          UpdateOffset;
    bool          PerceptionCombat;
    const CWorld* GetWorld() const { return &g_world; }
    bool IsUpdatingThisFrame() const { return CWorld::GetWorldFrame() % 4 == UpdateOffset; }
    void ResetPerceptionVariables() { g_log[g_n++] = 1; }
    void UpdateDistanceFromPerceivedThing() { g_log[g_n++] = 2; }
    void UpdatePerception() { g_log[g_n++] = 3; }
    void SetThingToPerceive(const CThing* thing) { g_log[g_n++] = 4; g_perceived = thing; }
    void FrameUpdate();
};

void CTCPerceiveThing::FrameUpdate()
{
    if (FramesSinceDirectlyPerceivedThing > -1)
        ++FramesSinceDirectlyPerceivedThing;
    bool inMelee;
    if (ParentThing->HasTC(0x30))
        inMelee = ((const CTCCombat*)ParentThing->GetTC(0x30))->IsInMelee();
    else
        inMelee = false;
    if (Perceiving) {
        if (IsUpdatingThisFrame() || (inMelee && !PerceptionCombat)) {
            if (PerceivedThing.GetPItem() == 0)
                ResetPerceptionVariables();
            else {
                UpdateDistanceFromPerceivedThing();
                UpdatePerception();
            }
        }
    } else if (SearchingForThingToPerceive && PDef->Type == 0)
        SetThingToPerceive(GetWorld()->PeekMyPlayer()->GetControlledCreature());
    PerceptionCombat = inMelee;
}

int main()
{
    int bad = 0;
    CTCCombat combat; combat.m_melee = false;
    CInterfaceMap::SEntry entries[1] = { { 0x30, (CTCBase*)&combat } };
    CThing creature; for (int i = 0; i < 9; ++i) creature.m_presentBits[i] = 0;
    creature.m_presentBits[1] = 1UL << 16; creature.m_interfaces.first = entries; creature.m_interfaces.last = entries + 1;
    CThing target;
    CPerceivedThingDef def; def.Type = 0;
    CTCPerceiveThing p; p.ParentThing = &creature; p.PerceivedThing.m_pItem = &target; p.FramesSinceDirectlyPerceivedThing = -1;
    p.Perceiving = true; p.SearchingForThingToPerceive = false; p.PDef = &def; p.UpdateOffset = 2; p.PerceptionCombat = true;
    g_frame = 5;                                        // 5 % 4 = 1: not this component's frame, not in melee
    p.FrameUpdate();
    if (g_n != 0 || p.FramesSinceDirectlyPerceivedThing != -1 || p.PerceptionCombat) bad |= 1;
    g_frame = 6; p.FramesSinceDirectlyPerceivedThing = 3;
    p.FrameUpdate();                                    // its frame: distance + perception
    if (g_n != 2 || g_log[0] != 2 || g_log[1] != 3 || p.FramesSinceDirectlyPerceivedThing != 4) bad |= 2;
    g_frame = 7; combat.m_melee = true;
    p.FrameUpdate();                                    // just entered melee: update now
    if (g_n != 4 || !p.PerceptionCombat) bad |= 4;
    p.FrameUpdate();                                    // still in melee, already flagged: no update
    if (g_n != 4) bad |= 8;
    p.PerceivedThing.m_pItem = 0; g_frame = 10;
    p.FrameUpdate();                                    // nothing perceived: reset
    if (g_n != 5 || g_log[4] != 1) bad |= 16;
    p.Perceiving = false; p.SearchingForThingToPerceive = true; def.Type = 1;
    p.FrameUpdate();
    if (g_n != 5) bad |= 32;
    def.Type = 0;
    p.FrameUpdate();                                    // searching for the player: perceive the controlled creature
    if (g_n != 6 || g_log[5] != 4 || g_perceived != &g_hero) bad |= 64;
    creature.m_presentBits[1] = 0;
    p.FrameUpdate();
    if (p.PerceptionCombat) bad |= 128;
    if (bad) { std::printf("00745c70_TEST FAIL\n"); return 1; }
    std::printf("00745c70_TEST PASS\n");
    return 0;
}