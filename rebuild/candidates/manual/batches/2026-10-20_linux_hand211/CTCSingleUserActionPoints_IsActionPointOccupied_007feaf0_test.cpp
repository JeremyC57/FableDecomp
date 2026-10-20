// Behaviour test for CTCSingleUserActionPoints::IsActionPointOccupied (0x007FEAF0): the dummy is placed (index, scale 1),
// the search box is the truncated dummy XY as both corners, the filter excludes the owner and the user within 0.5 of the
// dummy, and the result follows the search.
#include <cstdio>
#ifndef _MSC_VER
#define __forceinline inline
#endif
class CThing;
struct CThingCreatureBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet;
class CCharString {
public:
    void* PStringData;
};
CCharString g_CCharString_013bac9c;
struct CTCGraphicAppearance {
    const CCharString* lastName; long lastIndex; CRightHandedSet* lastOrientation; float lastScale;
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale)
    {
        lastName = &name; lastIndex = index; lastOrientation = orientation; lastScale = scale;
        position->X = 10.7f + index; position->Y = -3.2f; position->Z = 1.0f;
        return true;
    }
};
class CThing {
public:
    unsigned char _pad00[0x64];
    CTCGraphicAppearance* GraphicAppearance;            // +0x64
};
struct C2DCoordI {
    long X, Y;
    C2DCoordI(long x, long y) : X(x), Y(y) { }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DCoordI& topLeft, const C2DCoordI& bottomRight)
        : TLX(topLeft.X), TLY(topLeft.Y), BRX(bottomRight.X), BRY(bottomRight.Y) { }
};
struct CThingFilter_IsNot {
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template <class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* f1;
    const F2* f2;
    const F3* f3;
    CFilter_Bind3(const F1& a, const F2& b, const F3& c) : f1(&a), f2(&b), f3(&c) { }
};
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange> CActionPointFilter;
struct CThingSearchTools {
    C2DBoxI lastArea; CThing* notA; CThing* notB; C3DVector rangePos; float range; bool result;
    CThingSearchTools() : lastArea(C2DCoordI(0, 0), C2DCoordI(0, 0)) { }
    template <class TFilter, class T>
    bool GetPFirstThingInRoughArea(const C2DBoxI& area, const TFilter& filter, T** found)
    {
        lastArea = area; notA = filter.f1->Thing; notB = filter.f2->Thing; rangePos = filter.f3->Pos; range = filter.f3->Range;
        *found = 0;
        return result;
    }
};
struct CWorld {
    unsigned char _pad00[0x20];
    CThingSearchTools* ThingSearchTools;                // +0x20
};
struct CTCSingleUserActionPoints {
    virtual void v00() { }
    CThing* ParentThing;                                // +0x04 (CTCBase)
    const CWorld* World;
    const CWorld* GetWorld() const { return World; }
    __forceinline bool FindFirstCreature(const C2DBoxI& area, const CActionPointFilter& filter, CThingCreatureBase** found) const
    {
        CThingSearchTools* searchTools = GetWorld()->ThingSearchTools;
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

int main()
{
    int bad = 0;
    CTCGraphicAppearance appearance;
    CThing owner; owner.GraphicAppearance = &appearance;
    CThing user;
    CThingSearchTools tools; tools.result = true;
    CWorld world; world.ThingSearchTools = &tools;
    CTCSingleUserActionPoints points; points.ParentThing = &owner; points.World = &world;
    C3DVector pos; CRightHandedSet* orient = (CRightHandedSet*)0x77;
    bool r = points.IsActionPointOccupied(&user, &pos, orient, 2);
    if (!r) bad |= 1;
    if (appearance.lastName != &g_CCharString_013bac9c || appearance.lastIndex != 2 || appearance.lastOrientation != orient || appearance.lastScale != 1.0f) bad |= 2;
    if (tools.lastArea.TLX != 12 || tools.lastArea.TLY != -3 || tools.lastArea.BRX != 12 || tools.lastArea.BRY != -3) bad |= 4;
    if (tools.notA != &owner || tools.notB != &user || tools.rangePos.X != pos.X || tools.rangePos.Y != pos.Y || tools.range != 0.5f) bad |= 8;
    tools.result = false;
    if (points.IsActionPointOccupied(&user, &pos, orient, 0)) bad |= 16;
    if (bad) { std::printf("007feaf0_TEST FAIL\n"); return 1; }
    std::printf("007feaf0_TEST PASS\n");
    return 0;
}
