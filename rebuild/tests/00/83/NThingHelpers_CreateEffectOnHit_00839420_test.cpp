// Behaviour test for NThingHelpers::CreateEffectOnHit (0x00839420): the same statement text over small models of the engine classes.
#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
#include <utility>
static bool g_failed;
#define CHECK(c) do { if (!(c)) { g_failed = true; std::printf("CHECK failed line %d\n", __LINE__); } } while (0)
typedef std::string CCharString;
const float DELTA_NOTIONAL_ZERO = 1e-4f;
class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    const C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    const C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    const C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    float GetSquaredMagnitude() const { return X * X + Z * Z + Y * Y; }
    bool IsNonZero() const { return GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO * DELTA_NOTIONAL_ZERO; }
    C3DVector& AccurateNormalise() { float m = (float)std::sqrt(GetSquaredMagnitude()); X /= m; Y /= m; Z /= m; return *this; }
    C3DVector CrossWithUp() const { return C3DVector(Y, -X, 0.0f); }
};
inline float GFSqr(float n) { return n * n; }
template <class T> class CDefPointer { public:
    T* p;
    CDefPointer() : p(0) {}
    CDefPointer(T* q) : p(q) {}
    bool IsNull() const { return p == 0; }
    T* Get() const { return p; }
    T* operator->() const { return p; }
};
class CDefClassBase { public: CCharString Name; const CCharString& GetInstantiationName() const { return Name; } };
class CRegionDef : public CDefClassBase {};
class CCreatureDef { public: bool UseActualHitPosForHitEffects; };
class CMaterialDef { public:
    bool blended; long from, to; float blend; long berserked, plain; mutable long seen_region; mutable float seen_health;
    bool GetBlendedSwordHitEffect(long region_def_index, float delta_health, long& from_effect_id, long& to_effect_id, float& interpolate) const
    { seen_region = region_def_index; seen_health = delta_health; if (!blended) return false; from_effect_id = from; to_effect_id = to; interpolate = blend; return true; }
    long GetRandomBerserkedSwordHitParticleEffect() const { return berserked; }
    long GetRandomSwordHitParticleEffect() const { return plain; }
};
class CThingBaseDef { public:
    float RenderFadeDistance; const CCreatureDef* creature;
    bool GetSubDef(CDefPointer<const CCreatureDef>* out) const { if (!creature) return false; out->p = creature; return true; }
};
enum ETCInterfaceType { TCI_PARTICLE_EMITTER = 7, TCI_BERSERK = 132 };
class CTCPhysicsBase { public: float Radius; float GetRadius() const { return Radius; } };
namespace NParticleEngine { template <class K, class V> class CParticleUpdateParameters { public: std::vector<std::pair<K, C3DVector> > VectorParams; }; }
class CTCDParticleEmitter;
class CThing { public:
    bool dead; unsigned long TypeFlags; C3DVector Pos; const CThingBaseDef* PBaseDef; CTCPhysicsBase Physics; bool berserk; CTCDParticleEmitter* driver;
    CThing() : dead(false), TypeFlags(0), PBaseDef(0), berserk(false), driver(0) {}
    virtual ~CThing() {}
    bool IsDead() const { return dead; }
    unsigned long GetTypeFlags() const { return TypeFlags; }
    const C3DVector& GetPos() const { return Pos; }
    CDefPointer<const CThingBaseDef> GetPBaseDef() const { return CDefPointer<const CThingBaseDef>(PBaseDef); }
    const CTCPhysicsBase* GetPhysicsTC() const { return &Physics; }
    bool HasTC(ETCInterfaceType type) const { return type == TCI_BERSERK && berserk; }
    bool GetTCDriver(CTCDParticleEmitter** out) { *out = driver; return driver != 0; }
};
class CThingCreatureBase : public CThing { public: long handedness; long GetLastActionHandedness() const { return handedness; } };
template <class From, class To> inline To GFDebugPDynamicCast(From value, To) { return static_cast<To>(value); }
template <class T> inline bool IS_THING_ALIVE(const T& thing) { if (thing != 0) { if (!thing->IsDead()) return true; } return false; }
class CHitParameters { public:
    C3DVector Position, Direction; bool CreateParticleEffectOnHit; const CMaterialDef* Material; CThing* Hitter; bool melee;
    const C3DVector& GetHitPosition() const { return Position; }
    const C3DVector& GetHitDirection() const { return Direction; }
    CThing* GetPThingHitting() const { return Hitter; }
    bool IsCreateParticleEffectOnHit() const { return CreateParticleEffectOnHit; }
    CDefPointer<const CMaterialDef> GetPHitMaterialDef() const { return CDefPointer<const CMaterialDef>(Material); }
    bool IsMeleeStrike() const { return melee; }
};
class CRegion { public: const CRegionDef* PDef; CDefPointer<const CRegionDef> GetPRegionDef() const { return CDefPointer<const CRegionDef>(PDef); } };
class CWorldMap { public:
    bool in_map; CRegion region; mutable C3DVector asked;
    bool IsPosInMap(const C3DVector& pos) const { asked = pos; return in_map; }
    long GetLoadedRegion() const { return 3; }
    const CRegion& GetRegion(long index) const { CHECK(index == 3); return region; }
};
class CWorld { public: C3DVector lod; virtual C3DVector GetGameLODCentre() const { return lod; } };
class CGameDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name) const { return name == "REGION_OAKVALE" ? 77 : -1; } };
class CMainGameComponent { public:
    CWorld world; CGameDefinitionManager defs;
    const CWorld& PeekWorld() const { return world; }
    const CGameDefinitionManager& PeekDefinitionManager() const { return defs; }
    static CMainGameComponent* Get() { return PMainGameComponent; }
    static CMainGameComponent* PMainGameComponent;
};
CMainGameComponent* CMainGameComponent::PMainGameComponent;
class CCRC { public: static unsigned long Calc(unsigned long seed, const void* data, unsigned long length) { unsigned long h = seed; for (unsigned long i = 0; i < length; ++i) h = h * 31 + ((const unsigned char*)data)[i]; return h; } };
struct SCreated { int kind; long a, b; float blend; C3DVector pos; bool flag; };
static std::vector<SCreated> g_created;
static CThing g_emitter_thing;
class CTCDParticleEmitter { public:
    std::vector<std::pair<unsigned long, C3DVector> > got;
    static CThing* Create(long effect, long low_effect, float blend, const C3DVector& pos, bool flag) { SCreated c = { 5, effect, low_effect, blend, pos, flag }; g_created.push_back(c); return &g_emitter_thing; }
    static CThing* Create(long effect, const C3DVector& pos, bool flag) { SCreated c = { 3, effect, 0, 0.0f, pos, flag }; g_created.push_back(c); return &g_emitter_thing; }
    void SetEmitterUpdateParameters(const NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>& parameters) { got = parameters.VectorParams; }
};
namespace NGlobalConsole { bool CombatParticleEffects = true; }
static bool g_to_create = true;
static const CThing* g_asked_hitter;
namespace NCombatActionHelpers { bool IsToCreateCombatParticleEffects(const CThing&, const CThing* hitter) { g_asked_hitter = hitter; return g_to_create; } }
#define __fastcall
namespace NThingHelpers {
void __fastcall CreateEffectOnHit(const CThing& thing_hit, const CHitParameters& hit_params, const CWorldMap& world_map, float delta_health)
{
    const CThing* pthing_hitting = hit_params.GetPThingHitting();
    if (!NCombatActionHelpers::IsToCreateCombatParticleEffects(thing_hit, pthing_hitting))
    {
        return;
    }

    C3DVector point_of_hit = hit_params.GetHitPosition();

    if (!NGlobalConsole::CombatParticleEffects)
    {
        return;
    }
    if (!hit_params.IsCreateParticleEffectOnHit())
    {
        return;
    }

    C3DVector attack_direction = hit_params.GetHitDirection();
    CDefPointer<const CMaterialDef> pmaterial_def = hit_params.GetPHitMaterialDef();

    if (IS_THING_ALIVE(pthing_hitting) && (thing_hit.GetTypeFlags() & 8) && hit_params.IsMeleeStrike())
    {
        CDefPointer<const CCreatureDef> pcreature_def;
        if (thing_hit.GetPBaseDef()->GetSubDef(&pcreature_def) && !pcreature_def->UseActualHitPosForHitEffects)
        {
            C3DVector line_out = pthing_hitting->GetPos() - thing_hit.GetPos();
            line_out.Z = 0.0f;
            if (line_out.IsNonZero())
            {
                line_out.AccurateNormalise();

                C3DVector new_point_of_hit = thing_hit.GetPos() + line_out * thing_hit.GetPhysicsTC()->GetRadius() * 0.75f;
                new_point_of_hit.Z = point_of_hit.Z;

                C3DVector new_direction_of_hit = attack_direction;
                if (pthing_hitting->GetTypeFlags() & 8)
                {
                    C3DVector perp = line_out.CrossWithUp();
                    const CThingCreatureBase* phitter = GFDebugPDynamicCast(pthing_hitting, (const CThingCreatureBase*)0);
                    if (phitter->GetLastActionHandedness() == 0)
                    {
                        new_direction_of_hit = perp * -1.0f;
                    }
                    else if (phitter->GetLastActionHandedness() == 1)
                    {
                        new_direction_of_hit = perp * 1.0f;
                    }
                }

                point_of_hit = new_point_of_hit;
                attack_direction = new_direction_of_hit;
            }
        }
    }

    if (!world_map.IsPosInMap(point_of_hit))
    {
        return;
    }

    if (!thing_hit.GetPBaseDef().IsNull())
    {
        C3DVector lod_centre = CMainGameComponent::Get()->PeekWorld().GetGameLODCentre();
        if ((point_of_hit - lod_centre).GetSquaredMagnitude() > GFSqr(thing_hit.GetPBaseDef()->RenderFadeDistance))
        {
            return;
        }
    }

    if (!pmaterial_def.IsNull())
    {
        const CGameDefinitionManager& def_man = CMainGameComponent::Get()->PeekDefinitionManager();
        const CRegion& region = world_map.GetRegion(world_map.GetLoadedRegion());
        const CRegionDef* pregion_def = region.GetPRegionDef().Get();
        long region_def_index = 0;
        if (pregion_def)
        {
            region_def_index = def_man.GetDefGlobalIndexFromName(pregion_def->GetInstantiationName());
        }

        CThing* pthing_emitter = 0;
        long from_effect_id = 0;
        long to_effect_id = 0;
        float interpolate = 0.0f;

        if (pmaterial_def->GetBlendedSwordHitEffect(region_def_index, delta_health, from_effect_id, to_effect_id, interpolate))
        {
            if (from_effect_id && to_effect_id)
            {
                pthing_emitter = CTCDParticleEmitter::Create(from_effect_id, to_effect_id, interpolate, point_of_hit, false);
            }
        }
        else
        {
            if (IS_THING_ALIVE(pthing_hitting) && pthing_hitting->HasTC(TCI_BERSERK))
            {
                from_effect_id = pmaterial_def->GetRandomBerserkedSwordHitParticleEffect();
            }
            else
            {
                from_effect_id = pmaterial_def->GetRandomSwordHitParticleEffect();
            }
            if (from_effect_id > 0)
            {
                pthing_emitter = CTCDParticleEmitter::Create(from_effect_id, point_of_hit, false);
            }
        }

        if (IS_THING_ALIVE(pthing_emitter))
        {
            CTCDParticleEmitter* tcd_particle_emitter = 0;
            pthing_emitter->GetTCDriver(&tcd_particle_emitter);
            std::pair<unsigned long, C3DVector> parameter(CCRC::Calc(0, "DIRECTION", 9), attack_direction);
            NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long> params;
            params.VectorParams.push_back(parameter);
            tcd_particle_emitter->SetEmitterUpdateParameters(params);
        }
    }
}
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    CMainGameComponent game; CMainGameComponent::PMainGameComponent = &game; game.world.lod = C3DVector(0.0f, 0.0f, 0.0f);
    CTCDParticleEmitter emitter; g_emitter_thing.driver = &emitter;
    CCreatureDef creature_def; creature_def.UseActualHitPosForHitEffects = false;
    CThingBaseDef base_def; base_def.RenderFadeDistance = 100.0f; base_def.creature = &creature_def;
    CMaterialDef material; material.blended = false; material.from = 11; material.to = 12; material.blend = 0.25f; material.berserked = 21; material.plain = 22;
    CRegionDef region_def; region_def.Name = "REGION_OAKVALE";
    CWorldMap map; map.in_map = true; map.region.PDef = &region_def;
    CThingCreatureBase victim; victim.TypeFlags = 8; victim.Pos = C3DVector(10.0f, 10.0f, 1.0f); victim.PBaseDef = &base_def; victim.Physics.Radius = 2.0f;
    CThingCreatureBase hitter; hitter.TypeFlags = 8; hitter.Pos = C3DVector(14.0f, 10.0f, 5.0f); hitter.handedness = 0;
    CHitParameters hit; hit.Position = C3DVector(11.0f, 11.0f, 3.0f); hit.Direction = C3DVector(1.0f, 0.0f, 0.0f); hit.CreateParticleEffectOnHit = true;
    hit.Material = &material; hit.Hitter = &hitter; hit.melee = true;
    const unsigned long key = CCRC::Calc(0, "DIRECTION", 9);

