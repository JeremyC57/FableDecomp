// CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea() -- while the quest manager has boasts and the hero has not boasted yet, give the living hero the boasting tutorial (category 5) and enter crowd state 1
// Grade: DIFFER(137v135) -- the hero component lands in eax and is moved to ecx (`mov eax,[eax+4] ; test eax,eax ; mov ecx,eax`) where retail loads it straight into ecx (`mov ecx,[eax+4] ; test ecx,ecx`); tried out-param && test, inline accessor returning 0, pointer-returning lookup, preset-zero out-param
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00784600, length 135 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x0040F020
// (inlined GetInterface for type 0x29 = bit 9 of the presence word at +0x24: `mov ecx,[eax+24h] ; test ch,2`),
// CTCHero::GiveTutorialForCategory(ETutorialCategory, bool) @ 0x006C9FA0, EnterCrowdState(EBoastingCrowdState) @ 0x00784260.
// Globals: the quest manager @ 0x013B89FC; its retail +0x68 member is a single-pointer STLport list (empty when the
// sentinel links to itself), most likely the PDB's Boasts list.
// Layout: retail = engine/CTCCreatureGroupBoastingCrowd.h - 4 (the retail member map is 12 bytes): PHero +0x24,
// HeroBoasted +0x34. Local model (the header flattens the vptr and has the PDB offsets).
// Shape: one guarded block; the hero tutorial is an inline IsThingAlive test then GetInterface + null test.
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
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
};
struct CTCHero {                                // interface 0x29
    bool GiveTutorialForCategory(long category, bool force);   // 0x006C9FA0
};
struct SListNode { SListNode* next; SListNode* prev; };
struct CQuestManager {
    unsigned char _pad00[0x68];
    SListNode* m_boasts;                        // +0x68 list head (retail 4-byte STLport list)
    bool BoastsEmpty() const { return m_boasts->next == m_boasts; }
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCCreatureGroupBoastingCrowd {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x24 - 0x08];          // CTCBase tail + CTCCreatureGroup members
    CBaseIntelligentPointer PHero;              // +0x24 CIntelligentPointer<CThing>
    long CrowdState;                            // +0x2c
    long CurrentEncouragementState;             // +0x30
    bool HeroBoasted;                           // +0x34
    bool HeroMadeGoodBoast;                     // +0x35
    void EnterCrowdState(long state);           // 0x00784260
    void OnHeroApproachBoastingArea();
};

void CTCCreatureGroupBoastingCrowd::OnHeroApproachBoastingArea()
{
    if (!g_pQuestManager->BoastsEmpty() && !HeroBoasted) {
        if (PHero.GetPItem() && !PHero.GetPItem()->IsDead()) {
            CTCHero* hero = 0;
            PHero.GetPItem()->GetInterface(&hero, 0x29);
            if (hero)
                hero->GiveTutorialForCategory(5, false);
        }
        EnterCrowdState(1);
    }
}
