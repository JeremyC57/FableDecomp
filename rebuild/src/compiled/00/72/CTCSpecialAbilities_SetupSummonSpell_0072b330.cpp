// CTCSpecialAbilities::SetupSummonSpell() -- reset the creature's summon-spell minions if it already has a CTCSummonSpell component, else add one; always true
// Leaf: ?SetupSummonSpell@CTCSpecialAbilities@@AAE_NXZ
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe @ 0x0072B330, 118 bytes (`push ecx` frame = the LowerBound key word). Ego_r/FableWin: private
//   ?SetupSummonSpell@CTCSpecialAbilities@@AAE_NXZ. Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020,
//   CTCSummonSpell::ResetMinionPos 0x0079B6F0 (PDB QAEXXZ), CCharString(const char*, long) 0x0099EBF0 ("CTCSummonSpell"
//   0x0123C378), CThing::AddTC 0x004C9D60, ~CCharString 0x0099EAE0. Interface 0x91 = word 4 bit 17 of CThing+0x20 (`test
//   dword ptr [eax+30h],20000h`).
// Shape: the landed out-parameter GetInterface(T** ppOut, long type) template (0x007FFC70 / 0x008337B0 family), written
//   `if (!GetInterface(&spell, 0x91)) { add; return true; } spell->ResetMinionPos(); return true;` -- the negated test is
//   what leaves the component in eax (`mov eax,[eax+4] ; mov ecx,eax`); the positive `if (GetInterface) {call; return}`
//   form (ChatGPT's selected source and the 0x0072B0D0 sibling's shape) loads it straight into ecx (116v118).
// Layout: the landed sibling CTCSpecialAbilities::SetupPhysicalShieldSpell 0x0072B0D0's model (retail STLport offsets:
//   UndeadFollowers list 4 bytes at +0x14, Creature +0x18).
class CTCBase;
class CTCParamsBase;
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCSummonSpell { public:
    void ResetMinionPos();                      // 0x0079B6F0
};
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    unsigned char _base08[4];
    bool AbilityMutexLock;                      // +0x0c
    bool UsingBed;                              // +0x0d
    long CurrentAbility;                        // +0x10
    void* UndeadFollowers;                      // +0x14 list sentinel
    CThing* Creature;                           // +0x18 (PDB +0x20)
private:
    bool SetupSummonSpell();
};

bool CTCSpecialAbilities::SetupSummonSpell()
{
    CTCSummonSpell* spell;
    if (!Creature->GetTC(TCI_SUMMON_SPELL, &spell)) {
        CCharString name("CTCSummonSpell");
        Creature->AddTC(name, true, 0);
        return true;
    }
    spell->ResetMinionPos();
    return true;
}