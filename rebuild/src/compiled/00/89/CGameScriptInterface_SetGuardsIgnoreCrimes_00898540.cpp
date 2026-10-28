// CGameScriptInterface::SetGuardsIgnoreCrimes(bool) const -- script call: makes guards ignore (or notice) the hero's crimes on the main player's hero (skipped when there is no hero, the hero is dead, or it lacks the CTCHeroOpinionDeedLog component)
// Leaf: ?SetGuardsIgnoreCrimes@CGameScriptInterface@@UBEX_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00898540, 91 bytes (96 with int3 padding), thiscall `ret 4`.
// Identity (proven): CGameScriptInterface::SetGuardsIgnoreCrimes; evidence: retail CGameScriptInterface vtable 0x01260F0C slot 134 holds this body, and that slot is ?SetGuardsIgnoreCrimes@CGameScriptInterface@@UBEX_N@Z in ghidra_out/cgsi_slots_aligned.tsv (the Ego_r.pdb CGameScriptInterface virtual order aligned to the retail table); the body tests interface 0x51 and stores a bool at +0x2D, which is CTCHeroOpinionDeedLog::GuardsIgnoreCrimes (Ego_r.pdb +0x39), set through FableWin.pdb ?SetGuardsIgnoreCrimes@CTCHeroOpinionDeedLog@@QAEX_N@Z.
// Label: the manifest's "CGameScriptInterface::SetGuardsIgnoreCrimes" is propagated across family H029 (seven hero-component setters that differ only in the
//   component's interface bit, the stored field and its width).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::GetPControlledCreature 0x00487DC0,
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (CThing::GetTC<CTCHeroOpinionDeedLog> inlined, interface 0x51).
// Model: the landed CGameScriptInterface::SetHeroAbleToGainExperience 0x00899060 (same shape, other component). Layout:
//   CGameScriptInterface +0x14 PlayerManager; CThing retail UsingComponents +0x20, Components +0x44, Dead +0x91 bit 0;
//   CTCHeroOpinionDeedLog::GuardsIgnoreCrimes retail +0x2d (Ego_r.pdb CTCHeroOpinionDeedLog GuardsIgnoreCrimes +0x39; retail map 12 bytes and list 4 bytes put NextHandle +0x1c, VillageForCrimes +0x20, WeaponOutDeedHandle +0x28 (landed OnInitialActivate 0x006C5680 model), so this field is +0x2d).
// Setter: CTCHeroOpinionDeedLog::SetGuardsIgnoreCrimes(bool) (FableWin.pdb ?SetGuardsIgnoreCrimes@CTCHeroOpinionDeedLog@@QAEX_N@Z), inline.
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
    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CTCHeroOpinionDeedLog {
public:
    enum { INTERFACE_TYPE = 0x51 };
    unsigned char _pad00[0x2d];
    bool GuardsIgnoreCrimes;                                    // +0x2d
    void SetGuardsIgnoreCrimes(bool value) { GuardsIgnoreCrimes = value; }
};
class CGameScriptInterface {
public:
    virtual void SetGuardsIgnoreCrimes(bool value) const;
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
};

void CGameScriptInterface::SetGuardsIgnoreCrimes(bool value) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroOpinionDeedLog* tc;
    if (hero && !hero->IsDead() && hero->GetTC(&tc))
        tc->SetGuardsIgnoreCrimes(value);
}