// CGameScriptInterface::GiveHeroTutorial(ETutorialCategory) const -- gives the hero the tutorial for a category; false when there is no live hero
// Leaf: ?GiveHeroTutorial@CGameScriptInterface@@UBE_NW4ETutorialCategory@@@Z
// Retail: Fable.exe @ 0x00898A50, 105 bytes. Calls CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPPlayerCharacter 0x00487DC0,
// then CThing::GetTC<CTCHero> (Ego_r.pdb bool/out-pointer form; Components lower_bound 0x0040F020); CTCHero::GiveTutorialForCategory 0x006C9FA0.
// Layout: CGameScriptInterface per engine/CGameScriptInterface.h (+0x14 PlayerManager); CThing retail (UsingComponents +0x20,
// Components +0x44, Dead +0x91 bit 0).
// Shape: GetTC's end-or-mismatch choice is one ternary whose ->component is stored (`*out = ((it == last ||
// type < it->type) ? last : it)->component`); the if/assign form tests the TC in eax and copies it to ecx (2 bytes longer).
class CTCBase;
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
        *out = (T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
        return true;
    }
    template <class T> __forceinline bool PeekTC(const T** out) const  // ??$PeekTC@$$CBV...@@@CThing@@QBE_NPAPBV...@@@Z
    {
        if (!HasTC(T::INTERFACE_TYPE))
            return false;
        long type = T::INTERFACE_TYPE;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        *out = (const T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
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
enum ETutorialCategory { TUTORIAL_CATEGORY_FORCE_DWORD = 0x7fffffff };
class CTCHero {
public:
    enum { INTERFACE_TYPE = 0x29 };
    bool GiveTutorialForCategory(ETutorialCategory category, bool force);   // 0x006C9FA0
};
class CGameScriptInterface {
public:
    virtual bool GiveHeroTutorial(ETutorialCategory category) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

bool CGameScriptInterface::GiveHeroTutorial(ETutorialCategory category) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCHero* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc) && tc)
        return tc->GiveTutorialForCategory(category, false);
    return false;
}