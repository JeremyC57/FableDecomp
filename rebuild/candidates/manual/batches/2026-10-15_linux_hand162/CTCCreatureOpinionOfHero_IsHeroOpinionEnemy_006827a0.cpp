// CTCCreatureOpinionOfHero::IsHeroOpinionEnemy() -- the hero is this creature's enemy: either flagged directly or, for a creature with interface 0xB4, when its village treats the hero as an enemy of the guards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006827A0, length 99 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface for type 0x23 = CTCVillageMember,
// `test byte ptr [eax+24h],8`), CTCVillageMember::GetVillage @ 0x007EE540, CTCVillage::GetIsHeroEnemyOfGuards @ 0x00658604.
// The interface-0xB4 presence test (word 5 bit 20, `test dword ptr [eax+34h],100000h`) is an inline HasTC without lookup that
// returns the masked word (a `bool` HasTC compiles to `shr ecx,14h ; test cl,1`).
// Shape: nested ifs with two `return true` (one shared `mov al,1`) and a single `xor al,al` exit.
// Model: engine/CTCCreatureOpinionOfHero.h is the PDB layout; retail STLport lists are 4 bytes (PDB 12), so HeroOpinionEnemy
// sits at +0x342 (as in the landed SetHeroOpinionEnemy 0x006815D0 and KnowsOfDeed 0x00681AE0), hence a local model.
// Retail CThing: interface bitset +0x20, map +0x44, Dead bit +0x91.
struct CTCBaseInterface;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBaseInterface* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x004365B0
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
struct CTCVillage {
    bool GetIsHeroEnemyOfGuards();              // 0x00658604
};
struct CTCVillageMember {                       // interface 0x23
    CTCVillage* GetVillage();                   // 0x007EE540 (the manifest's "IS_THING_ALIVE<...>" is propagated)
};
struct CTCCreatureOpinionOfHero {
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _pad08[0x342 - 0x08];
    bool HeroOpinionEnemy;                      // +0x342 (PDB +0x64e)
    bool IsHeroOpinionEnemy();
};

bool CTCCreatureOpinionOfHero::IsHeroOpinionEnemy()
{
    if (HeroOpinionEnemy)
        return true;
    CThing* thing = ParentThing;
    if (thing->HasTC(0xb4)) {
        CTCVillageMember* member;
        if (thing->GetInterface(&member, 0x23)) {
            CTCVillage* village = member->GetVillage();
            if (village && village->GetIsHeroEnemyOfGuards())
                return true;
        }
    }
    return false;
}
