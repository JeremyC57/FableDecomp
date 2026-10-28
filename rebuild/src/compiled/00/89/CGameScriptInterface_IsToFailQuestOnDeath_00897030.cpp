// CGameScriptInterface::IsToFailQuestOnDeath() const -- script call: whether the hero's death fails the current quest (CTCHeroStats), true without a live hero holding the component
// Leaf: ?IsToFailQuestOnDeath@CGameScriptInterface@@UBE_NXZ
// Retail: Fable.exe @ 0x00897030, 89 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 316 holds this body; slot 316 is
//   ?IsToFailQuestOnDeath@CGameScriptInterface@@UBE_NXZ in ghidra_out/cgsi_slots_aligned.tsv, next to the landed
//   SetWhetherToFailQuestOnDeath 0x00897090 (slot 317). Getter: FableWin.pdb ?IsToFailQuestOnDeath@CTCHeroStats@@QBE_NXZ.
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->PeekPControlledCreature() (0x00487DD0); live hero holding
//   interface 4: PeekTC (const LowerBound 0x004365B0) and return the FailQuestOnDeath byte +0x174 (as landed 0x00897090);
//   otherwise true.
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
    unsigned char _pad00[0x174];
    bool FailQuestOnDeath;                               // +0x174
    bool IsToFailQuestOnDeath() const { return FailQuestOnDeath; }
};
class CGameScriptInterface {
public:
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    virtual bool IsToFailQuestOnDeath() const;
};
bool CGameScriptInterface::IsToFailQuestOnDeath() const
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (hero && !hero->IsDead())
    {
        const CTCHeroStats* stats;
        if (hero->PeekTC(&stats))
            return stats->IsToFailQuestOnDeath();
    }
    return true;
}