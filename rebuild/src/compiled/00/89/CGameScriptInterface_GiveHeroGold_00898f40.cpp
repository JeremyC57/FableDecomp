// CGameScriptInterface::GiveHeroGold(long amount) const -- script call: adds money to the main player's hero (CTCHeroStats::AddMoney(amount, true)) when the hero is alive and holds the component
// Leaf: ?GiveHeroGold@CGameScriptInterface@@UBEXJ@Z
// Retail: Fable.exe @ 0x00898F40, 95 bytes, thiscall `ret 4`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 126 holds this body; slot 126 is
//   ?GiveHeroGold@CGameScriptInterface@@UBEXJ@Z in ghidra_out/cgsi_slots_aligned.tsv.
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); live hero holding
//   interface 4: GetTC (non-const LowerBound 0x0040F020), then CTCHeroStats::AddMoney(amount, true) (0x0057B327,
//   FableWin.pdb ?AddMoney@CTCHeroStats@@QAEXJ_N@Z; the manifest and reloc_bindings carry the same name).
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
    template <class T> __forceinline bool PeekTC(const T** out) const
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
    CThingPlayerCreature* GetPControlledCreature() const;    // 0x00487DC0
    const CThingPlayerCreature* PeekPControlledCreature() const;   // 0x00487DD0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CTCHeroStats {                                    // interface 4
public:
    enum { INTERFACE_TYPE = 4 };
    void AddMoney(long amount, bool flag);              // 0x0057B327
};
class CGameScriptInterface {
public:
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    virtual void GiveHeroGold(long amount) const;
};
void CGameScriptInterface::GiveHeroGold(long amount) const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    CTCHeroStats* stats;
    if (hero && !hero->IsDead() && hero->GetTC(&stats))
        stats->AddMoney(amount, true);
}