// Behaviour test for 0080ca20: src/0080ca20.cpp copied verbatim (shared headers expanded inline), callees stubbed below so calls
// and arguments are observable. CTCDExplosion::GiveDamageToThing(CThingGameObject& thing)
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCDExplosion::GiveDamageToThing(CThingGameObject& thing) -- hit a thing caught in the explosion: unless it evades all hits, strike it from the closest point of the blast segment with the max damage (scaled down with distance when the def says so), remember it as hit, and set burnable things on fire
// Leaf: GiveDamageToThing
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0080CA20, 464 bytes (`ret 4`), no EH frame.
//         Ego_r.pdb: `?GiveDamageToThing@CTCDExplosion@@AAEXAAVCThingGameObject@@@Z`.
// Callees: NCreatureMode::IsToEvadeAllHits(const CThing&) __fastcall @ 0x0085FA00, CThing::GetPosition @ 0x004C73D0,
//          Geometry_ClosestPointOnLineSegment __fastcall @ 0x008F5970 (hidden result in ecx, start in edx, end + point
//          on the stack), CHitParameters ctor (9 args) @ 0x00789238 / dtor @ 0x00789358, AddDamageWithAttribute @
//          0x00868CA0, Geometry_DistancePointToLineSegment __fastcall @ 0x008F5BA0 (inline
//          GetDistanceFromMeAsRatioOfDamageRadius, landed 0x0080C210), the thing's virtual OnHit (+0x104),
//          CBaseIntelligentPointer ctor/SetPItem/dtor @ 0x00A01B10/0x00A01B90/0x00A01C10 (CIntelligentPointer<CThing>
//          vtable 0x01238C6C), vector<CIntelligentPointer<CThing>>::push_back @ 0x004AD520, CInterfaceMap::LowerBound @
//          0x0040F020, CTCBurntEffect::Reset @ 0x007CDD10, CThing::AddTC @ 0x004C9D60, CCharString @ 0x0099EBF0 /
//          0x0099EAE0 ("CTCBurntEffect" @ 0x0123D384).
// Shape: the hit direction is an explicit C3DVector(x, y, z) temporary in the CHitParameters argument list (C3DVector has
//        the PDB's user copy constructor): it computes X first and its frame words are reused by the damage float and the
//        intelligent-pointer temporary; an inline `operator-` returning `C3DVector(...)` computes Z first, and a named
//        direction local keeps its own frame words (`sub esp,80h`). The burn test is the raw bitset word (`test ah,2`), the
//        lookup the inline GetTC, the class name a named CCharString in its own block.
// Layout: CTCDExplosion (Ego_r.pdb) with retail 12-byte vectors: DamageRadius +0x0c, MaxDamage +0x14, StartPos +0x20,
//         EndPos +0x2c, PExplosionDef +0x38, Emitters +0x40, HitThings +0x54 (PDB +0x58). CExplosionDef:
//         DamageFalloffWithRadius +0x38, CauseFireEffectOnHitCreatures +0x65 (PDB +0x69, ExplosionRings 12 bytes in
//         retail). Retail CThing: interface bitset +0x20 (TC 0x89 = CTCBurntEffect: word +0x30 bit 9), map +0x44,
//         ThingTypeFlags +0x6C (bit 3 tested here).
class CTCBase;
class CTCParamsBase;
class CBaseObject;
class CThing;
namespace NHitLocations { class CHitLocation; }

struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length);     // 0x0099EBF0
    ~CCharString();                                 // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}   // ??0C3DVector@@QAE@ABV0@@Z (Ego_r.pdb)
};

class CHitParameters {                              // 0x60 bytes in retail
public:
    unsigned char _pad00[0x60];
    CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                   const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                   bool flourish, bool blockCounter);                       // 0x00789238
    ~CHitParameters();                              // 0x00789358
    void AddDamageWithAttribute(long attribute, float damage);             // 0x00868CA0 (EDamageAttribute)
};

class CTCBurntEffect {
public:
    void Reset();                                   // 0x007CDD10
};

