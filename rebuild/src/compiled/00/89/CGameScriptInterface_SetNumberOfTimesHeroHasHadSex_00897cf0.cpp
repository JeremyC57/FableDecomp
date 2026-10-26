// CGameScriptInterface::SetNumberOfTimesHeroHasHadSex(long) const -- script call: sets how many times the hero has had sex on the main player's hero (skipped when there is no hero, the hero is dead, or it lacks the CTCHeroStats component)
// Leaf: ?SetNumberOfTimesHeroHasHadSex@CGameScriptInterface@@UBEXJ@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00897CF0, 91 bytes (96 with int3 padding), thiscall `ret 4`.
// Identity (proven): CGameScriptInterface::SetNumberOfTimesHeroHasHadSex; evidence: retail CGameScriptInterface vtable 0x01260F0C slot 219 holds this body, and that slot is ?SetNumberOfTimesHeroHasHadSex@CGameScriptInterface@@UBEXJ@Z in ghidra_out/cgsi_slots_aligned.tsv (the Ego_r.pdb CGameScriptInterface virtual order aligned to the retail table); the body tests interface 0x4 and stores a long at +0x154, which is CTCHeroStats::NumSex (Ego_r.pdb +0x168), set through FableWin.pdb ?SetNumberOfTimesHadSex@CTCHeroStats@@QAEXJ@Z.
// Label: the manifest's "CGameScriptInterface::SetNumberOfTimesHeroHasHadSex" is propagated across family H029 (seven hero-component setters that differ only in the
//   component's interface bit, the stored field and its width).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPPlayerCharacter 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CThing::GetTC<CTCHeroStats> inlined, interface 0x4).
// Model: the landed CGameScriptInterface::SetHeroAbleToGainExperience 0x00899060 (same shape, other component). Layout:
//   CGameScriptInterface +0x14 PlayerManager; CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0;
//   CTCHeroStats::NumSex retail +0x154 (Ego_r.pdb CTCHeroStats NumSex +0x168; retail is 0x14 lower before HousePositions (retail +0x160, 12-byte vector) and 0x18 lower after it, as the landed CTCHeroStats models (HadScriptedSex +0x158, FailQuestOnDeath +0x174) pin).
// Setter: CTCHeroStats::SetNumberOfTimesHadSex(long) (FableWin.pdb ?SetNumberOfTimesHadSex@CTCHeroStats@@QAEXJ@Z), inline.
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
    unsigned char _pad00[0x154];
    long NumSex;                                    // +0x154
    void SetNumberOfTimesHadSex(long value) { NumSex = value; }
};
class CGameScriptInterface {
public:
    virtual void SetNumberOfTimesHeroHasHadSex(long value) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetNumberOfTimesHeroHasHadSex(long value) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPPlayerCharacter();
    CTCHeroStats* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc))
        tc->SetNumberOfTimesHadSex(value);
}