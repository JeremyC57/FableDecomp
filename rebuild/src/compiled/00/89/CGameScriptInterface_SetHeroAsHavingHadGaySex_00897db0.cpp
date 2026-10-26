// CGameScriptInterface::SetHeroAsHavingHadGaySex(bool) const -- script call: marks the hero as having had (scripted) gay sex on the main player's hero (skipped when there is no hero, the hero is dead, or it lacks the CTCHeroStats component)
// Leaf: ?SetHeroAsHavingHadGaySex@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897DB0, 91 bytes (96 with int3 padding), thiscall `ret 4`.
// Identity (proven): CGameScriptInterface::SetHeroAsHavingHadGaySex; evidence: retail CGameScriptInterface vtable 0x01260F0C slot 221 holds this body, and that slot is ?SetHeroAsHavingHadGaySex@CGameScriptInterface@@UBEX_N@Z in ghidra_out/cgsi_slots_aligned.tsv (the Ego_r.pdb CGameScriptInterface virtual order aligned to the retail table); the body tests interface 0x4 and stores a bool at +0x159, which is CTCHeroStats::HadScriptedGaySex (Ego_r.pdb +0x16D), set through FableWin.pdb ?SetAsHavingScriptedGaySex@CTCHeroStats@@QAEX_N@Z.
// Label: the manifest's "CGameScriptInterface::SetHeroAsHavingHadGaySex" is propagated across family H029 (seven hero-component setters that differ only in the
//   component's interface bit, the stored field and its width).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPPlayerCharacter 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CThing::GetTC<CTCHeroStats> inlined, interface 0x4).
// Model: the landed CGameScriptInterface::SetHeroAbleToGainExperience 0x00899060 (same shape, other component). Layout:
//   CGameScriptInterface +0x14 PlayerManager; CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0;
//   CTCHeroStats::HadScriptedGaySex retail +0x159 (Ego_r.pdb CTCHeroStats HadScriptedGaySex +0x16d; retail is 0x14 lower before HousePositions (retail +0x160, 12-byte vector) and 0x18 lower after it, as the landed CTCHeroStats models (HadScriptedSex +0x158, FailQuestOnDeath +0x174) pin).
// Setter: CTCHeroStats::SetAsHavingScriptedGaySex(bool) (FableWin.pdb ?SetAsHavingScriptedGaySex@CTCHeroStats@@QAEX_N@Z), inline.
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
    template <class T> __forceinline bool GetTC(T** out)
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
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CTCHeroStats {
public:
    enum { INTERFACE_TYPE = 0x4 };
    unsigned char _pad00[0x159];
    bool HadScriptedGaySex;                                    // +0x159
    void SetAsHavingScriptedGaySex(bool value) { HadScriptedGaySex = value; }
};
class CGameScriptInterface {
public:
    virtual void SetHeroAsHavingHadGaySex(bool value) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetHeroAsHavingHadGaySex(bool value) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCHeroStats* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc))
        tc->SetAsHavingScriptedGaySex(value);
}