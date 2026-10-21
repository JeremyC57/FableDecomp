// CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const -- how far away the hero can be heard (CTCStealth); 0.0f when there is no live hero
// Leaf: GetDistanceHeroCanBeHeardFrom@CGameScriptInterface
// Retail: Fable.exe @ 0x00899700, 101 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPPlayerCharacter 0x00487DD0,
// then CThing::PeekTC (Ego_r.pdb bool/out-pointer form; const Components lower_bound 0x004365B0).
// Shape: PeekTC's result is not tested -- without the TC bit retail reads through the uninitialised out pointer.
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// CTCStealth (Ego_r.pdb, interface 0x36): DistanceCanBeHeardFrom +0x14 (no STL member precedes it).
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry*       LowerBound(const long* type);          // 0x0040F020
    const SEntry* LowerBound(const long* type) const;    // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                              // +0x91 bit 0
    bool IsDead() const { return Dead; }
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline bool GetTC(T** out)               // ??$GetTC@V...@@@CThing@@QAE_NPAPAV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (T*)it->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || T::INTERFACE_TYPE < it->type)
            it = map->last;
        *out = (const T*)it->component;
        return true;
    }
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature*       GetPPlayerCharacter() const;       // 0x00487DC0
    const CThingPlayerCreature* PeekPPlayerCharacter() const;      // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                                       // 0x00449970
};
class CTCStealth {
public:
    enum { INTERFACE_TYPE = 0x36 };
    unsigned char _pad00[0x14];
    float         DistanceCanBeHeardFrom;                // +0x14
    float GetDistanceCanBeHeardFrom() const { return DistanceCanBeHeardFrom; }
};
class CGameScriptInterface {
public:
    virtual float GetDistanceHeroCanBeHeardFrom() const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

float CGameScriptInterface::GetDistanceHeroCanBeHeardFrom() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPPlayerCharacter();
    if (hero && !hero->IsDead())
    {
        const CTCStealth* tc;
        hero->PeekTC(&tc);
        return tc->GetDistanceCanBeHeardFrom();
    }
    return 0.0f;
}