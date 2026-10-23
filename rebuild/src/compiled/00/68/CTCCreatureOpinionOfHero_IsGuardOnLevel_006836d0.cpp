// CTCCreatureOpinionOfHero::IsGuardOnLevel() -- true when the creature's village has a living guard member (a thing holding interface 0xB4)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006836D0, length 152 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface for type 0x23 = CTCVillageMember),
// CTCVillageMember::GetVillage @ 0x007EE540 (twice: the null test, then the list), CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3).
// Layout: the retail CTCVillage keeps a single-pointer STLport list<CIntelligentPointer<CThing>> at +0x2C (its members; node
// {next, prev, pointer}); interface 0xB4 is word 5 bit 20 of the presence bitset.
// Shape: the list is a reference local (`add ebp,2Ch`, the end re-read through it each pass); inline IS_THING_ALIVE then a
// masked-word HasTC; nested ifs with one `xor al,al` exit.
// Model: engine/CTCCreatureOpinionOfHero.h is the PDB layout; retail STLport lists are 4 bytes (PDB 12), so HeroOpinionEnemy
// sits at +0x342 (as in the landed SetHeroOpinionEnemy 0x006815D0 and KnowsOfDeed 0x00681AE0), hence a local model.
// Retail CThing: interface bitset +0x20, map +0x44, Dead bit +0x91.
struct CTCBaseInterface;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Flags91;                      // +0x91 bit 0 Dead
    bool IsDead() const { return (Flags91 & 1) != 0; }
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
};
struct CTCVillage;
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetVillage();                   // 0x007EE540 (the manifest's "IS_THING_ALIVE<...>" is propagated)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
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
struct SThingPtrNode { SThingPtrNode* next; SThingPtrNode* prev; CBaseIntelligentPointer value; };
struct CThingPtrList {                          // list<CIntelligentPointer<CThing> > (retail: one sentinel pointer)
    SThingPtrNode* head;
};
struct CTCVillage {
    unsigned char _pad00[0x2c];
    CThingPtrList Members;                      // +0x2c
    bool GetIsHeroEnemyOfGuards();              // 0x00658604
};
struct CTCCreatureOpinionOfHero {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    bool IsGuardOnLevel();
};

bool CTCCreatureOpinionOfHero::IsGuardOnLevel()
{
    CTCVillageMember* member;
    if (ParentThing->GetInterface(&member, 0x23)) {
        if (member->GetVillage()) {
            CThingPtrList& members = member->GetVillage()->Members;
            for (SThingPtrNode* node = members.head->next; node != members.head; node = node->next) {
                if (IS_THING_ALIVE(node->value) && node->value.GetPItem()->HasTC(0xb4))
                    return true;
            }
        }
    }
    return false;
}