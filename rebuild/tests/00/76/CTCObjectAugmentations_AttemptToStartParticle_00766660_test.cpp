// Behaviour test for 0x00766660 CTCObjectAugmentations::AttemptToStartParticle (links the REAL row object: its expanded GetTC with the
// interface-map LowerBound, the CIntelligentPointer<CThing> ctor / assignment / dtor, IS_THING_ALIVE, std::max and the inline emitter
// setters run as compiled).
// Fixtures (raw native layout, every offset read by an instruction of the row): the component's ParentThing +0x04 (0x007666A4);
// CThing PTCGraphicAppearance +0x64 (0x007666AB), interface bits +0x20.. (TC 0x4B: +0x28 bit 11, 0x007667EB), the interface map vector
// +0x44 / +0x48 of (type, component) pairs (0x007667F4), the dead bit +0x91 bit 0 (0x007667B9); CTCDParticleEmitter PEmitter +0x14
// (0x0076684E); CParticleEmitter WaitingToSleep +0x41 bit 1 (0x00766862), AlwaysUpdate +0x44 bit 4 (0x007668EF); the intelligent
// pointer's item at +0x04 behind its vptr. The out-of-line engine callees are provided here and log every call (receiver, arguments);
// CBaseIntelligentPointer is modelled as a plain item holder.
// Reference model (independent): pfx cleared; start / end point each from the helper point, else the dummy object (else return);
// effectid <= 0 returns; pfx = Create(effectid, parent pos, true), blend = Create(effectidtoblendto, parent pos, true); when both are
// set and not dead: the emitters' particle emitters (TC 0x4B of each), blend emitter Sleep, distance |start - end|, the blend's then
// the effect's spline lengths, InterpolateTo(blend emitter, (distance - short) / max(long - short, 1e-4) * 2), effect AlwaysUpdate,
// AttachToThing(parent, 10, startpoint, 0, 0), blend Kill(true).
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

class CBaseObject;
class CCharString { public: void* PStringData; ~CCharString(); };
CCharString::~CCharString() { }
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Forward, Up; };
class CThing { public: const C3DVector& GetPos() const; void Kill(bool immediate); };
namespace NParticleEngine { class CParticleEmitter { public: float GetApproximateTotalSplineLength(); void InterpolateTo(const CParticleEmitter& other, float t); }; }
namespace NGraphicAppearance { class CTCGraphicAppearance { public:
    bool GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float scale) const;
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& orientation, float scale) const; }; }
class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long id, const C3DVector& pos, bool flag);
    void AttachToThing(CThing& thing, long type, const CCharString& name, long index, float offset); };
template<class T> class CIntelligentPointer;
class CTCObjectAugmentations {
    void AttemptToStartParticle(CIntelligentPointer<CThing>& pfx, long effectid, long effectidtoblendto, const CCharString& startpoint, const CCharString& endpoint);
public:
    static void Call(CTCObjectAugmentations* t, CIntelligentPointer<CThing>& pfx, long a, long b, const CCharString& s, const CCharString& e)
    { t->AttemptToStartParticle(pfx, a, b, s, e); } };
extern const float DELTA_NOTIONAL_ZERO = 1e-4f;
class CBaseIntelligentPointer { public:
    CBaseObject* PData;
    CBaseIntelligentPointer(); virtual ~CBaseIntelligentPointer();
    CBaseObject* GetPItem() const; void SetPItem(CBaseObject* item); };
