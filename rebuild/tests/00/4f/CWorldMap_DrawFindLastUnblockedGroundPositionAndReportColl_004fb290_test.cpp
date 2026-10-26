#include <stdio.h>
class C3DVector { public: float X, Y, Z; };
struct CBaseClassNonCopyable { virtual ~CBaseClassNonCopyable() {} };
class CIDrawWorldMap { public:
    virtual ~CIDrawWorldMap() {}
    virtual void Draw1() const = 0;
    virtual void Draw2() const = 0;
    virtual void Draw3() const = 0;
    virtual void Draw4() const = 0;
    virtual void Draw5() const = 0;
    virtual void Draw6() const = 0;
    virtual void Draw7() const = 0;
    virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const = 0;
    virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const = 0;
};
static const void* g_this; static const C3DVector* g_from; static const C3DVector* g_to; static C3DVector* g_out;
class CWorldMap : public CBaseClassNonCopyable, public CIDrawWorldMap { public:
    bool result;
    virtual void Draw1() const {} virtual void Draw2() const {} virtual void Draw3() const {} virtual void Draw4() const {}
    virtual void Draw5() const {} virtual void Draw6() const {} virtual void Draw7() const {}
    bool FindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const
    { g_this = this; g_from = &from; g_to = &to; g_out = &lastUnblocked; lastUnblocked.X = to.X; return result; }
    virtual bool DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const;
    virtual bool DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const;
};

bool CWorldMap::DrawFindLastUnblockedGroundPositionAndReportCollision(const C3DVector& from, const C3DVector& to, C3DVector& lastUnblocked) const
{
    return FindLastUnblockedGroundPositionAndReportCollision(from, to, lastUnblocked);
}
bool CWorldMap::DrawFindLastUnblockedThemePositionAndReportCollision(const C3DVector&, const C3DVector&, C3DVector&) const { return false; }

int main()
{
    CWorldMap m; m.result = true;
    C3DVector a = { 1, 2, 3 }, b = { 4, 5, 6 }, c = { 0, 0, 0 };
    const CIDrawWorldMap* i = &m;
    bool r1 = i->DrawFindLastUnblockedGroundPositionAndReportCollision(a, b, c);
    bool ok = r1 && g_this == &m && (const void*)i != (const void*)&m && g_from == &a && g_to == &b && g_out == &c && c.X == 4;
    m.result = false;
    ok = ok && !i->DrawFindLastUnblockedGroundPositionAndReportCollision(a, b, c);
    printf(ok ? "004fb290_TEST PASS\n" : "004fb290_TEST FAIL\n");
    return ok ? 0 : 1;
}