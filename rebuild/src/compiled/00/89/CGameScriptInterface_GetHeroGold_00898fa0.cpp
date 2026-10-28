// CGameScriptInterface::GetHeroGold() const -- script call: the main player's hero's money (CTCHeroStats::GetMoney), 0 without a live hero holding the component
// Leaf: ?GetHeroGold@CGameScriptInterface@@UBEJXZ
// Retail: Fable.exe @ 0x00898FA0, 86 bytes, thiscall `ret`.
// Identity (proven): retail CGameScriptInterface vtable 0x01260F0C slot 127 holds this body; slot 127 is
//   ?GetHeroGold@CGameScriptInterface@@UBEJXZ in ghidra_out/cgsi_slots_aligned.tsv (slot 126 GiveHeroGold = 0x00898F40).
//   Getter: FableWin.pdb ?GetMoney@CTCHeroStats@@QBEJXZ, inline (+0x3c, interface 4).
// Body: PlayerManager (+0x14)->GetMainPlayer() (0x00449970)->GetPControlledCreature() (0x00487DC0); live hero holding
//   interface 4: GetTC through the non-const LowerBound 0x0040F020 and return the money word +0x3c.
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
    unsigned char _pad00[0x3c];
    long Money;                                          // +0x3c
    long GetMoney() const { return Money; }
};
class CGameScriptInterface {
public:
    void*           World;                               // +0x04
    void*           Component;                           // +0x08
    void*           DisplayEngine;                       // +0x0c
    void*           DefinitionManager;                   // +0x10
    CPlayerManager* PlayerManager;                       // +0x14
    virtual long GetHeroGold() const;
};
long CGameScriptInterface::GetHeroGold() const
{
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (hero && !hero->IsDead())
    {
        CTCHeroStats* stats;
        if (hero->GetTC(&stats))
            return stats->GetMoney();
    }
    return 0;
}