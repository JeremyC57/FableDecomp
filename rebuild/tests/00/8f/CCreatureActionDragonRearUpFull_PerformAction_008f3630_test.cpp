// Behaviour test for 0x008F3630 CCreatureAction_DragonRearUpFull::PerformAction: runs the reconstructed body (copied
// verbatim) against stub definition managers, defs, creatures, physics and shot TCs. Checks the JACK_DRAGON_GROUND_SLAM index
// lookup through the game definition manager and the shot-def fetch through the WORLD's manager, the ground probe 10 units
// above the dragon and the spawn at ground + 0.25 with the dragon's player number, PShot set to the new thing, the shot TC
// added under the def's TCName string, Init with the def (passed by value: one extra reference while Init runs), the dragon
// as shooter, no origin, target = spawn position + flat normalised facing * 10, the accuracy percentage as a float, 1.0f,
// 0.0f and an empty ability pointer, the dragon excluded from the hits, the GROUND_SLAM_EXPLOSION effect at the dragon,
// and every def reference and string temporary released. Also: no thing (nothing added, effects still looked up, the last
// shot-def reference destroys it) and an unknown effect name (no effect spawned, the effects def still released).
#include <cstdio>
#include <cstring>
#include <cmath>

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)
static bool Near(float a, float b) { return std::fabs((double)a - (double)b) < 1e-3; }

// ---- fast inverse square root (64-bit safe copy: memcpy instead of the pointer pun) ----
static unsigned long g_InvSqrtMantissaTable[128];
static void BuildTable()
{
    for (int i = 0; i < 128; ++i) {
        int e = (i & 64) ? 127 : 128;                 // bit 6 of the index is the exponent's low bit
        double m = 1.0 + ((i & 63) + 0.5) / 64.0;
        double x = std::ldexp(m, e - 127);
        double y = 1.0 / std::sqrt(x);
        int ePrime = (0x17c - e) >> 1;
        double ratio = y / std::ldexp(1.0, ePrime - 127);
        long t = (long)((ratio - 1.0) * 256.0 + 0.5);
        if (t < 0) t = 0;
        if (t > 255) t = 255;
        g_InvSqrtMantissaTable[i] = (unsigned long)t;
    }
}
static float FastInvSqrt(float lengthSquared)
{
    unsigned int bits;
    std::memcpy(&bits, &lengthSquared, 4);
    unsigned int estimate = (((0x17c - ((bits >> 23) & 0xff)) >> 1) << 23) |
                            ((unsigned int)g_InvSqrtMantissaTable[(bits >> 17) & 0x7f] << 15);
    float r;
    std::memcpy(&r, &estimate, 4);
    return 0.5f * (r * (3.0f - lengthSquared * (r * r)));
}
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void Normalise() { float r = FastInvSqrt(X * X + Y * Y + Z * Z); X *= r; Y *= r; Z *= r; }
    C3DVector operator*(float f) const { return C3DVector(X * f, Y * f, Z * f); }
    C3DVector operator+(const C3DVector& v) const { return C3DVector(X + v.X, Y + v.Y, Z + v.Z); }
};
class CRightHandedSet;

// ---- strings ----
static int g_stringsBuilt = 0;
static int g_stringsDestroyed = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* text, long length = -1) : m_pData(text) { (void)length; ++g_stringsBuilt; }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_stringsBuilt; }
    ~CCharString() { ++g_stringsDestroyed; }
};
static const char* const kStrings[] = { "TC_UNUSED", "TC_SHOT_GROUND_SLAM" };
struct CDefStringTable {
    long LastIndex;
    CCharString GetString(long index) { LastIndex = index; return CCharString(kStrings[index]); }
};
static CDefStringTable g_DefStringTable;

