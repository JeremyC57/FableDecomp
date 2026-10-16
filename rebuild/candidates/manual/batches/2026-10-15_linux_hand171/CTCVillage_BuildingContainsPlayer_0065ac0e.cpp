#pragma optimize("s", on)
// CTCVillage::BuildingContainsPlayer(CThing* building) -- true when the live hero's building-location component (interface 117) says the hero is inside this building
// Leaf: BuildingContainsPlayer
// Retail: Fable.exe @ 0x0065AC0E, 117 bytes (`ret 4`); size TU (unaligned entry): `optimize("s",on)`.
// Callees: CTCBase::GetPlayerManager() 0x00686D40; CPlayerManager::GetCurrentPlayer() 0x00449970; CPlayer::
//          GetCharacterThing() 0x00487DC0; IS_THING_ALIVE<CThing*>(CThing* const&) 0x0043DBD0 (__fastcall); std::bitset<>::
//          test(size_t) const 0x00410DE0 (interface 117); CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020
//          (inlined GetTC, key in the hero's frame slot); CBaseIntelligentPointer::GetPItem 0x00A01B50.
// Layout: the interface-117 component holds the building as a CIntelligentPointer<CThing> at +0x0c (as
//         CTCTrespasser::IsCurrentlyTrespassing 0x007BAE70).
// Shape: the hero (a CThingCreatureBase*) is kept in esi; IS_THING_ALIVE<CThing*> binds its const reference to a converted
//        temporary at [ebp-4] (so no reload after the call); that slot is then reused for the GetTC key;
//        nested ifs with one `xor al,al` exit.
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
    bool HasTC(long type) const { return m_present.test(type); }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return it->component;
    }
};
bool __fastcall IS_THING_ALIVE(CThing* const& thing);   // 0x0043DBD0

struct CBaseIntelligentPointer {
    void* _vfptr;
    CThing* m_pItem;
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CTCBuildingLocation {                    // the hero's interface-117 component (class name unknown)
    unsigned char _pad_0x00[0xc];
    CBaseIntelligentPointer PBuilding;          // +0x0c
};

class CThingCreatureBase : public CThing {};
struct CPlayer {
    CThingCreatureBase* GetCharacterThing();    // 0x00487DC0
};
struct CPlayerManager {
    CPlayer* GetCurrentPlayer();                // 0x00449970
};

struct CTCVillage {
    virtual void v00();
    CPlayerManager* GetPlayerManager();         // 0x00686D40 (CTCBase)
    bool BuildingContainsPlayer(CThing* building);
};

bool CTCVillage::BuildingContainsPlayer(CThing* building)
{
    CThingCreatureBase* hero = GetPlayerManager()->GetCurrentPlayer()->GetCharacterThing();
    if (IS_THING_ALIVE(hero) && hero->HasTC(117)) {
        if (((CTCBuildingLocation*)hero->GetTC(117))->PBuilding.GetPItem() == building)
            return true;
    }
    return false;
}