static int g_liveIP;
CBaseIntelligentPointer::CBaseIntelligentPointer() { PData = 0; ++g_liveIP; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { --g_liveIP; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return PData; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = item; }
extern "C" void __cdecl Trap() { printf("FAIL unexpected call\n"); exit(3); }
#pragma comment(linker, "/alternatename:??_E?$CIntelligentPointer@VCThing@@@@UAEPAXI@Z=_Trap")

// ---- event log
struct Ev { char t; long a, b, c, d; float f; };
static Ev g_log[64]; static int g_n;
static Ev& Log(char t, long a = 0, long b = 0, long c = 0, long d = 0) { static Ev j; if (g_n >= 64) return j; Ev& e = g_log[g_n++]; memset(&e, 0, sizeof e); e.t = t; e.a = a; e.b = b; e.c = c; e.d = d; return e; }

// ---- raw objects
struct Thing { unsigned char b[0xA0]; };
struct Emitter { unsigned char b[0x40]; };      // CTCDParticleEmitter: PEmitter = {vptr +0x14, item +0x18}
struct PEm { unsigned char b[0x60]; float len; };
static Thing g_parent, g_fx, g_blend;
static Emitter g_e1, g_e2, g_eDecoy;
static PEm g_p1, g_p2;
static unsigned char g_app[4];
static unsigned char g_comp[8];
struct Pair { long type; void* comp; };
static Pair g_mapFx[3], g_mapBlend[3];
static C3DVector g_parentPos = { 5.0f, 6.0f, 7.0f };

struct Cfg { bool hs, ds, he, de; C3DVector sp, ep; long effectid, blendid; bool fxNull, blendNull, fxDead, blendDead; float len1, len2; };
static Cfg g_c;

static void SetupThing(Thing& t, Pair* map, Emitter* e)
{
    memset(&t, 0, sizeof t);
    t.b[0x28 + 1] = 0x08;                                         // TC 0x4B present: +0x28 bit 11
    map[0].type = 0x4A; map[0].comp = &g_eDecoy; map[1].type = 0x4B; map[1].comp = e; map[2].type = 0x4C; map[2].comp = &g_eDecoy;
    Pair* s = map; Pair* f = map + 3; memcpy(t.b + 0x44, &s, 4); memcpy(t.b + 0x48, &f, 4); memcpy(t.b + 0x4C, &f, 4);
}
const C3DVector& CThing::GetPos() const { Log('P', (long)this); return g_parentPos; }
void CThing::Kill(bool immediate) { Log('K', (long)this, immediate); }
float NParticleEngine::CParticleEmitter::GetApproximateTotalSplineLength() { Log('L', (long)this); return ((PEm*)this)->len; }
void NParticleEngine::CParticleEmitter::InterpolateTo(const CParticleEmitter& other, float t)
{ Ev& e = Log('I', (long)this, (long)&other); e.f = t; e.c = ((PEm*)&other)->b[0x41]; e.d = ((PEm*)this)->b[0x44]; }
bool NGraphicAppearance::CTCGraphicAppearance::GetHelperPoint(const CCharString& name, long index, unsigned long flags, C3DVector& pos, float scale) const
{
    Ev& e = Log('H', (long)this, (long)&name, index * 16 + (long)flags); e.f = scale;
    bool start = e.b == (long)&g_comp[0]; bool ok = start ? g_c.hs : g_c.he;   // see Run: startpoint is g_comp[0], endpoint g_comp[4]
    if (ok) pos = start ? g_c.sp : g_c.ep; else { pos.X = 1e9f; pos.Y = 1e9f; pos.Z = 1e9f; }
    e.d = (long)&pos; return ok;
}
bool NGraphicAppearance::CTCGraphicAppearance::GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector& pos, CRightHandedSet& orientation, float scale) const
{
    Ev& e = Log('D', (long)this, (long)&name, index * 16 + (long)flags); e.f = scale;
    bool start = e.b == (long)&g_comp[0]; bool ok = start ? g_c.ds : g_c.de;
    if (ok) pos = start ? g_c.sp : g_c.ep;
    orientation.Forward.X = 3.0f;
    e.d = (long)&pos; return ok;
}
CThing* __fastcall CTCDParticleEmitter::Create(long id, const C3DVector& pos, bool flag)
{
    Log('C', id, (long)&pos, flag);
    if (id == g_c.effectid) return g_c.fxNull ? 0 : (CThing*)&g_fx;
    return g_c.blendNull ? 0 : (CThing*)&g_blend;
}
void CTCDParticleEmitter::AttachToThing(CThing& thing, long type, const CCharString& name, long index, float offset)
{ Ev& e = Log('A', (long)this, (long)&thing, type, (long)&name); e.f = offset + (float)index * 1000.0f; }

static int failures;
static void check(bool ok, const char* what, int k) { if (!ok) { ++failures; if (failures < 40) printf("FAIL %s (case %d)\n", what, k); } }

