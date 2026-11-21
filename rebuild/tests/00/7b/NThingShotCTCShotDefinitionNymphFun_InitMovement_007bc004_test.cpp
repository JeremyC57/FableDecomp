// Behaviour test for NThingShot::CTCShotDefinitionNymphFun::InitMovement (0x007BC004): links the REAL row object. The test defines every
// callee the object imports (target lookup, alive test, positions, world map height, world seed, RandomiseShotDirection, normalise,
// ApplyAcceleration, ConstantFPS) and hand-builds the shot / parent thing / physics memory at retail offsets (PTargetThing +0x20,
// ParentThing +0x04, PShotDef +0x38 -> Speed +0x3C, AccuracyPercentage +0x40, physics +0x60 of the parent, Position +0x0C, vtable slots
// 32 ResetRHSet / 38 EnableGravity). 12 live cases are checked against an independent double-precision reference, plus a dead target.
#include <stdio.h>
#include <math.h>
#include <string.h>
static int failures;
static void check(bool ok, const char* what, int c) { if (!ok) { ++failures; printf("FAIL case %d: %s\n", c, what); } }
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector& AccurateNormalise(); };
C3DVector& C3DVector::AccurateNormalise() { double m = sqrt((double)X * X + (double)Y * Y + (double)Z * Z); if (m > 0.0) { X = (float)(X / m); Y = (float)(Y / m); Z = (float)(Z / m); } return *this; }
class CBaseObject;
class CThing { public: const C3DVector& GetPos() const; };
class CBaseIntelligentPointer { public: CBaseObject* GetPItem() const; };
class CWorldMap { public: float GetHeightZBelowPos(const C3DVector& pos) const; };
class CWorld { public: unsigned long* GetWorldSeed() const; };
class CTCBase { protected: CWorldMap& GetWorldMap(); CWorld& GetWorld() const; };
class CTCPhysicsBase { public: void ApplyAcceleration(const C3DVector& a); };
class CMainGameComponent { protected: static long ConstantFPS; friend int main(); };
long CMainGameComponent::ConstantFPS = 30;
template <class T> bool __fastcall IS_THING_ALIVE(const T& thing);
namespace NThingShot { C3DVector __fastcall RandomiseShotDirection(const C3DVector& direction, long accuracy, unsigned long* seed);
class CTCShotDefinitionNymphFun { public: virtual void InitMovement(); }; }

// fixtures
static unsigned char shot_mem[0xa8], parent_mem[0x100], target_mem[0x100], phys_mem[0x40], def_mem[0x40];
static void* phys_vtbl[40];
static C3DVector target_pos; static bool target_alive; static unsigned long world_seed;
static CWorldMap the_map; static CWorld the_world;
static int n_alive, n_getpos, n_height, n_rand, n_norm, n_reset, n_accel, n_grav, order;
static C3DVector height_arg, rand_arg, rh_up, rh_fwd, accel; static long rand_acc; static unsigned long* rand_seed; static bool grav_arg;
static int reset_at, accel_at, grav_at;

CBaseObject* CBaseIntelligentPointer::GetPItem() const { check((const unsigned char*)this == shot_mem + 0x20, "GetPItem this = PTargetThing (+0x20)", -1); return *(CBaseObject* const*)((const char*)this + 4); }
template <> bool __fastcall IS_THING_ALIVE<const CThing*>(const CThing* const& thing) { ++n_alive; return thing != 0 && target_alive; }
const C3DVector& CThing::GetPos() const { ++n_getpos; check((const unsigned char*)this == target_mem, "GetPos on the target", -1); return target_pos; }
CWorldMap& CTCBase::GetWorldMap() { check((unsigned char*)this == shot_mem, "GetWorldMap this", -1); return the_map; }
CWorld& CTCBase::GetWorld() const { check((const unsigned char*)this == shot_mem, "GetWorld this", -1); return the_world; }
float CWorldMap::GetHeightZBelowPos(const C3DVector& pos) const { ++n_height; height_arg = pos; return pos.Z * 0.5f - 1.25f + pos.X * 0.125f; }
unsigned long* CWorld::GetWorldSeed() const { return &world_seed; }
C3DVector __fastcall NThingShot::RandomiseShotDirection(const C3DVector& d, long accuracy, unsigned long* seed)
{
    ++n_rand; rand_arg = d; rand_acc = accuracy; rand_seed = seed; ++*seed;
    C3DVector r; r.X = d.X + accuracy * 0.01f; r.Y = d.Y - (float)(*seed % 7) * 0.25f; r.Z = d.Z * 0.5f + 1.0f; return r;
}
void CTCPhysicsBase::ApplyAcceleration(const C3DVector& a) { ++n_accel; accel_at = ++order; accel = a; check((unsigned char*)this == phys_mem, "ApplyAcceleration this", -1); }
struct FxPhys {
    void ResetRHSet(const float* set) { ++n_reset; reset_at = ++order; check((unsigned char*)this == phys_mem, "ResetRHSet this", -1);
        rh_up.X = set[0]; rh_up.Y = set[1]; rh_up.Z = set[2]; rh_fwd.X = set[3]; rh_fwd.Y = set[4]; rh_fwd.Z = set[5]; }
    void EnableGravity(bool on) { ++n_grav; grav_at = ++order; grav_arg = on; check((unsigned char*)this == phys_mem, "EnableGravity this", -1); }
    void Bad() { check(false, "unexpected virtual", -1); }
};
template <class F> static void* code_of(F f) { void* p; memcpy(&p, &f, sizeof p); return p; }
static void put_f(unsigned char* m, int off, float v) { memcpy(m + off, &v, 4); }
static void put_p(unsigned char* m, int off, const void* p) { memcpy(m + off, &p, 4); }
static bool near(double a, double b) { return fabs(a - b) <= 1e-4 * (1.0 + fabs(b)); }