// ---- defs ----
static int g_defsDestroyed = 0;
struct CDefBase {
    virtual void v00() { }
    virtual void Destroy() { ++g_defsDestroyed; }
    long RefCount;
    CDefBase() : RefCount(0) { }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CWeaponDef : public CDefBase { public: };
static long g_effectTypeResult = 77;
static const char* g_lastEffectName = 0;
static long Lookup_GetEffectFromString(const void* def, const CCharString& name)
{
    (void)def;
    g_lastEffectName = name.m_pData;
    return std::strcmp(name.m_pData, "GROUND_SLAM_EXPLOSION") == 0 ? g_effectTypeResult : -1;
}
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const
    {
        long found = Lookup_GetEffectFromString(this, name);
        if (found < 0)
            return false;
        type = found;
        return true;
    }
};
class CThingShotDef : public CDefBase { public:
    long TCName;
    long Thing;
};
template <class T> struct CDefPointerByValue {
    T* m_p;
    CDefPointerByValue(T* p) : m_p(p) { ++p->RefCount; }
    CDefPointerByValue(const CDefPointerByValue& o) : m_p(o.m_p) { ++m_p->RefCount; }
    ~CDefPointerByValue() { m_p->Release(); }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    virtual void v00() { }
    virtual void Destroy() { }
};
class CMeleeCombatAbilityDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
class CDefClassBase { public:
    bool GetSubDef(CWeaponDef** out);
    bool GetSubDef(CSpecialEffectsDef** out);
};
static CWeaponDef* g_weaponDefResult = 0;
static CSpecialEffectsDef* g_effectsDefResult = 0;
static CDefClassBase* g_lastWeaponLookup = 0;
static CDefClassBase* g_lastEffectsLookup = 0;
static void* FindSubDef_CWeaponDef(CDefClassBase* def)          // the real lookup hands out a counted reference
{
    g_lastWeaponLookup = def;
    if (g_weaponDefResult)
        ++g_weaponDefResult->RefCount;
    return g_weaponDefResult;
}
static void* FindSubDef_CSpecialEffectsDef(CDefClassBase* def)
{
    g_lastEffectsLookup = def;
    if (g_effectsDefResult)
        ++g_effectsDefResult->RefCount;
    return g_effectsDefResult;
}
bool CDefClassBase::GetSubDef(CWeaponDef** out)
{
    void* found = FindSubDef_CWeaponDef(this);
    if (!found)
        return false;
    *out = (CWeaponDef*)found;
    return true;
}
bool CDefClassBase::GetSubDef(CSpecialEffectsDef** out)
{
    void* found = FindSubDef_CSpecialEffectsDef(this);
    if (!found)
        return false;
    *out = (CSpecialEffectsDef*)found;
    return true;
}
class CDefinitionManager;
static CThingShotDef g_shotDef;
static CDefinitionManager* g_lastShotManager = 0;
static long g_lastShotIndex = -1;
static void* FindDef_CThingShotDef(CDefinitionManager* manager, long index)
{
    g_lastShotManager = manager;
    g_lastShotIndex = index;
    ++g_shotDef.RefCount;
    return &g_shotDef;
}
static const char* g_lastDefName = 0;
static CDefinitionManager* g_lastIndexManager = 0;
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name)
    {
        g_lastIndexManager = this;
        g_lastDefName = name.m_pData;
        return 42;
    }
    bool GetDefShot(long index, CThingShotDef** out)
    {
        void* found = FindDef_CThingShotDef(this, index);
        if (!found)
            return false;
        *out = (CThingShotDef*)found;
        return true;
    }
    bool GetDef(long index, CThingShotDef** out) { return GetDefShot(index, out); }
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& Get();
};
static CGameDefinitionManager g_gameDefinitionManager;
CGameDefinitionManager& CGameDefinitionManager::Get() { return g_gameDefinitionManager; }

// ---- world ----
static C3DVector g_groundQuery(0.0f, 0.0f, 0.0f);
static int g_groundCalls = 0;
class CWorldMap { public:
    float GetGroundSizeZAt(const C3DVector& pos) const { g_groundQuery = pos; ++g_groundCalls; return 5.0f; }
};
class CWorld { public:
    virtual CWorldMap* GetWorldMap() { return &Map; }
    CDefinitionManager* PDefinitionManager;
    CWorldMap Map;
};

// ---- things ----
class CTCBase;
class CTCParamsBase;
class CTCPhysicsBase { public:
    C3DVector Facing;
    virtual C3DVector GetFacing() { return Facing; }
};
class CThing;
struct AddTCRecord { CThing* thing; const char* name; bool init; CTCParamsBase* params; int calls; };
static AddTCRecord g_addTC;
struct CTCShotDefinitionBase;
static CTCShotDefinitionBase* g_shotTCResult = 0;
class CThing { public:
    CTCPhysicsBase* PhysicsTC;
    CDefClassBase*  PDef;
    signed char     PlayerNumber;
    C3DVector       Position;
    const C3DVector& GetPosition() const { return Position; }
    CTCPhysicsBase* GetPhysics() const { return PhysicsTC; }
    CDefClassBase* GetDef() const { return PDef; }
    long GetPlayerNumber() const { return PlayerNumber; }
    CTCBase* AddTC(const CCharString& name, bool init, CTCParamsBase* params)
    {
        g_addTC.thing = this;
        g_addTC.name = name.m_pData;
        g_addTC.init = init;
        g_addTC.params = params;
        ++g_addTC.calls;
        return (CTCBase*)g_shotTCResult;
    }
};
class CThingCreatureBase : public CThing { public:
    long GetShotAccuracyPercentage() { return 73; }
};
struct CreateRecord { long defIndex; C3DVector pos; long player; const CRightHandedSet* orientation; unsigned long flags;
                      const char* script; int calls; };
