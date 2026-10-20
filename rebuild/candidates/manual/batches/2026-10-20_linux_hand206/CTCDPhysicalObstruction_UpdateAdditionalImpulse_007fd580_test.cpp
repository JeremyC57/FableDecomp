// Behaviour test for CTCDPhysicalObstruction::UpdateAdditionalImpulse (0x007FD580): the search gets the line's area and the
// bound filters (ignore list, within the line radius without the thing radius, affected by obstructions); every creature
// found is handed to the smeared-line functor carrying the line ends and radius; the vector storage is freed.
#include <cstdio>
#include <cstdlib>
static int g_frees = 0; static bool g_armed = false;
static void SpyFree(void* p) { if (g_armed) ++g_frees; }   // records only, never frees
struct C3DVector { float X, Y, Z; };
struct C2DBoxI { long X0, Y0, X1, Y1; C2DBoxI() { } };
static const C3DVector* g_areaStart = 0; static float g_areaRadius = 0;
C2DBoxI GetLineThingSearchArea(const C3DVector& start, const C3DVector& end, float radius)
{ g_areaStart = &start; g_areaRadius = radius; C2DBoxI b; b.X0 = 1; b.Y0 = 2; b.X1 = 3; b.Y1 = 4; return b; }
struct CThing;
struct CThingCreatureBase { int id; };
struct CIntelligentPointer { void* vptr; CThing* item; };
template <class T> struct CRetailVector {
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CRetailVector() { if (_M_start) SpyFree(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
};
struct CThingFilter_IsNotList {
    const CRetailVector<const CThing*>*        PNotList;
    const CRetailVector<CIntelligentPointer>*  PNotList2;
    CThingFilter_IsNotList(const CRetailVector<CIntelligentPointer>& list) : PNotList(0), PNotList2(&list) { }
};
struct CThingFilter_IsWithinArea {
    const C3DVector* StartPos;
    const C3DVector* EndPos;
    float            Radius;
    bool             IncludeThingRad;
    CThingFilter_IsWithinArea(const C3DVector& start, const C3DVector& end, float radius, bool includeThingRad)
        : StartPos(&start), EndPos(&end), Radius(radius), IncludeThingRad(includeThingRad) { }
};
struct CThingFilter_IsAffectedByObstructions {
    unsigned char _empty;
    CThingFilter_IsAffectedByObstructions() : _empty(0) { }
};
template <class F1, class F2, class F3> struct CFilter_Bind3 {
    const F1* f1;
    const F2* f2;
    const F3* f3;
    CFilter_Bind3(const F1& a, const F2& b, const F3& c) : f1(&a), f2(&b), f3(&c) { }
};
typedef CFilter_Bind3<CThingFilter_IsNotList, CThingFilter_IsWithinArea, CThingFilter_IsAffectedByObstructions> CObstructionFilter;
static CThingCreatureBase* g_found[2];
static bool g_filterOk = false; static long g_boxX1 = 0;
static const void* g_expectIgnore = 0; static const C3DVector* g_expectStart = 0;
struct CThingSearchTools {
    template <class T, class TVector, class TFilter>
    void GetAllThingsInRoughArea(const C2DBoxI& area, const TFilter& filter, TVector& things)
    {
        g_boxX1 = area.X1;
        g_filterOk = filter.f1->PNotList == 0 && filter.f1->PNotList2 == g_expectIgnore && filter.f2->StartPos == g_expectStart
                  && filter.f2->Radius == 2.0f && !filter.f2->IncludeThingRad && filter.f3->_empty == 0;
        things._M_start = g_found; things._M_finish = g_found + 2; things._M_end_of_storage = g_found + 2;
    }
};
static CThingSearchTools g_tools;
static int g_applied = 0; static CThingCreatureBase* g_last = 0; static const C3DVector* g_fStart = 0; static float g_fRadius = 0;
struct CKeepOutOffSmearedOutLine {
    const C3DVector* StartPos;
    const C3DVector* EndPos;
    float            Radius;
    CKeepOutOffSmearedOutLine(const C3DVector& start, const C3DVector& end, float radius) : StartPos(&start), EndPos(&end), Radius(radius) { }
    void operator()(CThingCreatureBase* creature) { ++g_applied; g_last = creature; g_fStart = StartPos; g_fRadius = Radius; }
};
CKeepOutOffSmearedOutLine ForEach(CThingCreatureBase** first, CThingCreatureBase** last, CKeepOutOffSmearedOutLine f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
struct CTCDPhysicalObstruction {
    virtual void v00() { }
    CThing* ParentThing;
    C3DVector StartPos;
    C3DVector EndPos;
    long  LifeTimer;
    float Radius;
    CRetailVector<CIntelligentPointer> ThingsToIgnoreObstruction;
    CThingSearchTools* GetThingSearchTools() { return &g_tools; }
    void UpdateAdditionalImpulse();
};

void CTCDPhysicalObstruction::UpdateAdditionalImpulse()
{
    CRetailVector<CThingCreatureBase*> creatures;
    CThingFilter_IsWithinArea withinArea(StartPos, EndPos, Radius, false);
    C2DBoxI area = GetLineThingSearchArea(StartPos, EndPos, Radius);
    GetThingSearchTools()->GetAllThingsInRoughArea<CThingCreatureBase>(area,
        CObstructionFilter(CThingFilter_IsNotList(ThingsToIgnoreObstruction), withinArea, CThingFilter_IsAffectedByObstructions()), creatures);
    ForEach(creatures.begin(), creatures.end(), CKeepOutOffSmearedOutLine(StartPos, EndPos, Radius));
}

int main()
{
    int bad = 0;
    CThingCreatureBase a, b; g_found[0] = &a; g_found[1] = &b;
    CTCDPhysicalObstruction o; o.Radius = 2.0f;
    g_expectIgnore = &o.ThingsToIgnoreObstruction; g_expectStart = &o.StartPos;
    g_armed = true;
    o.UpdateAdditionalImpulse();
    g_armed = false;
    if (g_areaStart != &o.StartPos || g_areaRadius != 2.0f || g_boxX1 != 3 || !g_filterOk) bad |= 1;
    if (g_applied != 2 || g_last != &b || g_fStart != &o.StartPos || g_fRadius != 2.0f || g_frees != 1) bad |= 2;
    if (bad) { std::printf("007fd580_TEST FAIL\n"); return 1; }
    std::printf("007fd580_TEST PASS\n");
    return 0;
}
