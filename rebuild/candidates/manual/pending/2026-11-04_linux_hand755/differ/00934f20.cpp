// CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const -- of two creature groups pick the one whose nearest member is closer to the position (null / group-less sides lose)
// Grade: DIFFER(281v281) -- block layout: the last compare's return b is if-converted (mov eax,[b] ; jne ; mov eax,ebx) where retail jumps back to the earlier if (!nearestA) return b block
// Leaf: ??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00934F20, 279 bytes (+9 int3), `ret 8`. Identity: Ego_r.pdb / FableWin.pdb
//   ??$?RVCThing@@@CThingCompare_NearestGroupByMember@@QBEPAVCThing@@PAV1@0@Z (the comparator of
//   CThingSearchTools::GetPBestThing<CThingFilter_HasTC, CThingCompare_NearestGroupByMember, CThing>).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x004365B0 (CThing::GetTC<CTCCreatureGroup>, type 0xA3
//   TCI_CREATURE_GROUP), CTCCreatureGroup::GetNearestMember(const C3DVector&) const 0x008128F0 (Ego_r/FableWin
//   ?GetNearestMember@CTCCreatureGroup@@QBEPBVCThingAICreature@@ABVC3DVector@@@Z), CThing::GetPos() const 0x004C73D0.
// Layout: PDB CThingCompare_NearestGroupByMember { C3DVector Pos; } (12 bytes).
// Shape: null tests return the other side; the group component of `a` is fetched (end/hit pick) before `b`'s HasTC test;
//   retail asks a's group for its nearest member twice (b's group is never fetched -- kept as found); the closer member
//   wins by squared distance (first offset spilled to a frame local, the second on the x87 stack).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_C3DVECTOR
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"

class CThingAICreature;
class CTCCreatureGroup : public CTCBase {                          // no shared header
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_CREATURE_GROUP; }
    const CThingAICreature* GetNearestMember(const C3DVector& pos) const;   // 0x008128F0
};

class CThingCompare_NearestGroupByMember {
public:
    C3DVector Pos;                                                  // +0x00
    template <class T> T* operator()(T* a, T* b) const
    {
        if (!a)
            return b;
        if (!b)
            return a;
        if (!a->HasTC(TCI_CREATURE_GROUP))
            return b;
        const CTCCreatureGroup* groupA = static_cast<CTCCreatureGroup*>(&a->GetTC(TCI_CREATURE_GROUP));
        if (b->HasTC(TCI_CREATURE_GROUP)) {
            const CThing* nearestA = (const CThing*)groupA->GetNearestMember(Pos);
            const CThing* nearestB = (const CThing*)groupA->GetNearestMember(Pos);
            if (!nearestA)
                return b;
            if (nearestB) {
                C3DVector offsetA = nearestA->GetPos() - Pos;
                if (!(offsetA.GetSquaredMagnitude() < (nearestB->GetPos() - Pos).GetSquaredMagnitude()))
                    return b;
            }
        }
        return a;
    }
};
template CThing* CThingCompare_NearestGroupByMember::operator()<CThing>(CThing* a, CThing* b) const;