    // 1: refused by IsToCreateCombatParticleEffects, by the console switch, by the hit's own flag
    g_to_create = false; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); CHECK(g_asked_hitter == &hitter); g_to_create = true;
    NGlobalConsole::CombatParticleEffects = false; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); NGlobalConsole::CombatParticleEffects = true;
    hit.CreateParticleEffectOnHit = false; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); hit.CreateParticleEffectOnHit = true;

    // 2: melee on a creature that does not use the actual hit position: 0.75 x radius towards the striker, Z kept, direction = perp * -1 (hand 0)
    NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(g_created.size() == 1);
    if (g_created.size() == 1) {
        CHECK(g_created[0].kind == 3); CHECK(g_created[0].a == 22); CHECK(!g_created[0].flag);
        CHECK(Near(g_created[0].pos.X, 11.5f)); CHECK(Near(g_created[0].pos.Y, 10.0f)); CHECK(Near(g_created[0].pos.Z, 3.0f));
    }
    CHECK(material.seen_region == 77); CHECK(Near(material.seen_health, 0.5f));
    CHECK(emitter.got.size() == 1);
    if (emitter.got.size() == 1) { CHECK(emitter.got[0].first == key); CHECK(Near(emitter.got[0].second.X, 0.0f)); CHECK(Near(emitter.got[0].second.Y, 1.0f)); CHECK(Near(emitter.got[0].second.Z, 0.0f)); }

    // 3: the other hand flips the direction; a third value keeps the attack direction
    g_created.clear(); hitter.handedness = 1; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(emitter.got.size() == 1 && Near(emitter.got[0].second.Y, -1.0f));
    g_created.clear(); hitter.handedness = 2; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(emitter.got.size() == 1 && Near(emitter.got[0].second.X, 1.0f) && Near(emitter.got[0].second.Y, 0.0f));
    CHECK(g_created.size() == 1 && Near(g_created[0].pos.X, 11.5f));

    // 4: a def that uses the actual hit position, or a non-melee strike, keeps the hit position and direction
    g_created.clear(); creature_def.UseActualHitPosForHitEffects = true; hitter.handedness = 0; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(g_created.size() == 1 && Near(g_created[0].pos.X, 11.0f) && Near(g_created[0].pos.Y, 11.0f));
    CHECK(emitter.got.size() == 1 && Near(emitter.got[0].second.X, 1.0f));
    creature_def.UseActualHitPosForHitEffects = false;
    g_created.clear(); hit.melee = false; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(g_created.size() == 1 && Near(g_created[0].pos.X, 11.0f)); hit.melee = true;

    // 5: outside the map, or beyond the render fade distance of the LOD centre: nothing
    g_created.clear(); map.in_map = false; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); map.in_map = true;
    CHECK(Near(map.asked.X, 11.5f));
    game.world.lod = C3DVector(500.0f, 0.0f, 0.0f); NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); game.world.lod = C3DVector(0.0f, 0.0f, 0.0f);

    // 6: the blended pair wins when both ids are set; none is created when one is zero
    material.blended = true; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(g_created.size() == 1);
    if (g_created.size() == 1) { CHECK(g_created[0].kind == 5); CHECK(g_created[0].a == 11); CHECK(g_created[0].b == 12); CHECK(Near(g_created[0].blend, 0.25f)); }
    g_created.clear(); material.to = 0; emitter.got.clear(); NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); CHECK(emitter.got.empty());
    material.to = 12; material.blended = false;

    // 7: a live striker with TC 132 picks the berserked list; a non-positive id creates nothing; no material: nothing
    hitter.berserk = true; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.size() == 1 && g_created[0].a == 21); hitter.berserk = false;
    g_created.clear(); material.plain = 0; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); material.plain = 22;
    hit.Material = 0; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(g_created.empty()); hit.Material = &material;

    // 8: no region def gives region index 0; a dead striker is not used for the melee shift
    map.region.PDef = 0; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f); CHECK(material.seen_region == 0); map.region.PDef = &region_def;
    g_created.clear(); hitter.dead = true; NThingHelpers::CreateEffectOnHit(victim, hit, map, 0.5f);
    CHECK(g_created.size() == 1 && Near(g_created[0].pos.X, 11.0f));

    if (!g_failed) std::printf("00839420_TEST PASS\n");
    return g_failed ? 1 : 0;
}