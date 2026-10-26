#pragma optimize("s", on)
// CTCVillage::SetEnemyToHeroStatus() -- through the village thing's enemy component (interface 73): add the hero as an enemy while the village stance is hostile (1) and the hero is alive, else remove the hero from its enemies
// Leaf: SetEnemyToHeroStatus
// Retail: Fable.exe @ 0x0065A852, 156 bytes; size TU (unaligned entry): `optimize("s",on)`.
// Callees: std::bitset<>::test(size_t) const 0x00410DE0 (interface 73); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
//          0x0040F020 (inlined GetInterface); CTCBase::GetPlayerManager() 0x00686D40 (x2); CPlayerManager::GetMainPlayer()
//          0x00449970 (x2); CPlayer::GetCharacterThing() 0x00487DC0 (x2); IS_THING_ALIVE<CThing*>(CThing* const&) 0x00436380
//          (__fastcall); CTCEnemy::AddEnemyThing(CThing*) 0x0076D1B0 (manifest "CCreature_AddEnemyToList" is a gap-scan
//          name), CTCEnemy::RemoveEnemyThing(CThing*) 0x0076CC00.
// Layout: retail CTCVillage StanceToHero +0x98 (PDB +0x124, minus 0x8c); CTCBase::ParentThing +0x04.
// Shape: the out-pointer GetInterface with its result ignored: the component local is UNinitialised and shares the
//        [ebp-4] slot with the lookup key (the false arm loads it as is) and later with the hero pointer whose address
//        IS_THING_ALIVE takes.
class CTCBase;
class CThing;

struct CBitset {
    bool test(unsigned long position) const;    // 0x00410DE0
};
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    CBitset       m_present;                    // +0x20
    unsigned char _pad24[0x20];
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_present.test(interfaceType)) {
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
bool __fastcall IS_THING_ALIVE_00436380(CThing* const& thing);   // 0x00436380

class CPlayer { public:
    CThing* GetCharacterThing();                // 0x00487DC0
};
class CPlayerManager { public:
public:
    CPlayer& GetMainPlayer();   // 0x00449970
};
class CTCEnemy { public:
    void AddEnemyThing(CThing* enemy);          // 0x0076D1B0
    void RemoveEnemyThing(CThing* enemy);       // 0x0076CC00
};

class CTCBase
{
public:
    virtual void v00();
protected:
    CPlayerManager& GetPlayerManager();   // 0x00686D40
};

class CTCVillage : public CTCBase { public:
    CThing*       ParentThing;                  // +0x04
    unsigned char _pad_0x08[0x90];
    long          StanceToHero;                 // +0x98 EVillageStanceToHero
    void SetEnemyToHeroStatus();
};

void CTCVillage::SetEnemyToHeroStatus()
{
    CTCEnemy* enemy;
    ParentThing->GetInterface(&enemy, 73);
    if (StanceToHero == 1) {
        CThing* hero = GetPlayerManager().GetMainPlayer().GetCharacterThing();
        if (IS_THING_ALIVE_00436380(hero))
            enemy->AddEnemyThing(hero);
    } else {
        enemy->RemoveEnemyThing(GetPlayerManager().GetMainPlayer().GetCharacterThing());
    }
}