struct CInterfaceMap {                              // CVectorMap<ETCInterfaceType, CTCBase*>: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);           // 0x0040F020
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
    CInterfaceMap Interfaces;                       // +0x44
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;                   // +0x6C
    CTCBase* AddTC(const CCharString& name, bool activate, const CTCParamsBase* params);   // 0x004C9D60
    const C3DVector& GetPosition() const;           // 0x004C73D0
    __forceinline unsigned long HasTC(long type) const { return UsingComponents[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &Interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return (T*)it->component;
    }
};

class CThingGameObject : public CThing {
};

namespace NCreatureMode {
    bool __fastcall IsToEvadeAllHits(const CThing& thing);                  // 0x0085FA00
}

C3DVector __fastcall Geometry_ClosestPointOnLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                        const C3DVector& point);   // 0x008F5970
float __fastcall Geometry_DistancePointToLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd,
                                                     const C3DVector& point);   // 0x008F5BA0

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    void SetPItem(CBaseObject* item);               // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
template <class T> struct CRetailVector {           // STLport vector (12 bytes)
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    void push_back(const T& value);                 // 0x004AD520
};

struct CExplosionDef {
    unsigned char _pad00[0x38];
    bool DamageFalloffWithRadius;                   // +0x38
    unsigned char _pad39[0x65 - 0x39];
    bool CauseFireEffectOnHitCreatures;             // +0x65
};

class CTCDExplosion {
public:
    void* __vftable;                                // +0x00 (CTCBase)
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    float DamageRadius;                             // +0x0c
    float PhysicsRadius;                            // +0x10
    float MaxDamage;                                // +0x14
    long FireDamage;                                // +0x18
    long ExplosionLifeTimer;                        // +0x1c
    C3DVector StartPos;                             // +0x20
    C3DVector EndPos;                               // +0x2c
    const CExplosionDef* PExplosionDef;             // +0x38 (CDefPointer object word)
    bool CreatedEmitter;                            // +0x3c
    bool NeedToTriggerExplosion;                    // +0x3d
    unsigned char _pad3e[0x40 - 0x3e];
    unsigned char Emitters[12];                     // +0x40
    long ExplosionEvolutionTimer;                   // +0x4c
    long CurrentExplosionRing;                      // +0x50
    CRetailVector<CIntelligentPointer<CThing> > HitThings;   // +0x54
    float GetDistanceFromMeAsRatioOfDamageRadius(const CThing* thing) const
    {
        return Geometry_DistancePointToLineSegment(StartPos, EndPos, thing->GetPosition()) / DamageRadius;
    }
    void GiveDamageToThing(CThingGameObject& thing);
};

void CTCDExplosion::GiveDamageToThing(CThingGameObject& thing)
{
    if (NCreatureMode::IsToEvadeAllHits(thing))
        return;
    C3DVector closest = Geometry_ClosestPointOnLineSegment(StartPos, EndPos, thing.GetPosition());
    const C3DVector& position = thing.GetPosition();
    CHitParameters hit(0, ParentThing, 0, closest,
                       C3DVector(position.X - closest.X, position.Y - closest.Y, position.Z - closest.Z),
                       false, false, false, false);
    float damage = MaxDamage;
    if (PExplosionDef->DamageFalloffWithRadius) {
        float ratio = GetDistanceFromMeAsRatioOfDamageRadius(&thing);
        if (ratio < 0.0f)
            ratio = 0.0f;
        else if (ratio > 1.0f)
            ratio = 1.0f;
        damage *= 1.0f - ratio * 0.95f;
    }
    hit.AddDamageWithAttribute(5, damage);
    thing.OnHit(hit);
    HitThings.push_back(CIntelligentPointer<CThing>(&thing));
    if (PExplosionDef->CauseFireEffectOnHitCreatures && (thing.ThingTypeFlags & 8)) {
        if (thing.HasTC(0x89))
            thing.GetTC<CTCBurntEffect>(0x89)->Reset();
        else {
            CCharString name("CTCBurntEffect", -1);
            thing.AddTC(name, true, 0);
        }
    }
}

