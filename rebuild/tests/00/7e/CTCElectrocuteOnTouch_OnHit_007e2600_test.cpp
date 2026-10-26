#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __stdcall
#define __cdecl
#endif
#include <cstdio>
#include <cstring>
class CThing;
class CTCBase;
class CTCParamsBase;
namespace NHitLocations { class CHitLocation; }

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x22];
    bool MoveBack;                                  // +0x22
    unsigned char _pad23[0x60 - 0x23];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0 (EDamageAttribute)
};

class CTCElectrocutedEffect {
public:
    void Reset();                                   // 0x007CE7A0
};

class CThing {
public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004();
    virtual void v005(); virtual void v006(); virtual void v007(); virtual void v008(); virtual void v009();
    virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014();
    virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019();
    virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023(); virtual void v024();
    virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029();
    virtual void v030(); virtual void v031(); virtual void v032(); virtual void v033(); virtual void v034();
    virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044();
    virtual void v045(); virtual void v046(); virtual void v047(); virtual void v048(); virtual void v049();
    virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054();
    virtual void v055(); virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059();
    virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063(); virtual void v064();
    virtual void OnHit(CHitParameters& hit);        // slot 65 (+0x104)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long UsingComponents[9];               // +0x20 bitset<274>
    unsigned char _pad44[0x6c - 0x44];
    unsigned long ThingTypeFlags;                   // +0x6C
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPosition() const;           // 0x004C73D0
};

class CTCElectrocuteOnTouch {
public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04 (CTCBase)
    void OnHit(CThing& thing);
};

void CTCElectrocuteOnTouch::OnHit(CThing& thing)
{
    if (!(thing.UsingComponents[0x8a >> 5] & (1UL << (0x8a & 31)))) {
        CTCElectrocutedEffect* effect;
        {
            CCharString name("CTCElectrocutedEffect", -1);
            effect = (CTCElectrocutedEffect*)thing.AddTC(name, true, 0);
        }
        effect->Reset();
    }
    const C3DVector& from = ParentThing->GetPosition();
    const C3DVector& to = thing.GetPosition();
    CThing* owner = ParentThing;
    C3DVector direction;
    direction.X = to.X - from.X;
    direction.Y = to.Y - from.Y;
    direction.Z = to.Z - from.Z;
    CHitParameters hit(0, owner, 0, thing.GetPosition(), direction, true, false, false, false);
    hit.AddDamageWithAttribute(2, 1.0f);
    hit.MoveBack = false;
    if (thing.ThingTypeFlags & 4)
        thing.OnHit(hit);
}

// ---- test stubs for the retail callees ----
static const char* g_addedName; static bool g_addedActivate; static int g_resets, g_hits, g_dtors;
static float g_damage; static long g_attribute; static C3DVector g_dir, g_pos; static CThing* g_hitter; static bool g_knock;
static CTCElectrocutedEffect g_effect;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) {}
CCharString::~CCharString() {}
CHitParameters::CHitParameters(NHitLocations::CHitLocation*, CThing* thingHitting, CThing* weapon, const C3DVector& position,
    const C3DVector& direction, bool knockDown, bool blockable, bool flourish, bool blockCounter)
{ MoveBack = true; g_hitter = thingHitting; g_pos = position; g_dir = direction; g_knock = knockDown && !blockable && !flourish && !blockCounter && !weapon; }
CHitParameters::~CHitParameters() { ++g_dtors; }
void CHitParameters::AddDamageWithAttribute(long attribute, float damage) { g_attribute = attribute; g_damage = damage; }
void CTCElectrocutedEffect::Reset() { ++g_resets; }
struct CTestThing : CThing {
    C3DVector Pos; bool Got; bool GotMoveBack;
    virtual void OnHit(CHitParameters& hit) { Got = true; GotMoveBack = hit.MoveBack; }
};
CTCBase* CThing::AddTC(const CCharString& name, bool activate, const CTCParamsBase*)
{ g_addedName = (const char*)name.PStringData; g_addedActivate = activate; return (CTCBase*)&g_effect; }
const C3DVector& CThing::GetPosition() const { return ((const CTestThing*)this)->Pos; }
#define V(n) void CThing::n() {}
V(v000) V(v001) V(v002) V(v003) V(v004) V(v005) V(v006) V(v007) V(v008) V(v009) V(v010) V(v011) V(v012) V(v013) V(v014)
V(v015) V(v016) V(v017) V(v018) V(v019) V(v020) V(v021) V(v022) V(v023) V(v024) V(v025) V(v026) V(v027) V(v028) V(v029)
V(v030) V(v031) V(v032) V(v033) V(v034) V(v035) V(v036) V(v037) V(v038) V(v039) V(v040) V(v041) V(v042) V(v043) V(v044)
V(v045) V(v046) V(v047) V(v048) V(v049) V(v050) V(v051) V(v052) V(v053) V(v054) V(v055) V(v056) V(v057) V(v058) V(v059)
V(v060) V(v061) V(v062) V(v063) V(v064)
void CThing::OnHit(CHitParameters&) {}
void CTCElectrocuteOnTouch::v00() {}

static void Init(CTestThing& t, float x, float y, float z, unsigned long flags)
{
    for (int i = 0; i < 9; ++i) t.UsingComponents[i] = 0;
    t.ThingTypeFlags = flags; t.Pos.X = x; t.Pos.Y = y; t.Pos.Z = z; t.Got = false; t.GotMoveBack = true;
}

int main()
{
    int ok = 1;
    CTestThing owner, victim;
    Init(owner, 1.0f, 2.0f, 3.0f, 0); Init(victim, 4.0f, 6.0f, 11.0f, 4);
    CTCElectrocuteOnTouch tc; tc.ParentThing = &owner;
    tc.OnHit(victim);
    if (g_resets != 1 || std::strcmp(g_addedName, "CTCElectrocutedEffect") != 0 || !g_addedActivate) ok = 0;
    if (g_dir.X != 3.0f || g_dir.Y != 4.0f || g_dir.Z != 8.0f || g_pos.X != 4.0f || g_hitter != &owner || !g_knock) ok = 0;
    if (g_attribute != 2 || g_damage != 1.0f || !victim.Got || victim.GotMoveBack || g_dtors != 1) ok = 0;
    // already electrocuted, not a game object: no new effect, no OnHit
    victim.UsingComponents[4] = 1u << 10; victim.ThingTypeFlags = 0; victim.Got = false;
    tc.OnHit(victim);
    if (g_resets != 1 || victim.Got || g_dtors != 2) ok = 0;
    if (!ok) { std::printf("007e2600_TEST FAIL\n"); return 1; }
    std::printf("007e2600_TEST PASS\n");
    return 0;
}