// Behaviour test for NUISystem::CList::CanMove (0x00537776): the row body verbatim over models of its callees. Kills: the result
// not starting from `where`, the wrong Y replacement, scroll_up inverted, either comparison inverted or using the wrong edge
// (bottom when scrolling up, top otherwise), the UI scale or the converted top offset dropped, the relative / dummy offsets ignored or
// applied in the wrong order (the dummy wins), the definition not released.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static void check(bool v, int line) { if (!v) { printf("FAIL line %d\n", line); exit(1); } }
#define CHECK(c) check((c), __LINE__)
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector operator+(const C2DVector& v) const { return C2DVector(v.X + X, Y + v.Y); }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
};
struct C2DBoxI {
    long TLX, TLY, BRX, BRY;
    long GetTLX() const { return TLX; } long GetTLY() const { return TLY; } long GetBRY() const { return BRY; }
    long GetWidth() const { return BRX - TLX; } long GetHeight() const { return BRY - TLY; }
    void SetBRX(long v) { BRX = v; } void SetBRY(long v) { BRY = v; }
};
struct CUIDef { C2DBoxI ViewArea; };
static CUIDef the_def; static int def_refs;
template <class T> struct CDefPointer {
    const T* p;
    CDefPointer(const T* q) : p(q) { ++def_refs; }
    CDefPointer(const CDefPointer& o) : p(o.p) { ++def_refs; }
    ~CDefPointer() { --def_refs; }
    const T* operator->() const { return p; }
};
static C2DVector ui_scale;
struct CManager { static CManager* GetInstance() { static CManager m; return &m; } C2DVector GetUIScale() const { return ui_scale; } };
struct CUIScreenUnitConverter {
    static C2DVector ConvertVector(C2DVector v) { return C2DVector(v.X * 2.0f, v.Y * 2.0f); }
    static float ConvertY(float y) { return y * 2.0f; }
};
struct CComponent { C2DVector Rel; C2DVector GetRelativeRenderPosition() { return Rel; } };
namespace NUISystem {
class CList {
public:
    C2DVector Position, TargetPosition, RelativeRenderPosition, DynSize; bool UsingRel; CComponent* PInitialPositionDummy;
    CDefPointer<const CUIDef> LoadAndReturnDef() const { return CDefPointer<const CUIDef>(&the_def); }
    bool IsUsingRelativePosition() const { return UsingRel; }
    C2DVector GetDynamicSize() const { return DynSize; }
    C2DVector CanMove(const C2DVector& where);
};
}
using namespace NUISystem;
// ---- row body (verbatim) ----
C2DVector CList::CanMove(const C2DVector& where)
{
    C2DVector result(where);
    C2DVector testing_view_area(where);
    CDefPointer<const CUIDef> pdefinition = LoadAndReturnDef();
    bool scroll_up = where.Y < Position.Y;
    C2DVector view_area_offset(0.0f, 0.0f);
    C2DBoxI view_area(pdefinition->ViewArea);
    view_area.SetBRX((long)(view_area.GetTLX() + view_area.GetWidth() * CManager::GetInstance()->GetUIScale().X));
    view_area.SetBRY((long)(view_area.GetTLY() + view_area.GetHeight() * CManager::GetInstance()->GetUIScale().Y));
    if (IsUsingRelativePosition())
        testing_view_area = RelativeRenderPosition + CUIScreenUnitConverter::ConvertVector(where - Position);
    view_area_offset.Y = CUIScreenUnitConverter::ConvertY((float)view_area.GetTLY()) - view_area.GetTLY();
    view_area_offset.Y *= CManager::GetInstance()->GetUIScale().Y;
    if (PInitialPositionDummy)
        testing_view_area = PInitialPositionDummy->GetRelativeRenderPosition() + CUIScreenUnitConverter::ConvertVector(where - Position);
    if (scroll_up) {
        float size_y = GetDynamicSize().Y * CManager::GetInstance()->GetUIScale().Y;
        if (testing_view_area.Y + size_y < view_area.GetBRY() + view_area_offset.Y)
            result.Y = TargetPosition.Y;
    } else {
        if (view_area.GetTLY() + view_area_offset.Y < testing_view_area.Y)
            result.Y = TargetPosition.Y;
    }
    return result;
}
// ---- end row body ----
static C2DVector model(const CList& l, const C2DVector& where)
{
    C2DBoxI va = the_def.ViewArea;
    long bry = (long)(va.TLY + (va.BRY - va.TLY) * ui_scale.Y);
    C2DVector t = where;
    C2DVector conv(2.0f * (where.X - l.Position.X), 2.0f * (where.Y - l.Position.Y));
    if (l.UsingRel) t = C2DVector(l.RelativeRenderPosition.X + conv.X, l.RelativeRenderPosition.Y + conv.Y);
    float off = (2.0f * va.TLY - va.TLY) * ui_scale.Y;
    if (l.PInitialPositionDummy) t = C2DVector(l.PInitialPositionDummy->Rel.X + conv.X, l.PInitialPositionDummy->Rel.Y + conv.Y);
    C2DVector r = where;
    if (where.Y < l.Position.Y) { if (t.Y + l.DynSize.Y * ui_scale.Y < bry + off) r.Y = l.TargetPosition.Y; }
    else if (va.TLY + off < t.Y) r.Y = l.TargetPosition.Y;
    return r;
}
int main()
{
    the_def.ViewArea.TLX = 10; the_def.ViewArea.TLY = 20; the_def.ViewArea.BRX = 110; the_def.ViewArea.BRY = 220;
    CComponent dummy; dummy.Rel = C2DVector(-30.0f, 50.0f);
    const float ys[] = { -100.0f, 0.0f, 15.0f, 60.0f, 300.0f, 900.0f };
    const float sc[] = { 1.0f, 1.5f, 0.5f };
    long cases = 0, replaced = 0, kept_up = 0, kept_down = 0;
    for (int rel = 0; rel != 2; ++rel) for (int dm = 0; dm != 2; ++dm) for (int s = 0; s != 3; ++s) for (int y = 0; y != 6; ++y) for (int py = 0; py != 3; ++py) {
        ui_scale = C2DVector(2.0f, sc[s]);
        CList l; l.Position = C2DVector(5.0f, ys[(py * 2 + 1) % 6]); l.TargetPosition = C2DVector(-7.0f, 4242.0f);
        l.RelativeRenderPosition = C2DVector(3.0f, 40.0f); l.DynSize = C2DVector(9.0f, 30.0f); l.UsingRel = rel != 0;
        l.PInitialPositionDummy = dm ? &dummy : 0;
        C2DVector where(12.5f, ys[y]);
        def_refs = 0;
        C2DVector got = l.CanMove(where), want = model(l, where);
        CHECK(def_refs == 0);
        CHECK(got.X == where.X && got.X == want.X && got.Y == want.Y);
        if (got.Y == 4242.0f) ++replaced; else if (where.Y < l.Position.Y) ++kept_up; else ++kept_down;
        ++cases;
    }
    CHECK(cases == 216 && replaced > 0 && kept_up > 0 && kept_down > 0);
    puts("00537776_TEST PASS");
    return 0;
}