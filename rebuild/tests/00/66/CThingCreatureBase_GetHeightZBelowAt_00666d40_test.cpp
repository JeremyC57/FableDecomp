// Behaviour test for CThingCreatureBase::GetHeightZBelowAt (0x00666D40): a copy of the row's statements (and of its same-TU functor
// class) over stand-ins with the retail layouts. The row object cannot be linked by a test outside one fixed path: it calls
// ForEachInRoughArea<CThingPhysical, CFilter_Bind3<...>, `anonymous namespace'::CThingFindHeightZBelowAt>, whose ?A0x<hash> name depends
// on the compile path (the 0x0055F710 / 0x006F2DE0 precedent). The real object is exercised by test/00666d40_realobj.cpp (mut.py's fixed
// path, test object linked first; the mutant run), with the same fixtures and checks.
// Fixtures: the creature image (PhysicsTC +0x60 -> CTCPhysicsBase Radius +0x34), one CWorldMap (theme / ground height doubles), one
// CThingSearchTools whose ForEachInRoughArea double records the box, the three bound filters and the functor (From / To references,
// Height), then plays the solid things: each hit Z raises Height when it lies above Height and below From.Z (the 0x00668AB0 rule).
#include <stdio.h>
#include <math.h>
#include <string.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); } };
class C2DBoxI { public: long TLX, TLY, BRX, BRY; C2DBoxI(long tlx, long tly, long brx, long bry) : TLX(tlx), TLY(tly), BRX(brx), BRY(bry) {} };
class CTCPhysicsBase { public: unsigned char p00[0x34]; float Radius; float GetRadius() const { return Radius; } };
class CWorldMap { public: float GetThemeSizeZAt(const C3DVector& pos) const; float GetGroundSizeZAt(const C3DVector& pos) const; };
class CThingSearchTools;
class CThing { public: unsigned char p00[0x60]; CTCPhysicsBase* PhysicsTC;
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    const CWorldMap& PeekWorldMap() const; const CThingSearchTools& PeekThingSearchTools() const; };
class CThingPhysical : public CThing {};
class CTCSolidToThing;
class CThingFilter_SolidToThing { public: CThingFilter_SolidToThing(const CThing& source_thing) : SourceThing(&source_thing), PTCSolidToThing(0) {}
    const CThing* SourceThing; CTCSolidToThing* PTCSolidToThing; };
class CThingFilter_PhysicsFlagsSolidToThing { public: CThingFilter_PhysicsFlagsSolidToThing(const CThing& source_thing)
    : SourceThing(source_thing), SourceThingPhysicsTC(source_thing.GetPhysicsTC()) {}
    const CThing& SourceThing; const CTCPhysicsBase* SourceThingPhysicsTC; };
class CThingFilter_OverlappingCircle { public: CThingFilter_OverlappingCircle(const C3DVector& pos, float radius) : SpherePos(pos), Radius(radius) {}
    C3DVector SpherePos; float Radius; };
template<class A, class B, class C> class CFilter_Bind3 { public: CFilter_Bind3(const A& a, const B& b, const C& c) : F1(a), F2(b), F3(c) {}
    const A& F1; const B& F2; const C& F3; };
template<class A, class B, class C> inline CFilter_Bind3<A, B, C> GFFilter_Bind3(const A& a, const B& b, const C& c) { return CFilter_Bind3<A, B, C>(a, b, c); }
struct FunctorImage { const C3DVector* From; const C3DVector* To; float Height; };
extern "C" bool __fastcall ForEachDouble(const CThingSearchTools* tools, int, const long* area, const void* const* filter, FunctorImage* func);
class CThingSearchTools { public: int dummy;
    template<class T, class Filter, class Proc> bool ForEachInRoughArea(const C2DBoxI& area, const Filter& filter, const Proc& proc) const
    { return ForEachDouble(this, 0, &area.TLX, (const void* const*)&filter, (FunctorImage*)&proc); } };