int main()
{
    for (int i = 0; i < 40; ++i) phys_vtbl[i] = code_of(&FxPhys::Bad);
    phys_vtbl[32] = code_of(&FxPhys::ResetRHSet); phys_vtbl[38] = code_of(&FxPhys::EnableGravity);
    static const float cases[12][11] = {
        // physics pos xyz, target pos xyz, accuracy, speed, fps, seed, alive
        {0, 0, 0, 10, 0, 0, 0, 30, 30, 1, 1}, {0, 0, 0, 0, 10, 0, 50, 60, 30, 2, 1}, {1, 2, 3, 4, 6, 3, 99.9f, 15, 25, 3, 1},
        {-5, 4, 1, 7, -8, 2, 25, 45, 30, 4, 1}, {0, 0, 0, 3, 4, 12, 10, 30, 60, 5, 1}, {2, 2, 2, -2, -2, 0, 0, 90, 30, 6, 1},
        {100, 50, 0, 90, 70, 5, 75.5f, 12, 30, 7, 1}, {0, 0, 5, 1, 1, 0, 33, 33, 20, 8, 1}, {3, -3, 0, -3, 3, 0, 60, 75, 30, 9, 1},
        {0, 0, 0, 0.5f, 0.25f, 0, 5, 18, 30, 10, 1}, {-1, -1, -1, 1, 1, 1, 80, 45, 50, 11, 1}, {10, 0, 0, 10, 20, 3, 15, 22.5f, 30, 12, 0} };
    for (int c = 0; c < 12; ++c) {
        const float* k = cases[c];
        memset(shot_mem, 0, sizeof shot_mem); memset(parent_mem, 0, sizeof parent_mem); memset(phys_mem, 0, sizeof phys_mem); memset(def_mem, 0, sizeof def_mem);
        put_p(shot_mem, 0x04, parent_mem); put_p(shot_mem, 0x24, target_mem); put_p(shot_mem, 0x38, def_mem); put_f(shot_mem, 0x40, k[6]);
        put_f(def_mem, 0x3c, k[7]); put_p(parent_mem, 0x60, phys_mem); put_p(phys_mem, 0, phys_vtbl);
        put_f(phys_mem, 0x0c, k[0]); put_f(phys_mem, 0x10, k[1]); put_f(phys_mem, 0x14, k[2]);
        target_pos.X = k[3]; target_pos.Y = k[4]; target_pos.Z = k[5]; target_alive = k[10] != 0;
        CMainGameComponent::ConstantFPS = (long)k[8]; world_seed = (unsigned long)k[9];
        n_alive = n_getpos = n_height = n_rand = n_norm = n_reset = n_accel = n_grav = order = 0;
        ((NThingShot::CTCShotDefinitionNymphFun*)shot_mem)->NThingShot::CTCShotDefinitionNymphFun::InitMovement();
        check(n_alive == 1, "one alive test", c);
        if (!target_alive) {
            check(n_getpos == 0 && n_height == 0 && n_rand == 0 && n_reset == 0 && n_accel == 0 && n_grav == 0 && world_seed == (unsigned long)k[9], "dead target: nothing", c);
            continue;
        }
        // reference
        double fx = k[3], fy = k[4], fz = (double)(float)(k[5] + 0.5f);
        check(n_height == 1 && near(height_arg.X, fx) && near(height_arg.Y, fy) && near(height_arg.Z, fz), "height query at target + 0.5", c);
        fz = fz * 0.5 - 1.25 + fx * 0.125;
        double lx = fx - k[0], ly = fy - k[1], lz = fz - k[2];
        long acc = (long)k[6]; unsigned long seed = (unsigned long)k[9] + 1;
        check(n_rand == 1 && rand_acc == acc && rand_seed == &world_seed && near(rand_arg.X, lx) && near(rand_arg.Y, ly) && near(rand_arg.Z, lz), "randomise args", c);
        check(world_seed == seed, "seed advanced", c);
        double dx = lx + acc * 0.01, dy = ly - (double)(seed % 7) * 0.25, dz = lz * 0.5 + 1.0;
        double m = sqrt(dx * dx + dy * dy + dz * dz); dx /= m; dy /= m; dz /= m;
        double ux = dy, uy = -dx, uz = 0.0;
        double cx = uy * dz - uz * dy, cy = uz * dx - ux * dz, cz = ux * dy - uy * dx;
        double cm = sqrt(cx * cx + cy * cy + cz * cz); if (cm > 0) { cx /= cm; cy /= cm; cz /= cm; }
        check(n_reset == 1 && n_accel == 1 && n_grav == 1 && reset_at == 1 && accel_at == 2 && grav_at == 3 && !grav_arg, "call order / gravity off", c);
        check(near(rh_fwd.X, dx) && near(rh_fwd.Y, dy) && near(rh_fwd.Z, dz), "RH forward", c);
        check(near(rh_up.X, cx) && near(rh_up.Y, cy) && near(rh_up.Z, cz), "RH up", c);
        double s = (double)k[7] / (double)(long)k[8];
        check(near(accel.X, dx * s) && near(accel.Y, dy * s) && near(accel.Z, dz * s), "acceleration = forward * speed / fps", c);
    }
    if (failures) { printf("%d failures\n", failures); return 1; }
    puts("007bc004_TEST PASS");
    return 0;
}