static CreateRecord g_create;
static CThing* g_createResult = 0;
CThing* Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
               unsigned long flags, const CCharString& scriptName)
{
    g_create.defIndex = defIndex;
    g_create.pos = *position;
    g_create.player = player;
    g_create.orientation = orientation;
    g_create.flags = flags;
    g_create.script = scriptName.m_pData;
    ++g_create.calls;
    return g_createResult;
}
inline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return Create(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}
struct EffectRecord { long type; const C3DVector* pos; bool flag; int calls; };
static EffectRecord g_effect;
CThing* CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    g_effect.type = type;
    g_effect.pos = position;
    g_effect.flag = flag;
    ++g_effect.calls;
    return 0;
}
struct InitRecord { CThingShotDef* def; long defRefsDuring; CThing* shooter; CThing* origin; C3DVector target; float accuracy;
                    float a; float b; bool abilityGiven; bool abilityEmpty; int calls; CThing* hit; int hitCalls; };
static InitRecord g_init;
struct CTCShotDefinitionBase {
    void Init(CDefPointerByValue<CThingShotDef> def, CThing* shooter, CThing* origin, const C3DVector* target, float accuracy,
              float a, float b, CDefPointer<CMeleeCombatAbilityDef>* ability)
    {
        g_init.def = def.m_p;
        g_init.defRefsDuring = def.m_p->RefCount;
        g_init.shooter = shooter;
        g_init.origin = origin;
        g_init.target = *target;
        g_init.accuracy = accuracy;
        g_init.a = a;
        g_init.b = b;
        g_init.abilityGiven = ability != 0;
        g_init.abilityEmpty = ability != 0 && ability->m_p == 0;
        ++g_init.calls;
    }
    void AddHitThing(CThing* thing) { g_init.hit = thing; ++g_init.hitCalls; }
};
struct CBaseIntelligentPointer {
    virtual void v00() { }
    CThingCreatureBase* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    ~CBaseIntelligentPointer() { }
    CThingCreatureBase* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = static_cast<CThingCreatureBase*>(item); }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00() { }
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
class CCombatActionBase { public:
    virtual void PerformAction() { }
    CBaseIntelligentPointer PCreature;
    CWorld* World;
    CWorld* GetPWorld() const { return World; }
};
class CCreatureAction_DragonRearUpFull : public CCombatActionBase { public:
    CIntelligentPointer PShot;
    virtual void PerformAction();
};

// ---- the reconstructed body (verbatim) ----
void CCreatureAction_DragonRearUpFull::PerformAction()
{
    long index = CGameDefinitionManager::Get().GetDefGlobalIndexFromName("JACK_DRAGON_GROUND_SLAM");
    CThingShotDef* shotDef;
    {
        CThingShotDef* out = 0;
        GetPWorld()->PDefinitionManager->GetDef(index, &out);
        shotDef = out;
    }
    const C3DVector& creaturePos = PCreature.GetPItem()->GetPosition();
    C3DVector position(creaturePos.X, creaturePos.Y, creaturePos.Z + 10.0f);
    position.Z = GetPWorld()->GetWorldMap()->GetGroundSizeZAt(position) + 0.25f;
    CThing* thing = CreateThingObject(shotDef->Thing, position, PCreature.GetPItem(), "");
    if (thing) {
        PShot = CIntelligentPointer(thing);
        CWeaponDef* weaponDef = 0;
        CDefClassBase* creatureDef = PCreature.GetPItem()->GetDef();
        creatureDef->GetSubDef(&weaponDef);
        long accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
        CTCShotDefinitionBase* shot;
        {
            CCharString name = g_DefStringTable.GetString(shotDef->TCName);
            shot = (CTCShotDefinitionBase*)PShot.GetPItem()->AddTC(name, true, 0);
        }
        C3DVector facing = PCreature.GetPItem()->GetPhysics()->GetFacing();
        facing.Z = 0.0f;
        facing.Normalise();
        C3DVector target = position + facing * 10.0f;
        {
            CDefPointer<CMeleeCombatAbilityDef> ability;
            shot->Init(shotDef, PCreature.GetPItem(), 0, &target, (float)accuracy, 1.0f, 0.0f, &ability);
        }
        shot->AddHitThing(PCreature.GetPItem());
        if (weaponDef)
            weaponDef->Release();
    }
    CSpecialEffectsDef* effects = 0;
    long effectType = 0;
    CThing* creature = PCreature.GetPItem();
    creature->GetDef()->GetSubDef(&effects);
    if (effects) {
        if (effects->GetEffectFromString("GROUND_SLAM_EXPLOSION", effectType))
            CreateEffectThing(effectType, &PCreature.GetPItem()->GetPosition(), false);
        effects->Release();
    }
    shotDef->Release();
}

