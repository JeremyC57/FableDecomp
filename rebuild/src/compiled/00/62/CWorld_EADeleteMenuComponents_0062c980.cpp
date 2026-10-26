// CWorld::EADeleteMenuComponents(const CGameEvent* event) -- game-event action: if the event player's hero has the CTCInventory interface (TC 0x11), call its DeleteMenuComponents()
// Leaf: ?EADeleteMenuComponents@CWorld@@QAEXPBVCGameEvent@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062C980, 78 bytes (+ int3 padding to 80), thiscall `ret 4`.
// Identity (proven): CWorld::EADeleteMenuComponents; evidence: its only reference is a store into the game-event handler table by the static initialiser 0x0121B9B0 (no direct calls); it calls 0x00587A41 on the hero's TC 0x11 component -- the CTCInventory member that every PC menu opener calls first (0x0058AF10 options, 0x0058F9BC photo journal, 0x00589A30 map, 0x0058807D inventory and five more) and that closes the open scoreboard / map / in-game / inventory menus: Ego_r.pdb CTCInventory::DeleteMenuComponents, whose event action is ?EADeleteMenuComponents@CWorld@@QAEXPBVCGameEvent@@@Z
// Label: the manifest's "sub_0062c980" is a gap-scan placeholder (family H087: six one-call CWorld event actions, each its own
//   function, registered by address in the game-event handler table that the static initialiser 0x0121B9B0 fills).
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetHeroThing 0x00487DC0, the interface-map LowerBound 0x0040F020,
//   CTCInventory::DeleteMenuComponents 0x00587A41.
// Layout (retail CThing): interface bitset +0x20 (TC 0x11 -> word 0 mask 0x20000), interface map +0x44 (the landed
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
class CTCInventory {                                  // TC 0x11
public:
    void DeleteMenuComponents();                             // 0x00587A41
};
class CPlayer {
public:
    CThing* GetHeroThing();                        // 0x00487DC0
};
class CWorld {
public:
    CPlayer* GetPlayer(long number);               // 0x0049D850
    void EADeleteMenuComponents(const CGameEvent* event);
};

void CWorld::EADeleteMenuComponents(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetHeroThing();
    if (hero->HasTC(0x11))
        hero->GetTC<CTCInventory >(0x11)->DeleteMenuComponents();
}