namespace {
class CThingFindHeightZBelowAt {
public:
    CThingFindHeightZBelowAt(const C3DVector& from, const C3DVector& to, float height) : From(from), To(to), Height(height) {}
        float GetHeight() const { return Height; }
private:
    const C3DVector& From;                                             // +0x00
    const C3DVector& To;                                               // +0x04
    mutable float Height;                                              // +0x08
};
}
class CThingCreatureBase : public CThing { public: float GetHeightZBelowAt(const C3DVector& pos, bool include_theme_height, bool include_things) const; };
// the row's statements
float CThingCreatureBase::GetHeightZBelowAt(const C3DVector& pos, bool include_theme_height, bool include_things) const
{
    float extra_size_area = 1.0f;
    float radius = GetPhysicsTC()->GetRadius();
    C2DBoxI area((long)(pos.X - radius - extra_size_area), (long)(pos.Y - radius - extra_size_area),
                 (long)(pos.X + radius + extra_size_area), (long)(pos.Y + radius + extra_size_area));
    float height_z = include_theme_height ? PeekWorldMap().GetThemeSizeZAt(pos) : PeekWorldMap().GetGroundSizeZAt(pos);
    if (include_things) {
        C3DVector to_centre(0.0f, 0.0f, radius);
        C3DVector from = pos + to_centre;
        C3DVector to = from;
        from.Z += 1.0f;
        to.Z -= 50.0f;
        CThingFindHeightZBelowAt get_height_below_func(from, to, height_z);
        PeekThingSearchTools().ForEachInRoughArea<CThingPhysical>(area, GFFilter_Bind3(CThingFilter_SolidToThing(*this), CThingFilter_PhysicsFlagsSolidToThing(*this), CThingFilter_OverlappingCircle(pos, radius)), get_height_below_func);
        height_z = get_height_below_func.GetHeight();
    }
    return height_z;
}

static unsigned char creature_mem[0x100], phys_mem[0x40];
static CWorldMap the_map; static CThingSearchTools the_tools;
static float theme_z, ground_z; static int n_theme, n_ground, n_peek_map, n_peek_tools, n_foreach;
static const C3DVector* theme_arg; static const C3DVector* ground_arg;
static long box[4]; static const void* f1_src; static const void* f1_ptc; static const void* f2_src; static const void* f2_ptc;
static C3DVector f3_pos; static float f3_radius; static C3DVector got_from, got_to; static float got_height_in;
static int n_hits; static float hits[4];

const CWorldMap& CThing::PeekWorldMap() const { ++n_peek_map; check((const unsigned char*)this == creature_mem, "PeekWorldMap this", -1); return the_map; }
const CThingSearchTools& CThing::PeekThingSearchTools() const { ++n_peek_tools; check((const unsigned char*)this == creature_mem, "PeekThingSearchTools this", -1); return the_tools; }
float CWorldMap::GetThemeSizeZAt(const C3DVector& pos) const { ++n_theme; theme_arg = &pos; check(this == &the_map, "theme map", -1); return theme_z; }
float CWorldMap::GetGroundSizeZAt(const C3DVector& pos) const { ++n_ground; ground_arg = &pos; check(this == &the_map, "ground map", -1); return ground_z; }
extern "C" bool __fastcall ForEachDouble(const CThingSearchTools* tools, int, const long* area, const void* const* filter, FunctorImage* func)
{
    ++n_foreach; check(tools == &the_tools, "ForEachInRoughArea this", -1);
    memcpy(box, area, sizeof box);
    const void* const* f1 = (const void* const*)filter[0]; const void* const* f2 = (const void* const*)filter[1]; const float* f3 = (const float*)filter[2];
    f1_src = f1[0]; f1_ptc = f1[1]; f2_src = f2[0]; f2_ptc = f2[1];
    f3_pos = C3DVector(f3[0], f3[1], f3[2]); f3_radius = f3[3];
    got_from = *func->From; got_to = *func->To; got_height_in = func->Height;
    for (int i = 0; i < n_hits; ++i)
        if (hits[i] > func->Height && hits[i] < func->From->Z) func->Height = hits[i];
    return true;
}
static void put_f(unsigned char* m, int off, float v) { memcpy(m + off, &v, 4); }
static void put_p(unsigned char* m, int off, const void* p) { memcpy(m + off, &p, 4); }
static bool near(double a, double b) { return fabs(a - b) <= 1e-4 * (1.0 + fabs(b)); }

