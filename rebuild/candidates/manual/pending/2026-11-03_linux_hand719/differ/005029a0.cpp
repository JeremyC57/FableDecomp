// GetRelevantBuilding(map<CThingBuilding*, list<CThingAICreature*> >&, ESimBuildingProperty, bool, bool) -- among the village's buildings whose sim definition has the property (and, for work / live places, a free work / living space), the one with the fewest assigned creatures
// Grade: DIFFER(334v337) -- register colouring only: retail keeps the map reference in edi (spilled once) and the def in esi; VC7.1 here keeps the map in the frame and the def in edx (16 probes: cached end, spaces local, if/else-if arms, same-TU STLport map::operator[] body halved the residue 67->41 lines; the proposed same-TU noinline PeekPSimDefinition 0x0082D2D0 stand-in moved it the wrong way (315 bytes, 63 lines: that body lives in CThingBuilding's TU at 0x0082D2D0, so retail could not see it either), a building local and a while loop with a full flag were worse)
// Leaf: ?GetRelevantBuilding@@YIPAVCThingBuilding@@AAV?$map@PAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@U?$less@PAVCThingBuilding@@@3@V?$allocator@U?$pair@QAVCThingBuilding@@V?$list@PAVCThingAICreature@@V?$allocator@PAVCThingAICreature@@@std@@@std@@@std@@@3@@std@@W4ESimBuildingProperty@@_N2@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005029A0, 337 bytes, __fastcall (map in ecx, property in edx), `ret 8`.
// Identity: Ego_r.pdb / FableWin.pdb ?GetRelevantBuilding@@YIPAVCThingBuilding@@AAV?$map@...@std@@W4ESimBuildingProperty@@_N2@Z
//   (Ego_r 0x004D68D0, a free __fastcall of the CWorldMap TU between CacheMaps and AddToRelevantBuilding); its only caller is
//   CWorldMap::PopulateRegionWithVillagers 0x00502E90 (six calls, map in ecx). The catalog's "CBuilding_FindMostSpaceForNPCs"
//   is descriptive.
// Callees: CThingBuilding::PeekPSimDefinition() const 0x0082D2D0 (Ego_r ?PeekPSimDefinition@CThingBuilding@@QBE?AV?$CDefPointer@
//   $$CBVCSimBuildingDef@@@@XZ, hidden result slot), map<CThingBuilding*, list<CThingAICreature*> >::operator[] 0x0051BF90
//   (out of line, bound spelling), the def's Delete (CIVCountedPointeeBase slot 1) on the last release.
// Layout: CSimBuildingDef : CParentDefClassBase, Ego_r members Property +0x40 / WorkSpaces +0x44 / LiveSpaces +0x48 at retail
//   +0x3C / +0x40 / +0x44 (Ego_r - 4, the Dinkumware vector in CParentDefClassBase). STLport map node: key +0x10, list +0x14.
// Shape: the def lives in the loop body (released with the Release() == 0 form and the Object = 0 store); the two capacity
//   tests share the list-size loop (if / else if with a merged tail); `!best ||` short-circuits into the two operator[] sizes.
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_map.h"
class C3DVector;
#define CThingBuilding CThingBuildingDonor
#include "engine/CThingBuilding.h"
#undef CThingBuilding

class CSimBuildingDef : public CParentDefClassBase {               // Ego_r.pdb members at retail offsets (Ego_r - 4)
public:
    long Property;                                                  // +0x3c (ESimBuildingProperty flags)
    long WorkSpaces;                                                // +0x40
    long LiveSpaces;                                                // +0x44
};
class CThingBuilding : public CThingBuildingDonor {
public:
    CDefPointer<const CSimBuildingDef> PeekPSimDefinition() const;  // 0x0082D2D0
};
class CThingAICreature;
// STLport 4.6 _map.h map::operator[] (retail's out-of-line COMDAT 0x0051BF90 for this map), same-TU body under auto_inline(off).
namespace std {
#pragma auto_inline(off)
template <class K, class V, class C, class A> __declspec(noinline) V& map<K, V, C, A>::operator[](const K& __k)
{
    iterator __i = lower_bound(__k);
    if (__i == end() || key_comp()(__k, (*__i).first))
        __i = insert(__i, value_type(__k, V()));
    return (*__i).second;
}
#pragma auto_inline(on)
}
enum ESimBuildingProperty { };                                      // flag values unused here

CThingBuilding* __fastcall GetRelevantBuilding(
    std::map<CThingBuilding*, std::list<CThingAICreature*> >& buildings, ESimBuildingProperty property, bool work, bool live)
{
    CThingBuilding* best = 0;
    std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator end = buildings.end();
    for (std::map<CThingBuilding*, std::list<CThingAICreature*> >::iterator it = buildings.begin(); it != end; ++it)
    {
        CDefPointer<const CSimBuildingDef> def = it->first->PeekPSimDefinition();
        if (!def.IsNull() && (def->Property & property))
        {
            if (work || live)
            {
                unsigned long spaces = work ? def->WorkSpaces : def->LiveSpaces;
                if (it->second.size() >= spaces)
                    continue;
            }
            if (!best || buildings[it->first].size() < buildings[best].size())
                best = it->first;
        }
    }
    return best;
}