static void Run(const Cfg& c, int k)
{
    g_c = c;
    SetupThing(g_parent, g_mapFx, &g_e1);                         // the parent's own map is never read
    void* app = g_app; memcpy(g_parent.b + 0x64, &app, 4);
    SetupThing(g_fx, g_mapFx, &g_e1); SetupThing(g_blend, g_mapBlend, &g_e2);
    g_fx.b[0x91] = c.fxDead ? 0x03 : 0x02;                        // dead bit 0; a neighbouring bit always set
    g_blend.b[0x91] = c.blendDead ? 0x05 : 0x04;
    memset(&g_e1, 0, sizeof g_e1); memset(&g_e2, 0, sizeof g_e2); memset(&g_eDecoy, 0, sizeof g_eDecoy);
    void* p1 = &g_p1; void* p2 = &g_p2; memcpy(g_e1.b + 0x18, &p1, 4); memcpy(g_e2.b + 0x18, &p2, 4);
    memset(&g_p1, 0, sizeof g_p1); memset(&g_p2, 0, sizeof g_p2);
    g_p1.b[0x41] = 0x80; g_p2.b[0x41] = 0x01; g_p1.b[0x44] = 0x01; g_p2.b[0x44] = 0x20;   // neighbouring bits preserved
    g_p1.len = c.len1; g_p2.len = c.len2;
    // the component: {vptr, ParentThing}; the caller's pfx holds a stale item first
    void* comp[2] = { 0, &g_parent };
    struct { void* vptr; void* item; } pfx = { (void*)0x1234, (void*)0x5678 };
    g_n = 0; g_liveIP = 0;
    CTCObjectAugmentations::Call((CTCObjectAugmentations*)(void*)comp, *(CIntelligentPointer<CThing>*)(void*)&pfx, c.effectid, c.blendid,
                                                                  *(const CCharString*)(void*)&g_comp[0], *(const CCharString*)(void*)&g_comp[4]);
    // model
    Ev exp[64]; int n = 0; Ev* e;
#define M(T) (e = &exp[n++], memset(e, 0, sizeof *e), e->t = T, e)
    void* expItem = 0; bool done = false;
    long st = (long)&g_comp[0], en = (long)&g_comp[4];
    e = M('H'); e->a = (long)g_app; e->b = st; e->f = 1.0f;
    bool sOk = c.hs;
    if (!sOk) { e = M('D'); e->a = (long)g_app; e->b = st; e->f = 1.0f; sOk = c.ds; }
    if (!sOk) done = true;
    bool eOk = false;
    if (!done) {
        e = M('H'); e->a = (long)g_app; e->b = en; e->f = 1.0f; eOk = c.he;
        if (!eOk) { e = M('D'); e->a = (long)g_app; e->b = en; e->f = 1.0f; eOk = c.de; }
        if (!eOk) done = true;
    }
    if (!done && c.effectid <= 0) done = true;
    bool attached = false;
    if (!done) {
        M('P')->a = (long)&g_parent; e = M('C'); e->a = c.effectid; e->b = (long)&g_parentPos; e->c = 1;
        M('P')->a = (long)&g_parent; e = M('C'); e->a = c.blendid; e->b = (long)&g_parentPos; e->c = 1;
        expItem = c.fxNull ? 0 : (void*)&g_fx;
        bool fxAlive = !c.fxNull && !c.fxDead;
        bool blendAlive = !c.blendNull && !c.blendDead;
        if (fxAlive && blendAlive) {
            attached = true;
            float dx = c.sp.X - c.ep.X, dy = c.sp.Y - c.ep.Y, dz = c.sp.Z - c.ep.Z;
            float dist = (float)sqrt((double)(dx * dx + dy * dy + dz * dz));
            M('L')->a = (long)&g_p2; M('L')->a = (long)&g_p1;
            float diff = c.len2 - c.len1;
            float den = diff < DELTA_NOTIONAL_ZERO ? DELTA_NOTIONAL_ZERO : diff;
            e = M('I'); e->a = (long)&g_p1; e->b = (long)&g_p2; e->f = (dist - c.len1) / den * 2.0f; e->c = 0x03; e->d = 0x01;
            e = M('A'); e->a = (long)&g_e1; e->b = (long)&g_parent; e->c = 10; e->d = st; e->f = 0.0f;
            e = M('K'); e->a = (long)&g_blend; e->b = 1;
        }
    }
#undef M
    check(g_n == n, "event count", k);
    for (int i = 0; i < n && i < g_n; ++i) {
        const Ev& a = g_log[i]; const Ev& b = exp[i];
        bool ok = a.t == b.t && a.a == b.a;
        if (ok) switch (b.t) {
            case 'H': case 'D': ok = a.b == b.b && a.c == 0 && a.f == 1.0f; break;
            case 'C': ok = a.b == b.b && a.c == b.c; break;
            case 'I': ok = a.b == b.b && a.c == b.c && a.d == b.d && fabs(a.f - b.f) <= 1e-4f * (1.0f + fabs(b.f)); break;
            case 'A': ok = a.b == b.b && a.c == b.c && a.d == b.d && a.f == 0.0f; break;
            case 'K': ok = a.b == b.b; break;
        }
        if (!ok) { check(false, "event", k); printf("  case %d event %d: got %c(%lx,%lx,%lx,%lx,%g) want %c(%lx,%lx,%lx,%lx,%g)\n", k, i,
                                                 a.t, a.a, a.b, a.c, a.d, a.f, b.t, b.a, b.b, b.c, b.d, b.f); break; }
    }
    // the start point is resolved into the object the end point does not overwrite (H/D out pointers differ)
    check(pfx.item == expItem, "pfx item", k);
    check(pfx.vptr == (void*)0x1234, "pfx vptr untouched", k);
    check(g_liveIP == 0, "temporaries destroyed", k);
    check(g_p2.b[0x41] == (attached ? 0x03 : 0x01), "blend emitter Sleep", k);
    check(g_p1.b[0x44] == (attached ? 0x11 : 0x01), "effect emitter AlwaysUpdate", k);
    check(g_p1.b[0x41] == 0x80 && g_p2.b[0x44] == 0x20, "other emitter flags untouched", k);
    check(g_eDecoy.b[0x18] == 0, "decoy component untouched", k);
}

