// CWorld::EADeleteTradeActiveMenu(const CGameEvent* event) -- game-event action: if the event player's hero has the CTCInventoryTrade interface (TC 0x17), call its DeleteComponents()
// Leaf: ?EADeleteTradeActiveMenu@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062C9D0, 78 bytes (+ int3 padding to 80), thiscall `ret 4`.
// Identity (proven): CWorld::EADeleteTradeActiveMenu; evidence: its only reference is a store into the game-event handler table by the static initialiser 0x0121B9B0 (no direct calls); it calls 0x0059C8AE on the hero's TC 0x17 component -- the CTCInventoryTrade member that the buy / sell / wanted openers (0x005AE972, 0x005AC0EE, 0x005AC0FE) and CloseInventoryPC 0x0059E2C7 call before rebuilding: Ego_r.pdb CTCInventoryTrade::DeleteComponents, the class's only Delete* member; the PDB's trade-menu teardown event action is ?EADeleteTradeActiveMenu@CWorld@@QAEXPBVCGameEvent@@@Z
// Label: the manifest's "sub_0062c9d0" is a gap-scan placeholder (family H087: six one-call CWorld event actions, each its own
//   function, registered by address in the game-event handler table that the static initialiser 0x0121B9B0 fills).
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetHeroThing 0x00487DC0, the interface-map LowerBound 0x0040F020,
//   CTCInventoryTrade::DeleteComponents 0x0059C8AE.
// Layout (retail CThing): interface bitset +0x20 (TC 0x17 -> word 0 mask 0x800000), interface map +0x44 (the landed
//   CWorld::EARemoveCurrentMode 0x0062C4B0 model). Local classes: the generated engine headers declare CGameEvent as a
//   `struct`, but the PDB symbol mangles it `V` (class) -- Review at landing.
// Shape: no null test on the hero; the GetTC key lives in the dead `event` argument slot.
class CTCBase;
class CInterfaceMap {
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;                                 // +0x00
    SEntry* last;                                  // +0x04
    SEntry* LowerBound(const long* type);          // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];              // +0x20 bitset<274>
    CInterfaceMap Components;                      // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
};
class CGameEvent {                                 // 0x28 bytes (Ego_r.pdb)
public:
    long          Type;                            // +0x00
    signed char   Player;                          // +0x04
    unsigned char Data[32];                        // +0x05
    unsigned char EndPos;                          // +0x25
    bool          Valid;                           // +0x26
    bool          Replacement;                     // +0x27
};
class CTCInventoryTrade {                                  // TC 0x17
public:
    void DeleteComponents();                             // 0x0059C8AE
};
class CPlayer {
public:
    CThing* GetHeroThing();                        // 0x00487DC0
};
class CWorld {
public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EADeleteTradeActiveMenu(const CGameEvent* event);
};

void CWorld::EADeleteTradeActiveMenu(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero->HasTC(0x17))
        hero->GetTC<CTCInventoryTrade >(0x17)->DeleteComponents();
}