// CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit(long type, float minRange, float maxRange) -- the nearest conscious creature in range that the AI thing can see and is available for the given interaction
// Grade: DIFFER(236v235) -- with a memberwise C3DVector copy ctor only the box arithmetic differs: retail adds the range to the X register in place (`add ebp,eax`), the lane keeps X live and forms X + range with `lea` (and schedules the found-pointer zero store one call earlier) Also tried here: the C2DBoxI(const C2DCoordI&, long) constructor and a temporary C2DCoordI with an inline (long)maxRange (same bytes).
// Leaf: GetPBestCreatureForInteractionTypeWithinRangeLimit
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F67C0, 233 bytes (`ret 0Ch`).
// Callees: CThing::GetPosition const @ 0x004C73D0, __ftol2 @ 0x00BFEA70 (x, y, range), CTCBase::GetThingSearchTools @ 0x00686D90,
// CThingSearchTools::GetPBestThingInRoughArea<CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction,
// CThingFilter_In2DRangeLimit, CThingFilter_CanSeeEachOther, unary_negate<CThingFilter_IsUnconscious> >, CThingCompare_Nearest,
// CThingCreatureBase>(const C2DBoxI&, const filter&, const CThingCompare_Nearest&, CThingCreatureBase**) @ 0x007F70D0 (its body
// calls the four filters' operator() 0x006A7430 / 0x006A7700 / 0x006A78D0 / 0x006A73E0 (negated) and CThingCompare_Nearest
// 0x0065DA97; the manifest's template label names another filter set).
// Layout: Ego_r.pdb filters: IsAvailableForCreatureInteraction {SourceThing, Type}, In2DRangeLimit {Pos, MinRange, MaxRange},
// CanSeeEachOther {Creature}, IsUnconscious (empty), CThingCompare_Nearest {Pos}, CFilter_Bind4 {F1..F4}; C2DBoxI {TLX, TLY,
// BRX, BRY}.
// Shape: the position is truncated X then Y through a C2DCoordI constructor and the box grown by the truncated range; the
// filter and comparator temporaries are built in the call (the empty negated filter homed in the dead maxRange slot).
class CThing;
class CThingAICreature;
class CThingCreatureBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
class CThing {
public:
    const C3DVector& GetPosition() const;               // 0x004C73D0
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct CThingFilter_IsAvailableForCreatureInteraction {
    CThingAICreature* SourceThing;                      // +0x00
    long Type;                                          // +0x04 ECreatureInteractionType
    CThingFilter_IsAvailableForCreatureInteraction(CThingAICreature* source, long type) : SourceThing(source), Type(type) { }
};
struct CThingFilter_In2DRangeLimit {
    C3DVector Pos;                                      // +0x00
    float MinRange;                                     // +0x0c
    float MaxRange;                                     // +0x10
    CThingFilter_In2DRangeLimit(const C3DVector& pos, float minRange, float maxRange) : Pos(pos), MinRange(minRange), MaxRange(maxRange) { }
};
struct CThingFilter_CanSeeEachOther {
    CThingCreatureBase* Creature;                       // +0x00
    CThingFilter_CanSeeEachOther(CThingCreatureBase* creature) : Creature(creature) { }
};
struct CThingFilter_IsUnconscious {
    CThingFilter_IsUnconscious() { }
};
template <class T> struct unary_negate {
    T Pred;
    explicit unary_negate(const T& pred) : Pred(pred) { }
};
template <class T> inline unary_negate<T> not1(const T& pred) { return unary_negate<T>(pred); }
template <class F1, class F2, class F3, class F4> struct CFilter_Bind4 {
    const F1* f1;
    const F2* f2;
    const F3* f3;
    const F4* f4;
    CFilter_Bind4(const F1& a, const F2& b, const F3& c, const F4& d) : f1(&a), f2(&b), f3(&c), f4(&d) { }
};
typedef CFilter_Bind4<CThingFilter_IsAvailableForCreatureInteraction, CThingFilter_In2DRangeLimit, CThingFilter_CanSeeEachOther,
                      unary_negate<CThingFilter_IsUnconscious> > CInteractionFilter;
struct CThingCompare_Nearest {
    C3DVector Pos;                                      // +0x00
    CThingCompare_Nearest(const C3DVector& pos) : Pos(pos) { }
};
struct CThingSearchTools {
    template <class TFilter, class TCompare, class T>
    bool GetPBestThingInRoughArea(const C2DBoxI& area, const TFilter& filter, const TCompare& compare, T** best);   // 0x007F70D0
};
struct CTCTargetingAI {
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    CThingSearchTools* GetThingSearchTools();           // 0x00686D90 (CTCBase)
    CThingCreatureBase* GetPBestCreatureForInteractionTypeWithinRangeLimit(long type, float minRange, float maxRange);
};

CThingCreatureBase* CTCTargetingAI::GetPBestCreatureForInteractionTypeWithinRangeLimit(long type, float minRange, float maxRange)
{
    CThing* thing = ParentThing;
    const C3DVector& pos = thing->GetPosition();
    CThingCreatureBase* best = 0;
    C2DCoordI centre((long)pos.X, (long)pos.Y);
    long range = (long)maxRange;
    C2DBoxI area(centre.X - range, centre.Y - range, centre.X + range, centre.Y + range);
    GetThingSearchTools()->GetPBestThingInRoughArea(area,
        CInteractionFilter(CThingFilter_IsAvailableForCreatureInteraction((CThingAICreature*)thing, type),
                           CThingFilter_In2DRangeLimit(pos, minRange, maxRange),
                           CThingFilter_CanSeeEachOther((CThingCreatureBase*)thing),
                           not1(CThingFilter_IsUnconscious())),
        CThingCompare_Nearest(pos), &best);
    return best;
}
