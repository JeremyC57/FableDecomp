// CAIStateGroup_BanditPlayDice::GetDiceGame() -- the dice-game component (interface 0xf0) of the village this creature's villager component belongs to, or null
// Leaf: ?GetDiceGame@CAIStateGroup_BanditPlayDice@@QAEPAVCTCDiceGame@@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00929220, length 135 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (two inlined GetInterface lookups:
// the creature's villager component, interface 0x23 = bitset word 1 bit 3 -> `test byte ptr [eax+24h],8`, and the
// village thing's dice game, interface 0xf0 = word 7 -> `test dword ptr [eax+3Ch],10000h`),
// GFGetPVillageInterface @ 0x007EE540 (__fastcall on the villager component, twice; the manifest's IS_THING_ALIVE
// label is propagated -- see the landed 0x0093EDF0).
// Shape: the landed GetInterface out-parameter template (`__forceinline`: two uses) inside nested ifs with one shared `return 0` tail
// (`xor eax,eax`); the villager component stays in esi for the second village call; the village's owner thing
// (+4) is read from the second call's result; the final lookup returns the component straight from the entry.
// Model: local (engine/CAIStateGroup_BanditPlayDice.h flattens the base, and the base layout is needed); the base fields keep their PDB names.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCVillage { public:
    void*   __vftable;                          // +0x00
    CThing* Owner;                              // +0x04 (CTCBase)
};
struct CTCVillager;
class CTCDiceGame;
class CTCVillageMember { public: CTCVillage* GetPVillage() const; };   // 0x007EE540 (Ego_r.pdb CTCVillageMember::GetPVillage, const)

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_BanditPlayDice : CAIStateGroupBase {
    CTCDiceGame* GetDiceGame();
};

CTCDiceGame* CAIStateGroup_BanditPlayDice::GetDiceGame()
{
    CTCVillager* villager;
    if (PBrain->Creature->GetTC(TCI_VILLAGE_MEMBER, &villager)) {
        if (((CTCVillageMember*)villager)->GetPVillage() != 0) {
            CThing* villageThing = ((CTCVillageMember*)villager)->GetPVillage()->Owner;
            CTCDiceGame* game;
            if (villageThing->GetTC(TCI_DICE_GAME, &game))
                return game;
        }
    }
    return 0;
}