int main()
{
    Cfg base = { true, false, true, false, { 1.0f, 2.0f, 3.0f }, { 4.0f, 6.0f, 15.0f }, 7, 9, false, false, false, false, 2.0f, 10.0f };
    Cfg c;
    Run(base, 1);                                                          // helper points, both alive: attach
    c = base; c.hs = false; c.ds = true; Run(c, 2);                       // start from the dummy object
    c = base; c.hs = false; c.ds = false; Run(c, 3);                      // no start point: return after clearing
    c = base; c.he = false; c.de = true; Run(c, 4);
    c = base; c.he = false; c.de = false; Run(c, 5);
    c = base; c.effectid = 0; Run(c, 6);                                  // effect 0: nothing created
    c = base; c.effectid = -3; Run(c, 7);
    c = base; c.effectid = 1; Run(c, 8);                                  // the bound itself creates
    c = base; c.fxNull = true; Run(c, 9);
    c = base; c.blendNull = true; Run(c, 10);
    c = base; c.fxDead = true; Run(c, 11);
    c = base; c.blendDead = true; Run(c, 12);
    c = base; c.len1 = 5.0f; c.len2 = 5.0f; Run(c, 13);                   // equal lengths: the 1e-4 floor
    c = base; c.len1 = 8.0f; c.len2 = 3.0f; Run(c, 14);                   // negative difference: floored
    c = base; c.len1 = 1.0f; c.len2 = 1.0002f; Run(c, 15);                // just above the floor
    c = base; c.sp.X = -7.5f; c.ep.Y = 100.0f; c.len1 = 0.0f; c.len2 = 50.0f; Run(c, 16);
    c = base; c.hs = false; c.ds = true; c.he = false; c.de = true; c.len1 = 3.0f; c.len2 = 4.0f; Run(c, 17);
    printf(failures ? "00766660_TEST FAIL\n" : "00766660_TEST PASS\n");
    return failures ? 1 : 0;
}