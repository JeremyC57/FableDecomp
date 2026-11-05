// CTCInventoryWeapons::RefreshSheathedWeapons() -- looks up the owner's carrier component (interface 0x29) and either puts the weapons in their sheathes or takes them out, following its sheathed flag
// Leaf: ?RefreshSheathedWeapons@CTCInventoryWeapons@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed; map pointer made const (the row grades the const LowerBound).
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C9962, 92 bytes.
// Callees: std::bitset::test @ 0x00410DE0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0,
//          CTCInventoryWeapons::PutWeaponsInSheathes @ 0x005C8101, the opposite weapon-sheathing update @ 0x005C552C
//          (manifest label "CTCCarryable_PrepareWeaponSheathing"; name here descriptive).
//
// Shape: the inline out-parameter GetInterface (landed 0x007BEA08 family) whose result is IGNORED: the component local
// starts at 0 (`xor ebx,ebx` scheduled before the bitset call), is only assigned on a hit, and is dereferenced
// unconditionally (`cmp byte ptr [ebx+0Ch],0`). Frame pointer kept in a size TU (`#pragma optimize("y", off)` +
// `optimize("s", on)`); `this` lives in the frame slot [ebp-4] across the lookup and both tails are `call ; leave ;
// ret` (no sibling jump with a frame). The helper must be `__forceinline` under "s". +4 is CTCBase::ParentThing; the
// component's flag at +0x0C is named from use. Local model.
#include "engine/CTCBase.h"
#pragma optimize("y", off)
#pragma optimize("s", on)
class CTCBase;

#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCCarrier {                           // interface 0x29
    unsigned char _pad_0x00[0xc];
    bool m_weaponsSheathed;                   // +0x0C
};

class CTCInventoryWeapons : public CTCBase { public:
    void PutWeaponsInSheathes();              // 0x005C8101
    void TakeWeaponsOutOfSheathes();          // 0x005C552C
    void RefreshSheathedWeapons();
};

void CTCInventoryWeapons::RefreshSheathedWeapons()
{
    const CTCCarrier* carrier = 0;
    ParentThing->PeekTC(TCI_HERO, &carrier);
    if (carrier->m_weaponsSheathed)
        PutWeaponsInSheathes();
    else
        TakeWeaponsOutOfSheathes();
}