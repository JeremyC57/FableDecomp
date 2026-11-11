// CWorld::EADeleteMenuComponents(const CGameEvent* event) -- game-event action: if the event player's hero has the CTCInventory interface (TC 0x11), call its DeleteMenuComponents()
// Leaf: ?EADeleteMenuComponents@CWorld@@QAEXPBVCGameEvent@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0062C980, 78 bytes (+ int3 padding to 80), thiscall `ret 4`.
// Identity (proven): CWorld::EADeleteMenuComponents; evidence: its only reference is a store into the game-event handler table by the static initialiser 0x0121B9B0 (no direct calls); it calls 0x00587A41 on the hero's TC 0x11 component -- the CTCInventory member that every PC menu opener calls first (0x0058AF10 options, 0x0058F9BC photo journal, 0x00589A30 map, 0x0058807D inventory and five more) and that closes the open scoreboard / map / in-game / inventory menus: Ego_r.pdb CTCInventory::DeleteMenuComponents, whose event action is ?EADeleteMenuComponents@CWorld@@QAEXPBVCGameEvent@@@Z
// Label: the manifest's "sub_0062c980" is a gap-scan placeholder (family H087: six one-call CWorld event actions, each its own
//   function, registered by address in the game-event handler table that the static initialiser 0x0121B9B0 fills).
// Callees: CWorld::GetPlayer(long) 0x0049D850, CPlayer::GetPControlledCreature 0x00487DC0, the interface-map LowerBound 0x0040F020,
//   CTCInventory::DeleteMenuComponents 0x00587A41.
// Layout (retail CThing): interface bitset +0x20 (TC 0x11 -> word 0 mask 0x20000), interface map +0x44 (the landed
//   CWorld::EARemoveCurrentMode 0x0062C4B0 model). Local classes: the generated engine headers declare CGameEvent as a
//   `struct`, but the PDB symbol mangles it `V` (class) -- Review at landing.
// Shape: no null test on the hero; the GetTC key lives in the dead `event` argument slot.
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThingPlayerCreature : public CThing { };
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
    CThingPlayerCreature* GetPControlledCreature() const;                        // 0x00487DC0
};
class CWorld {
public:
protected:
    CPlayer* GetPlayer(unsigned long number);               // 0x0049D850 ?GetPlayer@CWorld@@IAEPAVCPlayer@@K@Z (Ego_r/FableWin, protected)
public:
    void EADeleteMenuComponents(const CGameEvent* event);
};

void CWorld::EADeleteMenuComponents(const CGameEvent* event)
{
    CThing* hero = GetPlayer(event->Player)->GetPControlledCreature();
    if (hero->HasTC(TCI_INVENTORY))
        ((CTCInventory*)&hero->GetTC(TCI_INVENTORY))->DeleteMenuComponents();
}