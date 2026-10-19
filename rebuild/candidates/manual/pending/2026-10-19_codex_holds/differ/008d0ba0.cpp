// Grade: HELD(bytes match retail) -- test exercises a hand-written stand-in, not the candidate | Codex source 00/8d/CGameScriptThing_IsAwareOfHero_008d0ba0.cpp, flags , pass pattern 008d0ba0_TEST PASS
// CGameScriptThing::IsAwareOfHero() -- checks the live thing's perception component and perceived hero
// Retail: 0x008d0ba0, 176 bytes.  The pimpl and interface map use the retail layouts.
struct CThing;
struct CIntelligentPointer { void* vft; CThing* p; CThing* GetPItem(); };
struct CTCBase;
struct CInterfaceMap { struct SEntry { long type; CTCBase* component; }; SEntry* first; SEntry* last; SEntry* LowerBound(const long*); };
struct CThing {
    unsigned char pad00[0x20]; unsigned long present[9]; CInterfaceMap interfaces; unsigned char pad4c[0x45]; unsigned char dead;
    unsigned char kind;
};
struct CTCPerceiveThing {
    unsigned char pad00[0x18]; float awareness;
    CThing* GetPerceivedThing() const; // 0x007446a0
    bool IsPlayerExtendedLineOfSight(); // 0x00744830
};
struct CGameScriptThing { unsigned char pad[0xc]; CIntelligentPointer thing; bool IsAwareOfHero(); };
extern float g_zero; // 0x0122dedc
bool CGameScriptThing::IsAwareOfHero()
{
    CIntelligentPointer* p = &thing;
    CThing* t;
    if (p->GetPItem() && !(p->GetPItem()->dead & 1) && (t = p->GetPItem())->present[1] & 0x100000) {
        long kind = 0x34;
        CInterfaceMap* map = &t->interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&kind);
        if (it == map->last || it->type > 0x34) it = map->last;
        CTCPerceiveThing* perceive = (CTCPerceiveThing*)it->component;
        if (perceive->IsPlayerExtendedLineOfSight()) return true;
        if (perceive->awareness == g_zero) return false;
        struct Target { unsigned char pad10[0x10]; unsigned char type; unsigned char pad11[0x80]; unsigned char dead; };
        Target* target = (Target*)perceive->GetPerceivedThing();
        if (target && !(target->dead & 1) && ((Target*)perceive->GetPerceivedThing())->type == 2) return true;
    }
    return false;
}