static void ResetRecorders()
{
    std::memset(&g_addTC, 0, sizeof(g_addTC));
    g_create.defIndex = -1; g_create.player = -1; g_create.orientation = 0; g_create.flags = 99; g_create.script = 0;
    g_create.calls = 0;
    g_effect.type = -1; g_effect.pos = 0; g_effect.flag = true; g_effect.calls = 0;
    g_init.def = 0; g_init.defRefsDuring = -1; g_init.shooter = 0; g_init.origin = 0; g_init.accuracy = -1.0f;
    g_init.a = -1.0f; g_init.b = -1.0f; g_init.abilityGiven = false; g_init.abilityEmpty = false; g_init.calls = 0;
    g_init.hit = 0; g_init.hitCalls = 0;
    g_lastDefName = 0; g_lastIndexManager = 0; g_lastShotManager = 0; g_lastShotIndex = -1;
    g_lastWeaponLookup = 0; g_lastEffectsLookup = 0; g_lastEffectName = 0; g_groundCalls = 0;
    g_stringsBuilt = 0; g_stringsDestroyed = 0; g_defsDestroyed = 0;
    g_DefStringTable.LastIndex = -1;
}

int main()
{
    BuildTable();
    CHECK(Near(FastInvSqrt(25.0f), 0.2f));
    CHECK(Near(FastInvSqrt(2.0f), 0.70710678f));

    CDefinitionManager worldDefinitions;
    CWorld world;
    world.PDefinitionManager = &worldDefinitions;
    CDefClassBase dragonDef;
    CTCPhysicsBase physics;
    physics.Facing = C3DVector(3.0f, 4.0f, 7.0f);
    CThingCreatureBase dragon;
    dragon.PhysicsTC = &physics;
    dragon.PDef = &dragonDef;
    dragon.PlayerNumber = 2;
    dragon.Position = C3DVector(100.0f, 200.0f, 30.0f);
    CThingCreatureBase shotThing;
    shotThing.PhysicsTC = 0;
    shotThing.PDef = 0;
    shotThing.PlayerNumber = 0;
    shotThing.Position = C3DVector(0.0f, 0.0f, 0.0f);
    CTCShotDefinitionBase shotTC;
    g_shotTCResult = &shotTC;
    CWeaponDef weapon;
    weapon.RefCount = 1;
    CSpecialEffectsDef effectsDef;
    effectsDef.RefCount = 1;
    g_shotDef.TCName = 1;
    g_shotDef.Thing = 555;

    // 1. full path: the shot thing is created, Init'd toward the flat facing, and the explosion spawns at the dragon
    ResetRecorders();
    g_shotDef.RefCount = 1;
    g_weaponDefResult = &weapon;
    g_effectsDefResult = &effectsDef;
    g_effectTypeResult = 77;
    g_createResult = &shotThing;
    CCreatureAction_DragonRearUpFull action;
    action.PCreature.SetPItem(&dragon);
    action.World = &world;
    action.PerformAction();
    CHECK(g_lastDefName != 0 && std::strcmp(g_lastDefName, "JACK_DRAGON_GROUND_SLAM") == 0);
    CHECK(g_lastIndexManager == &g_gameDefinitionManager);
    CHECK(g_lastShotManager == &worldDefinitions);
    CHECK(g_lastShotIndex == 42);
    CHECK(g_groundCalls == 1);
    CHECK(Near(g_groundQuery.X, 100.0f) && Near(g_groundQuery.Y, 200.0f) && Near(g_groundQuery.Z, 40.0f));
    CHECK(g_create.calls == 1);
    CHECK(g_create.defIndex == 555);
    CHECK(Near(g_create.pos.X, 100.0f) && Near(g_create.pos.Y, 200.0f) && Near(g_create.pos.Z, 5.25f));
    CHECK(g_create.player == 2);
    CHECK(g_create.orientation == 0);
    CHECK(g_create.flags == 0);
    CHECK(g_create.script != 0 && g_create.script[0] == '\0');
    CHECK(action.PShot.GetPItem() == &shotThing);
    CHECK(g_lastWeaponLookup == &dragonDef);
    CHECK(g_DefStringTable.LastIndex == 1);
    CHECK(g_addTC.calls == 1);
    CHECK(g_addTC.thing == &shotThing);
    CHECK(g_addTC.name != 0 && std::strcmp(g_addTC.name, "TC_SHOT_GROUND_SLAM") == 0);
    CHECK(g_addTC.init);
    CHECK(g_addTC.params == 0);
    CHECK(g_init.calls == 1);
    CHECK(g_init.def == &g_shotDef);
    CHECK(g_init.defRefsDuring == 3);                    // caller's 1 + the fetched reference + the by-value copy
    CHECK(g_init.shooter == &dragon);
    CHECK(g_init.origin == 0);
    CHECK(Near(g_init.target.X, 106.0f) && Near(g_init.target.Y, 208.0f) && Near(g_init.target.Z, 5.25f));
    CHECK(g_init.accuracy == 73.0f);
    CHECK(g_init.a == 1.0f);
    CHECK(g_init.b == 0.0f);
    CHECK(g_init.abilityGiven && g_init.abilityEmpty);
    CHECK(g_init.hitCalls == 1 && g_init.hit == &dragon);
    CHECK(weapon.RefCount == 1);
    CHECK(g_lastEffectsLookup == &dragonDef);
    CHECK(g_lastEffectName != 0 && std::strcmp(g_lastEffectName, "GROUND_SLAM_EXPLOSION") == 0);
    CHECK(g_effect.calls == 1);
    CHECK(g_effect.type == 77);
    CHECK(g_effect.pos == &dragon.Position);
    CHECK(!g_effect.flag);
    CHECK(effectsDef.RefCount == 1);
    CHECK(g_shotDef.RefCount == 1);
    CHECK(g_defsDestroyed == 0);
    CHECK(g_stringsBuilt == g_stringsDestroyed);

    // 2. no thing created and no effects def: nothing added or Init'd, the last shot-def reference destroys it
    ResetRecorders();
    g_shotDef.RefCount = 0;
    g_weaponDefResult = &weapon;
    g_effectsDefResult = 0;
    g_createResult = 0;
    CCreatureAction_DragonRearUpFull idle;
    idle.PCreature.SetPItem(&dragon);
    idle.World = &world;
    idle.PerformAction();
    CHECK(g_create.calls == 1);
    CHECK(idle.PShot.GetPItem() == 0);
    CHECK(g_lastWeaponLookup == 0);
    CHECK(g_addTC.calls == 0);
    CHECK(g_init.calls == 0 && g_init.hitCalls == 0);
    CHECK(g_lastEffectsLookup == &dragonDef);
    CHECK(g_effect.calls == 0);
    CHECK(g_shotDef.RefCount == 0);
    CHECK(g_defsDestroyed == 1);
    CHECK(weapon.RefCount == 1);
    CHECK(g_stringsBuilt == g_stringsDestroyed);

    // 3. unknown effect name and no weapon def: shot still Init'd, no explosion, effects def still released
    ResetRecorders();
    g_shotDef.RefCount = 1;
    g_weaponDefResult = 0;
    g_effectsDefResult = &effectsDef;
    g_effectTypeResult = -1;
    g_createResult = &shotThing;
    physics.Facing = C3DVector(0.0f, -2.0f, -9.0f);
    CCreatureAction_DragonRearUpFull quiet;
    quiet.PCreature.SetPItem(&dragon);
    quiet.World = &world;
    quiet.PerformAction();
    CHECK(g_init.calls == 1);
    CHECK(Near(g_init.target.X, 100.0f) && Near(g_init.target.Y, 190.0f) && Near(g_init.target.Z, 5.25f));
    CHECK(g_effect.calls == 0);
    CHECK(effectsDef.RefCount == 1);
    CHECK(g_shotDef.RefCount == 1);
    CHECK(g_defsDestroyed == 0);
    CHECK(g_stringsBuilt == g_stringsDestroyed);

    if (g_failures) {
        std::printf("008f3630_TEST FAIL\n");
        return 1;
    }
    std::printf("008f3630_TEST PASS\n");
    return 0;
}