static void run(int c, float x, float y, float z, float radius, bool theme, bool things, float tz, float gz, int nh, const float* h)
{
    put_p(creature_mem, 0x60, phys_mem); put_f(phys_mem, 0x34, radius);
    theme_z = tz; ground_z = gz; n_hits = nh; for (int i = 0; i < nh; ++i) hits[i] = h[i];
    n_theme = n_ground = n_peek_map = n_peek_tools = n_foreach = 0; f1_ptc = (const void*)1;
    C3DVector pos(x, y, z);
    float r = ((const CThingCreatureBase*)creature_mem)->GetHeightZBelowAt(pos, theme, things);
    // reference
    double base = theme ? tz : gz, want = base;
    check(n_peek_map == 1, "one world map peek", c);
    check(theme ? (n_theme == 1 && n_ground == 0 && theme_arg == &pos) : (n_ground == 1 && n_theme == 0 && ground_arg == &pos), "theme / ground query on pos", c);
    if (things) {
        double fz = (double)z + radius + 1.0, tzz = (double)z + radius - 50.0;
        for (int i = 0; i < nh; ++i) if (h[i] > want && h[i] < fz) want = h[i];
        check(n_peek_tools == 1 && n_foreach == 1, "one rough-area search", c);
        check(box[0] == (long)(x - radius - 1.0f) && box[1] == (long)(y - radius - 1.0f) && box[2] == (long)(x + radius + 1.0f) && box[3] == (long)(y + radius + 1.0f), "search box pos -+ (radius + 1), truncated", c);
        check(f1_src == creature_mem && f1_ptc == 0, "SolidToThing(this): source, no solid TC", c);
        check(f2_src == creature_mem && f2_ptc == phys_mem, "PhysicsFlagsSolidToThing(this): source, its physics TC", c);
        check(f3_pos.X == x && f3_pos.Y == y && f3_pos.Z == z && f3_radius == radius, "OverlappingCircle(pos, radius)", c);
        check(got_from.X == x && got_from.Y == y && near(got_from.Z, fz), "from = pos + (0, 0, radius + 1)", c);
        check(got_to.X == x && got_to.Y == y && near(got_to.Z, tzz), "to = pos + (0, 0, radius - 50)", c);
        check(got_height_in == (float)base, "functor starts at the map height", c);
    } else
        check(n_peek_tools == 0 && n_foreach == 0, "no search without include_things", c);
    check(near(r, want), "result", c);
}

int main()
{
    float h1[] = { 3.5f, 2.0f, 9.0f };                  // 9.0 above from (rejected), 3.5 the highest valid
    float h2[] = { -10.0f };                            // below the map height
    float h3[] = { 1.25f, 1.75f };
    run(1, 10.5f, 20.25f, 1.0f, 0.75f, true, false, 4.0f, 2.0f, 0, 0);
    run(2, 10.5f, 20.25f, 1.0f, 0.75f, false, false, 4.0f, 2.0f, 0, 0);
    run(3, 10.5f, 20.25f, 2.0f, 0.75f, false, true, 4.0f, 1.0f, 3, h1);
    run(4, 10.5f, 20.25f, 2.0f, 0.75f, true, true, 1.5f, 1.0f, 1, h2);
    run(5, -7.4f, -0.3f, 0.0f, 1.65f, false, true, 0.0f, 0.5f, 2, h3);
    run(6, 3.9f, -12.6f, -5.0f, 0.0f, true, true, -6.0f, 0.0f, 3, h1);
    run(7, 100.0f, 50.0f, 0.5f, 2.5f, false, true, 0.0f, 0.25f, 2, h3);
    if (failures) { printf("00666d40_TEST FAIL (%d)\n", failures); return 1; }
    printf("00666d40_TEST PASS\n");
    return 0;
}