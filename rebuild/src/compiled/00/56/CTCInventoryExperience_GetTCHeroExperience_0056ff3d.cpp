// CTCInventoryExperience::GetTCHeroExperience() -- the owner's CTCHeroExperience component (interface 0x68), not checked for presence
// Leaf: ?GetTCHeroExperience@CTCInventoryExperience@@AAEPAVCTCHeroExperience@@XZ
// Plan phase 3 (2026-11-04), CVectorMap wave (stand-in map): the local stand-in map class (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Also: the empty local std::bitset stand-in is the STLport bitset<274> (36 bytes) of stlport/fable_containers.h, test() declared only under FABLE_RETAIL_STLP_BITSET_TEST_DECL (retail calls 0x00410DE0); bitset pad _pad_0x21[0x23] removed. Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Naming wave 18: the interface-presence stand-in CPresenceBits is std::bitset<274> (STLport 4.6); its test is ?test@?$bitset@$0BBC@@std@@QBE_NI@Z (name-only).
// Retail: Fable.exe (TLC, VC7.1) @ 0x0056FF3D, 68 bytes, thiscall, plain `ret`, ebp frame.
// Identity (proven): Ego_r.pdb ?GetTCHeroExperience@CTCInventoryExperience@@AAEPAVCTCHeroExperience@@XZ (private); interface type 0x68 is CTCHeroExperience
//   (as the landed CTCInventoryExperience / inventory rows name it); callers are CTCInventoryExperience's ability / stat / level-up
//   code (0x00570332 .. 0x005724CD).
// Callees: std::bitset::test 0x00410DE0 (the presence bit), CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020.
// Shape (family M140, representative 0x0056FF3D): `CTCHeroExperience* tc; ParentThing->GetInterface(&tc, 0x68); return tc;` -- the
//   landed F88 GetInterface (0x0056FBED model, `optimize("y", off)` + `optimize("s", on)`): the out local, the lookup key and
//   the miss path's returned value share one frame word.
// Header debt: CThing / CTCInventoryExperience modelled locally (presence bits +0x20, interface map +0x44, ParentThing +4).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#include "engine/CVectorMap.h"
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
class CTCBase;
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CThing {
public:
    unsigned char _pad_0x00[0x20];
    std::bitset<274> m_present;                                // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    template<class T> __forceinline bool GetInterface(T** out, long wanted)
    {
        if (m_present.test(wanted)) {
            long type = wanted;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* found = map->LowerBound((const ETCInterfaceType&)type);
            CInterfaceMap::value_type* it;
            if (found != map->end() && !(wanted < found->first))
                it = found;
            else
                it = map->end();
            *out = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CTCHeroExperience;
class CTCInventoryExperience {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
private:
    CTCHeroExperience* GetTCHeroExperience();
};
CTCHeroExperience* CTCInventoryExperience::GetTCHeroExperience()
{
    CTCHeroExperience* tc;
    ParentThing->GetInterface(&tc, 0x68);
    return tc;
}