// CTCInventoryExperience::GetTCInventoryAbilities() -- the owner's CTCInventoryAbilities component (interface 0x6F), not checked for presence
// Leaf: ?GetTCInventoryAbilities@CTCInventoryExperience@@AAEPAVCTCInventoryAbilities@@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FFC5, 68 bytes, thiscall, plain `ret`, ebp frame.
// Identity (proven): Ego_r.pdb ?GetTCInventoryAbilities@CTCInventoryExperience@@AAEPAVCTCInventoryAbilities@@XZ (private); interface type 0x6F is CTCInventoryAbilities
//   (as the landed CTCInventoryExperience / inventory rows name it); callers are CTCInventoryExperience's ability / stat / level-up
//   code (0x00570332 .. 0x005724CD).
// Callees: std::bitset::test 0x00410DE0 (the presence bit), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020.
// Shape (family M140, representative 0x0056FF3D): `CTCInventoryAbilities* tc; ParentThing->GetInterface(&tc, 0x6F); return tc;` -- the
//   landed F88 GetInterface (0x0056FBED model, `optimize("y", off)` + `optimize("s", on)`): the out local, the lookup key and
//   the miss path's returned value share one frame word.
// Header debt: CThing / CTCInventoryExperience modelled locally (presence bits +0x20, interface map +0x44, ParentThing +4).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
class CTCBase;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCInventoryAbilities;
class CTCInventoryExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
private:
    CTCInventoryAbilities* GetTCInventoryAbilities();
};
CTCInventoryAbilities* CTCInventoryExperience::GetTCInventoryAbilities()
{
    CTCInventoryAbilities* tc;
    ParentThing->GetTC(TCI_HERO_ABILITIES, &tc);
    return tc;
}