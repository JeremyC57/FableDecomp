// Behaviour test for CTCNamedUserActionPoints::IsActionPointIndexBlocked (0x007FF0C0): the named dummy fills position and
// orientation, the search box is the truncated point, the filters exclude the owner and the user and use a 0.5 range, and
// the result is whether a creature was found.
#include <cstdio>
#include <cstring>
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector Up; C3DVector Forward; };
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) { }
};
struct CCharString { const char* PStringData; };
class CThing;
struct CThingCreatureBase { int id; };
struct CThingFilter_IsNot {
    CThing* Thing;
    CThingFilter_IsNot(CThing* thing) : Thing(thing) { }
};
struct CThingFilter_In2DRange {
    C3DVector Pos;
    float     Range;
    CThingFilter_In2DRange(const C3DVector& pos, float range) : Pos(pos), Range(range) { }
};
template <class A, class B, class C> struct CFilter_Bind3 {
    const A* F1;
    const B* F2;
    const C* F3;
    CFilter_Bind3(const A& a, const B& b, const C& c) : F1(&a), F2(&b), F3(&c) { }
};
static long g_box[4]; static CThing* g_not1 = 0; static CThing* g_not2 = 0; static float g_range = 0; static float g_px = 0;
static CThingCreatureBase g_blocker; static bool g_found = false;
struct CThingSearchTools {
    template <class TFilter, class T>
    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const TFilter& filter, T** out) const
    {
        g_box[0] = box.TLX; g_box[1] = box.TLY; g_box[2] = box.BRX; g_box[3] = box.BRY;
        g_not1 = filter.F1->Thing; g_not2 = filter.F2->Thing; g_range = filter.F3->Range; g_px = filter.F3->Pos.X;
        *out = g_found ? &g_blocker : 0;
        return g_found;
    }
};
static CThingSearchTools g_tools;
struct CWorld { const CThingSearchTools* PThingSearchTools; };
static CWorld g_world = { &g_tools };
static const char* g_dummyName = 0;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position, CRightHandedSet* orientation, float scale)
    {
        g_dummyName = name.PStringData;
        position->X = 3.75f; position->Y = -2.5f; position->Z = 1.0f;
        orientation->Up.Z = scale + (float)(index + (long)flags);
        return true;
    }
};
class CThing {
public:
    CTCGraphicAppearance* PTCGraphicAppearance;
};
class CBaseIntelligentPointer {
public:
    CThing* m_pItem;
    CThing* GetPItem() const { return m_pItem; }
};
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    T& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CTCUserActionPointBase {
    CThing* ParentThing;
    CBaseIntelligentPointer PThing;
    const CWorld* GetWorld() const { return &g_world; }
};
struct CTCNamedUserActionPoints : CTCUserActionPointBase {
    vector<CCharString> Names;
    bool IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const;
};

bool CTCNamedUserActionPoints::IsActionPointIndexBlocked(CThing& thing, C3DVector& position, CRightHandedSet& orientation, long index) const
{
    ParentThing->PTCGraphicAppearance->GetDummyObject(Names[index], 0, 0, &position, &orientation, 1.0f);
    long x = (long)position.X;
    long y = (long)position.Y;
    C2DBoxI box(x, y, x, y);
    CThingFilter_In2DRange inRange(position, 0.5f);
    CThingFilter_IsNot notUser(PThing.GetPItem());
    CThingFilter_IsNot notSelf(ParentThing);
    CFilter_Bind3<CThingFilter_IsNot, CThingFilter_IsNot, CThingFilter_In2DRange> filter(notSelf, notUser, inRange);
    const CThingSearchTools* tools = GetWorld()->PThingSearchTools;
    CThingCreatureBase* blocker;
    return tools->GetPFirstThingInRoughArea(box, filter, &blocker) ? true : false;
}

int main()
{
    int bad = 0;
    CTCGraphicAppearance appearance; CThing owner; owner.PTCGraphicAppearance = &appearance; CThing user;
    CCharString names[2] = { { "SEAT_A" }, { "SEAT_B" } };
    CTCNamedUserActionPoints p; p.ParentThing = &owner; p.PThing.m_pItem = &user;
    p.Names._M_start = names; p.Names._M_finish = names + 2; p.Names._M_end_of_storage = names + 2;
    C3DVector pos = { 0, 0, 0 }; CRightHandedSet set; std::memset(&set, 0, sizeof(set));
    g_found = false;
    if (p.IsActionPointIndexBlocked(user, pos, set, 1)) bad |= 1;
    if (std::strcmp(g_dummyName, "SEAT_B") != 0 || pos.X != 3.75f || set.Up.Z != 1.0f) bad |= 2;
    if (g_box[0] != 3 || g_box[1] != -2 || g_box[2] != 3 || g_box[3] != -2) bad |= 4;
    if (g_not1 != &owner || g_not2 != &user || g_range != 0.5f || g_px != 3.75f) bad |= 8;
    g_found = true;
    if (!p.IsActionPointIndexBlocked(user, pos, set, 0) || std::strcmp(g_dummyName, "SEAT_A") != 0) bad |= 16;
    if (bad) { std::printf("007ff0c0_TEST FAIL\n"); return 1; }
    std::printf("007ff0c0_TEST PASS\n");
    return 0;
}