// ---- stubs and driver ----
// stubs: record every observable call; the geometry helpers return scripted values
static int g_strCtor = 0, g_strDtor = 0, g_hitCtor = 0, g_hitDtor = 0, g_onHit = 0, g_resets = 0, g_addTC = 0;
static int g_pushes = 0, g_ptrCtor = 0, g_ptrDtor = 0, g_lookups = 0, g_evadeCalls = 0;
static bool g_evade = false; static float g_distance = 0.0f;
static C3DVector g_closest; static C3DVector g_hitPos, g_hitDir; static CThing* g_hitBy = 0; static long g_attr = -1;
static float g_damage = -1.0f; static const CHitParameters* g_lastHit = 0; static const CHitParameters* g_onHitArg = 0;
static CThing* g_pushed = 0; static const char* g_tcName = 0; static bool g_tcActivate = false; static const CTCParamsBase* g_tcParams = (const CTCParamsBase*)1;
static CTCBurntEffect* g_resetThis = 0; static long g_lookupKey = 0;
CCharString::CCharString(const char* text, long length) { ++g_strCtor; PStringData = (void*)text; CHECK(length == -1); }
CCharString::~CCharString() { ++g_strDtor; }
CHitParameters::CHitParameters(NHitLocations::CHitLocation* hitLocation, CThing* thingHitting, CThing* weapon,
                               const C3DVector& position, const C3DVector& direction, bool knockDown, bool blockable,
                               bool flourish, bool blockCounter)
{
    ++g_hitCtor; g_lastHit = this; g_hitBy = thingHitting; g_hitPos = position; g_hitDir = direction;
    CHECK(hitLocation == 0 && weapon == 0 && !knockDown && !blockable && !flourish && !blockCounter);
}
CHitParameters::~CHitParameters() { ++g_hitDtor; CHECK(this == g_lastHit); }
void CHitParameters::AddDamageWithAttribute(long attribute, float damage) { CHECK(this == g_lastHit); g_attr = attribute; g_damage = damage; }
void CTCBurntEffect::Reset() { ++g_resets; g_resetThis = this; }
static CInterfaceMap::SEntry g_entries[3];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    ++g_lookups; g_lookupKey = *type;
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
#define V(n) void CThing::v##n() { }
V(000) V(001) V(002) V(003) V(004) V(005) V(006) V(007) V(008) V(009) V(010) V(011) V(012) V(013) V(014) V(015) V(016)
V(017) V(018) V(019) V(020) V(021) V(022) V(023) V(024) V(025) V(026) V(027) V(028) V(029) V(030) V(031) V(032) V(033)
V(034) V(035) V(036) V(037) V(038) V(039) V(040) V(041) V(042) V(043) V(044) V(045) V(046) V(047) V(048) V(049) V(050)
V(051) V(052) V(053) V(054) V(055) V(056) V(057) V(058) V(059) V(060) V(061) V(062) V(063) V(064)
void CThing::OnHit(CHitParameters& hit) { ++g_onHit; g_onHitArg = &hit; }
static C3DVector g_thingPos;
const C3DVector& CThing::GetPosition() const { return g_thingPos; }
CTCBase* CThing::AddTC(const CCharString& name, bool activate, const CTCParamsBase* params)
{ ++g_addTC; g_tcName = (const char*)name.PStringData; g_tcActivate = activate; g_tcParams = params; return 0; }
bool __fastcall NCreatureMode::IsToEvadeAllHits(const CThing& thing) { ++g_evadeCalls; return g_evade; }
static const C3DVector* g_segStart = 0; static const C3DVector* g_segEnd = 0;
C3DVector __fastcall Geometry_ClosestPointOnLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point)
{ g_segStart = &lineStart; g_segEnd = &lineEnd; CHECK(&point == &g_thingPos); return g_closest; }
float __fastcall Geometry_DistancePointToLineSegment(const C3DVector& lineStart, const C3DVector& lineEnd, const C3DVector& point)
{ CHECK(&lineStart == g_segStart && &lineEnd == g_segEnd && &point == &g_thingPos); return g_distance; }
CBaseIntelligentPointer::CBaseIntelligentPointer() : m_pItem(0) { ++g_ptrCtor; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_ptrDtor; }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { m_pItem = (CThing*)item; }
void CBaseIntelligentPointer::v00() { }
template <> void CIntelligentPointer<CThing>::v00() { }
template <> void CRetailVector<CIntelligentPointer<CThing> >::push_back(const CIntelligentPointer<CThing>& value)
{ ++g_pushes; g_pushed = value.m_pItem; }
static void Reset()
{
    g_strCtor = g_strDtor = g_hitCtor = g_hitDtor = g_onHit = g_resets = g_addTC = g_pushes = g_ptrCtor = g_ptrDtor = 0;
    g_lookups = g_evadeCalls = 0; g_attr = -1; g_damage = -1.0f; g_pushed = 0; g_tcName = 0; g_resetThis = 0; g_lookupKey = 0;
}
static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }
int main()
{
    static CExplosionDef def;
    static CTCDExplosion e;
    static CThingGameObject thing;
    static CThing parent;
    static unsigned char burnt[4];
    e.ParentThing = &parent; e.DamageRadius = 4.0f; e.MaxDamage = 100.0f; e.PExplosionDef = &def;
    g_thingPos.X = 5.0f; g_thingPos.Y = 7.0f; g_thingPos.Z = 9.0f;
    g_closest.X = 1.0f; g_closest.Y = 2.0f; g_closest.Z = 3.0f;
    thing.Interfaces.first = g_entries; thing.Interfaces.last = g_entries + 3;
    g_entries[0].type = 0x10; g_entries[1].type = 0x89; g_entries[1].component = (CTCBase*)burnt; g_entries[2].type = 0x90;

    // evades every hit: nothing happens
    Reset(); g_evade = true;
    e.GiveDamageToThing(thing);
    CHECK(g_evadeCalls == 1 && g_hitCtor == 0 && g_onHit == 0 && g_pushes == 0);
    g_evade = false;

    // plain hit: full damage, attribute 5, hit from the closest point along (thing - closest), remembered
    Reset(); def.DamageFalloffWithRadius = false; def.CauseFireEffectOnHitCreatures = false;
    e.GiveDamageToThing(thing);
    CHECK(g_hitCtor == 1 && g_hitDtor == 1 && g_onHit == 1 && g_onHitArg == g_lastHit && g_hitBy == &parent);
    CHECK(g_segStart == &e.StartPos && g_segEnd == &e.EndPos);
    CHECK(g_hitPos.X == 1.0f && g_hitPos.Y == 2.0f && g_hitPos.Z == 3.0f);
    CHECK(g_hitDir.X == 4.0f && g_hitDir.Y == 5.0f && g_hitDir.Z == 6.0f);
    CHECK(g_attr == 5 && g_damage == 100.0f);
    CHECK(g_pushes == 1 && g_pushed == &thing && g_ptrCtor == 1 && g_ptrDtor == 1);
    CHECK(g_resets == 0 && g_addTC == 0 && g_lookups == 0);

    // falloff: ratio = distance / radius, clamped to [0, 1], damage *= 1 - 0.95 * ratio
    def.DamageFalloffWithRadius = true;
    Reset(); g_distance = 2.0f; e.GiveDamageToThing(thing); CHECK(Near(g_damage, 100.0f * (1.0f - 0.5f * 0.95f)));
    Reset(); g_distance = 40.0f; e.GiveDamageToThing(thing); CHECK(Near(g_damage, 5.0f));
    Reset(); g_distance = -3.0f; e.GiveDamageToThing(thing); CHECK(Near(g_damage, 100.0f));
    def.DamageFalloffWithRadius = false;

    // fire: only for things with type flag 8; reset an existing burnt effect, otherwise add one
    def.CauseFireEffectOnHitCreatures = true;
    Reset(); thing.ThingTypeFlags = 0; e.GiveDamageToThing(thing);
    CHECK(g_resets == 0 && g_addTC == 0 && g_lookups == 0 && g_onHit == 1);
    Reset(); thing.ThingTypeFlags = 8; thing.UsingComponents[4] = 1UL << 9; e.GiveDamageToThing(thing);
    CHECK(g_resets == 1 && g_resetThis == (CTCBurntEffect*)burnt && g_lookups == 1 && g_lookupKey == 0x89 && g_addTC == 0);
    CHECK(g_strCtor == 0 && g_hitDtor == 1);
    Reset(); thing.UsingComponents[4] = 0; e.GiveDamageToThing(thing);
    CHECK(g_addTC == 1 && g_tcName && strcmp(g_tcName, "CTCBurntEffect") == 0 && g_tcActivate && g_tcParams == 0);
    CHECK(g_strCtor == 1 && g_strDtor == 1 && g_resets == 0 && g_lookups == 0 && g_hitDtor == 1);
    if (fails == 0) printf("0080ca20_TEST PASS\n");
    return fails;
}
