// CTCSingleUserActionPoints::IsActionPointOccupied(CThing* user, C3DVector* position, CRightHandedSet* orientation, long index) -- place the index'th action-point dummy, then report whether any creature other than the owner and the user stands within 0.5 of it
// Leaf: IsActionPointOccupied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FEAF0, 190 bytes (`ret 10h`); CTCSingleUserActionPoints vtable 0x01241EAC
// slot 28 (the catalog's "vfunc_28"; with four arguments it is not CTCBase::OnActionUse -- named descriptively here).
// Callees: CTCGraphicAppearance::GetDummyObject(const CCharString&, long, unsigned long, C3DVector*, CRightHandedSet*, float)
// @ 0x004C08C0 on the owner's appearance (+0x64) with the dummy-name string global @ 0x013BAC9C, __ftol2 @ 0x00BFEA70,
// CTCBase::PeekWorld const @ 0x00686A80, CThingSearchTools::GetPFirstThingInRoughArea<CFilter_Bind3<CThingFilter_IsNot,
// CThingFilter_IsNot, CThingFilter_In2DRange>, CThingCreatureBase>(const C2DBoxI&, const filter&, CThingCreatureBase**)
// @ 0x007FE730 on the world's search tools (+0x20).
// Layout: Ego_r.pdb CThingFilter_IsNot {Thing}, CThingFilter_In2DRange {C3DVector Pos, float Range}, CFilter_Bind3 {F1, F2, F3}
// (pointers to the three filters), C2DBoxI {TLX, TLY, BRX, BRY}, C2DCoordI {X, Y}.
// Shape: the dummy position is truncated X then Y through a C2DCoordI constructor and used as both box corners; the two
// IsNot filters and the found-creature out pointer live in the dead argument slots, the range filter copies the position
// as dwords (the filter temporaries are built right to left); the search tools are fetched into a local inside an inline
// helper so they come after the temporaries and before the pushes; the search result is normalised to a bool.
class CThing;
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CCharString {
public:
    void* PStringData;
};
extern CCharString g_CCharString_013bac9c;              // 0x013BAC9C ("ACTION_POINT"; the landed initializer 0x0121E960 spelling)
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale);   // 0x004C08C0
};
class CThing {
public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* GraphicAppearance;            // +0x64
};
class C2DCoordI { public:
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& topLeft, const C2DCoordI& bottomRight)
        : TLX(topLeft.X), TLY(topLeft.Y), BRX(bottomRight.X), BRY(bottomRight.Y) { }
};
class CThingFilter_IsNot { public:
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
class CThingFilter_In2DRange { public:
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template <class F1, class F2, class F3> class CFilter_Bind3 { public:
    const F1* f1;
    const F2* f2;
    const F3* f3;
    CFilter_Bind3(const F1& a, const F2& b, const F3& c) : f1(&a), f2(&b), f3(&c) { }
};
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange> CActionPointFilter;
class CThingSearchTools { public:
    template <class TFilter, class T>
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** found);   // 0x007FE730
};
class CWorld { public:
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool ToBeRemoved;                                   // +0x08
    bool IsInGlobalUpdateTCs;                           // +0x09
    unsigned char _pad0a[2];
protected:
    const CWorld& PeekWorld() const;   // 0x00686A80
};
class CTCSingleUserActionPoints : public CTCBase { public:
    __forceinline bool FindFirstCreature(const C2DBoxI& area, const CActionPointFilter& filter, CThingCreatureBase** found) const
    {
        CThingSearchTools* searchTools = PeekWorld().ThingSearchTools;
        return searchTools->GetPFirstThingInRoughArea(area, filter, found);
    }
    bool IsActionPointOccupied(CThing* user, C3DVector* position, CRightHandedSet* orientation, long index);
};

bool CTCSingleUserActionPoints::IsActionPointOccupied(CThing* user, C3DVector* position, CRightHandedSet* orientation, long index)
{
    ParentThing->GraphicAppearance->GetDummyObject(g_CCharString_013bac9c, index, 0, position, orientation, 1.0f);
    C2DCoordI coord((long)position->X, (long)position->Y);
    C2DBoxI area(coord, coord);
    CThingCreatureBase* found;
    return FindFirstCreature(area,
        CActionPointFilter(CThingFilter_IsNot(ParentThing), CThingFilter_IsNot(user), CThingFilter_In2DRange(*position, 0.5f)),
        &found) ? true : false;
}