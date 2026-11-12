// CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType type) const -- the nearest AI creature within the owner's extended sight box that is available for the interaction, aware of the owner and not incapacitated
// Grade: DIFFER(256v255) -- regraded as-is (1 grade); still uses a stand-in GetPBestThingInRoughArea body (FindBestThingInRoughArea_007f7340): not landable until the real template body is shared; residue BRX lea vs add (0x007F6929).
// Leaf: ?GetPBestCreatureForInteractionType@CTCTargetingAI@@UBEPAVCThingCreatureBase@@W4ECreatureInteractionType@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F68B0, 255 bytes, `ret 4`.
// Callees: CThing::GetPos() const @ 0x004C73D0, the const component-map lower bound @ 0x004365B0 (PeekTC<const
//   CTCPerceiveThing>, TCI_PERCEIVE_THING 52), __ftol2 @ 0x00BFEA70 (x3), CTCPerceiveThing::GetExtendedSightRadius() const
//   @ 0x007448F0, CTCBase::PeekThingSearchTools() const @ 0x00686D90, CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<
//   CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_IsAICreatureAwareOf, unary_negate<CThingFilter_IsIncapacitated> >,
//   CThingCompare_Nearest, CThingAICreature> const @ 0x007F7340.
// Layout: retail CThing / CTCBase headers; Ego_r.pdb filters IsAvailableForCreatureInteraction {SourceThing, Type},
//   IsAICreatureAwareOf {PCreature}, CThingCompare_Nearest {Pos}; shared C2DBoxI(centre, radius).
// Shape: the perception component from PeekTC (uninitialised on a miss); the box, the predicates and the comparator are
//   argument temporaries of the search call; the result comes back through the out pointer.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
class CThingCreatureBase;
class CThingAICreature;
enum ECreatureInteractionType { ECIT_DUMMY = 0 };
class CTCPerceiveThing : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_PERCEIVE_THING; }
    float GetExtendedSightRadius() const;               // 0x007448F0
};
class CThingFilter_IsAvailableForCreatureInteraction { public:
    const CThing* SourceThing;
    ECreatureInteractionType Type;
    CThingFilter_IsAvailableForCreatureInteraction(const CThing* source, ECreatureInteractionType type) : SourceThing(source), Type(type) {}
};
class CThingFilter_IsAICreatureAwareOf { public:
    const CThing* PCreature;
    CThingFilter_IsAICreatureAwareOf(const CThing* creature) : PCreature(creature) {}
};
class CThingFilter_IsIncapacitated { public:
    CThingFilter_IsIncapacitated() {}
};
namespace std {
template <class P> class unary_negate { public:
    P m_pred;
    explicit unary_negate(const P& pred) : m_pred(pred) {}
};
template <class P> inline unary_negate<P> not1(const P& pred) { return unary_negate<P>(pred); }
}
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
class CThingCompare_Nearest { public:
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
typedef CFilter_Bind3<CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_IsAICreatureAwareOf,
                      std::unary_negate<CThingFilter_IsIncapacitated> > CInteractionFilter;
class CThingSearchTools { public:
    template <class Filter, class Compare, class T>
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const Filter& filter, const Compare& compare, T** out) const;   // 0x007F7340
};
void* FindBestThingInRoughArea_007f7340(const CThingSearchTools* tools, const C2DBoxI* area, const void* filter, const void* compare);
#pragma auto_inline(off)
template <class Filter, class Compare, class T>
__declspec(noinline) bool CThingSearchTools::GetPBestThingInRoughArea(const C2DBoxI& area, const Filter& filter, const Compare& compare, T** out) const
{
    T* found = (T*)FindBestThingInRoughArea_007f7340(this, &area, &filter, &compare);
    *out = found;
    return found != 0;
}
#pragma auto_inline(on)
class CTCTargetingAI : public CTCBase {
public:
    virtual CThingCreatureBase* GetPBestCreatureForInteractionType(ECreatureInteractionType type) const;
};

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType type) const
{
    const CThing& parent = GetParentThing();
    const C3DVector& position = parent.GetPos();
    const CTCPerceiveThing* perceive;
    parent.PeekTC(&perceive);
    CThingAICreature* best = 0;
    C2DBoxI area(C2DCoordI((long)position.X, (long)position.Y), (long)perceive->GetExtendedSightRadius());
    PeekThingSearchTools().GetPBestThingInRoughArea(area,
        CInteractionFilter(CThingFilter_IsAvailableForCreatureInteraction(&parent, type), CThingFilter_IsAICreatureAwareOf(&parent),
                           std::not1(CThingFilter_IsIncapacitated())),
        CThingCompare_Nearest(position), &best);
    return (CThingCreatureBase*)best;
}
