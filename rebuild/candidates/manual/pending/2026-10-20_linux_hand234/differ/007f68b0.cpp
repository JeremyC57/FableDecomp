// CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType type) const -- the nearest AI creature within the owner's extended sight box that is available for the interaction, aware of the owner and not incapacitated
// Grade: DIFFER(256v255) -- frame packing: the 0x34 lookup key shares the perception pointer's word (retail: the result word) and the box/bounds arithmetic schedules differently (lea vs add); declaration-order variants gave the same frame
// Leaf: GetPBestCreatureForInteractionType
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F68B0, 255 bytes, `ret 4`. Ego_r.pdb `?GetPBestCreatureForInteractionType@
//   CTCTargetingAI@@UBEPAVCThingCreatureBase@@W4ECreatureInteractionType@@@Z`.
// Callees: CThing::GetPosition() const @ 0x004C73D0, the const interface LowerBound @ 0x004365B0 (interface 0x34 =
//   CTCPerceiveThing), __ftol2 @ 0x00BFEA70 (x3), CTCPerceiveThing::GetExtendedSightRadius @ 0x007448F0,
//   CTCBase::GetThingSearchTools() const @ 0x00686D90, CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind3<
//   CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_IsAICreatureAwareOf, unary_negate<CThingFilter_IsIncapacitated> >,
//   CThingCompare_Nearest, CThingAICreature> @ 0x007F7340.
// Layout: retail CThing interface bitset +0x20 (0x34 = word +0x24 bit 20), map +0x44; Ego_r.pdb filters
//   IsAvailableForCreatureInteraction {SourceThing, Type}, IsAICreatureAwareOf {PCreature}, CThingCompare_Nearest {Pos}.
// Shape: the perception component from the out-parameter GetInterface (uninitialised on a miss); the truncated X, Y and range
//   are named locals; the box, the three predicates (the empty negated one in the dead `type` slot), the Bind3 and the
//   comparator are argument temporaries of the search call; the result comes back through the out pointer.
class CTCBase;
struct C3DVector { float X, Y, Z; };
struct C2DCoordI { long X, Y; };
struct C2DBoxI {
    C2DCoordI TopLeft;
    C2DCoordI BottomRight;
    C2DBoxI(long x0, long y0, long x1, long y1) { TopLeft.X = x0; TopLeft.Y = y0; BottomRight.X = x1; BottomRight.Y = y1; }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    const C3DVector& GetPosition() const;               // 0x004C73D0
    template <class T> __forceinline bool GetInterface(T** out, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            if (it == map->last || type < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing {};
class CThingAICreature : public CThingCreatureBase {};
enum ECreatureInteractionType { ECIT_DUMMY = 0 };
struct CTCPerceiveThing {
    float GetExtendedSightRadius();                     // 0x007448F0
};
struct CThingFilter_IsAvailableForCreatureInteraction {
    CThing* SourceThing;
    ECreatureInteractionType Type;
    CThingFilter_IsAvailableForCreatureInteraction(CThing* source, ECreatureInteractionType type) : SourceThing(source), Type(type) {}
};
struct CThingFilter_IsAICreatureAwareOf {
    CThing* PCreature;
    CThingFilter_IsAICreatureAwareOf(CThing* creature) : PCreature(creature) {}
};
struct CThingFilter_IsIncapacitated {
    CThingFilter_IsIncapacitated() {}
};
template <class P> struct unary_negate {
    P m_pred;
    unary_negate(const P& pred) : m_pred(pred) {}
};
template <class P> inline unary_negate<P> not1(const P& pred) { return unary_negate<P>(pred); }
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
struct CThingCompare_Nearest {
    C3DVector Pos;
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) {}
};
typedef CFilter_Bind3<CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_IsAICreatureAwareOf,
                      unary_negate<CThingFilter_IsIncapacitated> > CInteractionFilter;
struct CThingSearchTools {
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const CInteractionFilter& filter, const CThingCompare_Nearest& compare,
                                  CThingAICreature** out);   // 0x007F7340
};
struct CTCTargetingAI {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CThingSearchTools* GetThingSearchTools() const;     // 0x00686D90 (CTCBase)
    CThingCreatureBase* GetPBestCreatureForInteractionType(ECreatureInteractionType type) const;
};

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionType(ECreatureInteractionType type) const
{
    CThing* parent = ParentThing;
    const C3DVector& position = parent->GetPosition();
    CTCPerceiveThing* perceive;
    parent->GetInterface(&perceive, 0x34);
    CThingAICreature* best = 0;
    long x = (long)position.X;
    long y = (long)position.Y;
    long range = (long)perceive->GetExtendedSightRadius();
    C2DBoxI area(x - range, y - range, x + range, y + range);
    GetThingSearchTools()->GetPBestThingInRoughArea(area,
        CInteractionFilter(CThingFilter_IsAvailableForCreatureInteraction(parent, type), CThingFilter_IsAICreatureAwareOf(parent),
                           not1(CThingFilter_IsIncapacitated())),
        CThingCompare_Nearest(position), &best);
    return best;
}
