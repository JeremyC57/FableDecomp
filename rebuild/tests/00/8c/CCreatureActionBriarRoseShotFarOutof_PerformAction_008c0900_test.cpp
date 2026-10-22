// Behaviour test for 0x008C0900 CCreatureAction_BriarRoseShotFarOutof::PerformAction: runs the reconstructed body over stubbed
// callees; checks the def lookups, the dummy lookup, the Create arguments, PShot, the AddTC name, every Init argument and the
// def releases (both on the created path and when Create returns null).
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif

struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { C3DVector First; C3DVector Second; };

static int g_strings = 0;
static char g_lastString[64];
struct CCharString {
    char m_text[64];
    CCharString(const char* text, long = -1) { std::strcpy(m_text, text); ++g_strings; }
    CCharString(const CCharString& o) { std::strcpy(m_text, o.m_text); ++g_strings; }
    ~CCharString() { --g_strings; }
};
struct CDefStringTable {
    CCharString GetString(long index)
    {
        std::sprintf(g_lastString, "TC_%ld", index);
        return CCharString(g_lastString);
    }
};
static CDefStringTable g_DefStringTable;

static int g_destroyed = 0;
struct CDefBase {
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; }
    long RefCount;
    void Release() { if (--RefCount == 0) Destroy(); }
};
struct CWeaponDef : CDefBase { long Shot; };
struct CThingShotDef : CDefBase { long TCName; long Thing; };

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
static int g_abilityDestroyed = 0;
struct CDefVirtual {
    virtual void v00() { }
    virtual void Destroy() { ++g_abilityDestroyed; }
};
struct CMeleeCombatAbilityDef : CDefVirtual, CRefCounted {
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
template <class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
};

static CWeaponDef g_weaponDef;
static CThingShotDef g_shotDef;
struct CDefClassBase {
    bool GetSubDef(CWeaponDef** out) { ++g_weaponDef.RefCount; *out = &g_weaponDef; return true; }
};
static long g_getDefIndex = 0;
struct CDefinitionManager {
    bool GetDef(long index, CThingShotDef** out) { g_getDefIndex = index; ++g_shotDef.RefCount; *out = &g_shotDef; return true; }
};
struct CWorld { CDefinitionManager* PDefinitionManager; };

static char g_dummyName[64];
static C3DVector* g_dummyPos = 0;
static CRightHandedSet* g_dummySet = 0;
static float g_dummyScale = 0;
struct CTCBase { };
struct CTCParamsBase;
struct CTCGraphicAppearance {
    bool GetDummyObject(const CCharString& name, long index, unsigned long flags, C3DVector* position,
                        CRightHandedSet* orientation, float scale)
    {
        std::strcpy(g_dummyName, name.m_text);
        g_dummyPos = position; g_dummySet = orientation; g_dummyScale = scale;
        if (index != 0 || flags != 0) return false;
        position->X += 1.0f;       // the dummy moves the position
        return true;
    }
};

struct CThingShotTC;
static char g_tcName[64];
static bool g_tcInit = false;
struct CThing {
    CTCGraphicAppearance* PGraphicAppearance;
    CDefClassBase*        PDef;
    signed char           PlayerNumber;
    C3DVector             m_pos;
    CTCBase*              m_tc;
    const C3DVector& GetPosition() const { return m_pos; }
    CTCBase* AddTC(const CCharString& name, bool init, CTCParamsBase* params)
    {
        std::strcpy(g_tcName, name.m_text); g_tcInit = init && params == 0; return m_tc;
    }
};
__forceinline bool GetDummyObject(const CThing* thing, const CCharString& name, C3DVector* position, CRightHandedSet* orientation)
{
    CTCGraphicAppearance* appearance = thing->PGraphicAppearance;
    return appearance->GetDummyObject(name, 0, 0, position, orientation, 1.0f);
}
struct CThingCreatureBase : CThing {
    long m_accuracy;
    long GetShotAccuracyPercentage() { return m_accuracy; }
};

static CThing* g_created = 0;
static long g_createIndex, g_createPlayer;
static C3DVector g_createPos;
static char g_createName[64];
CThing* Create(long defIndex, const C3DVector* position, long player, const CRightHandedSet* orientation,
               unsigned long flags, const CCharString& scriptName)
{
    g_createIndex = defIndex; g_createPos = *position; g_createPlayer = player; std::strcpy(g_createName, scriptName.m_text);
    if (orientation != 0 || flags != 0) return 0;
    return g_created;
}
__forceinline CThing* CreateThingObject(const long& defIndex, const C3DVector& position, const CThing* creature, const CCharString& name)
{
    return Create(defIndex, &position, creature->PlayerNumber, 0, 0, name);
}

static CMeleeCombatAbilityDef g_ability;
static long g_initRef = 0;
static CThing* g_initShooter; static CThing* g_initOrigin; static CThing* g_initTarget;
static float g_initAcc, g_initA, g_initB;
static int g_inits = 0;
struct CTCShotDefinitionBase : CTCBase {
    void Init(CDefPointerByValue<CThingShotDef> def, CThing* shooter, CThing* origin, CThing* target, float accuracy,
              float a, float b, CDefPointer<CMeleeCombatAbilityDef>* ability)
    {
        ++g_inits; g_initRef = def.m_p->RefCount; g_initShooter = shooter; g_initOrigin = origin; g_initTarget = target;
        g_initAcc = accuracy; g_initA = a; g_initB = b;
        g_ability.m_count = 1; ability->m_p = &g_ability;
    }
};

struct CBaseIntelligentPointer {
    CThing* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { }
    CThing* GetPItem() const { return m_pItem; }
    void SetPItem(CThing* item) { m_pItem = item; }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CCreatureBaseIntelligentPointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
};

struct CCombatActionBase {
    virtual void PerformAction() { }
    CCreatureBaseIntelligentPointer PCreature;
    CBaseIntelligentPointer PTarget;
    CWorld* m_world;
    CWorld* GetPWorld() const { return m_world; }
};
struct CCreatureAction_BriarRoseShotFarOutof : CCombatActionBase {
    CIntelligentPointer PShot;
    virtual void PerformAction();
};

void CCreatureAction_BriarRoseShotFarOutof::PerformAction()
{
    CWeaponDef* weaponDef = 0;
    CDefClassBase* creatureDef = PCreature.GetPItem()->PDef;
    creatureDef->GetSubDef(&weaponDef);
    CThingShotDef* shotDef;
    {
        CThingShotDef* out = 0;
        CDefinitionManager* manager = GetPWorld()->PDefinitionManager;
        manager->GetDef(weaponDef->Shot, &out);
        shotDef = out;
    }
    C3DVector position = PCreature.GetPItem()->GetPosition();
    CRightHandedSet orientation;
    GetDummyObject(PCreature.GetPItem(), "WEAPON_FOCUS_01", &position, &orientation);
    CThing* thing = CreateThingObject(shotDef->Thing, position, PCreature.GetPItem(), "");
    if (thing) {
        long accuracy;
        PShot = CIntelligentPointer(thing);
        accuracy = PCreature.GetPItem()->GetShotAccuracyPercentage();
        CTCShotDefinitionBase* shot;
        {
            CCharString name = g_DefStringTable.GetString(shotDef->TCName);
            shot = (CTCShotDefinitionBase*)PShot.GetPItem()->AddTC(name, true, 0);
        }
        {
            CDefPointer<CMeleeCombatAbilityDef> ability;
            CThing* creature = PCreature.GetPItem();
            shot->Init(shotDef, creature, 0, PTarget.GetPItem(), (float)accuracy, 1.0f, 0.0f, &ability);
        }
    }
    shotDef->Release();
    weaponDef->Release();
}

int main()
{
    bool ok = true;
    CDefinitionManager manager; CWorld world; world.PDefinitionManager = &manager;
    CTCGraphicAppearance appearance; CDefClassBase creatureDef;
    CThingCreatureBase creature;
    creature.PGraphicAppearance = &appearance; creature.PDef = &creatureDef; creature.PlayerNumber = -3;
    creature.m_pos.X = 10.0f; creature.m_pos.Y = 20.0f; creature.m_pos.Z = 30.0f; creature.m_accuracy = 65; creature.m_tc = 0;
    CThing target; target.m_tc = 0;
    CTCShotDefinitionBase shotTC;
    CThing shot; shot.m_tc = &shotTC;
    g_weaponDef.RefCount = 1; g_weaponDef.Shot = 77;
    g_shotDef.RefCount = 1; g_shotDef.TCName = 12; g_shotDef.Thing = 404;

    CCreatureAction_BriarRoseShotFarOutof action;
    action.PCreature.m_pItem = &creature; action.PTarget.m_pItem = &target; action.m_world = &world;
    g_created = &shot;
    action.PerformAction();
    ok = ok && g_getDefIndex == 77 && std::strcmp(g_dummyName, "WEAPON_FOCUS_01") == 0 && g_dummyScale == 1.0f;
    ok = ok && g_createIndex == 404 && g_createPlayer == -3 && g_createPos.X == 11.0f && g_createPos.Z == 30.0f;
    ok = ok && std::strcmp(g_createName, "") == 0 && action.PShot.GetPItem() == &shot;
    ok = ok && std::strcmp(g_tcName, "TC_12") == 0 && g_tcInit && g_inits == 1 && g_initRef == 3;
    ok = ok && g_initShooter == &creature && g_initOrigin == 0 && g_initTarget == &target;
    ok = ok && g_initAcc == 65.0f && g_initA == 1.0f && g_initB == 0.0f;
    ok = ok && g_ability.m_count == 0 && g_abilityDestroyed == 1 && g_strings == 0;
    ok = ok && g_weaponDef.RefCount == 1 && g_shotDef.RefCount == 1 && g_destroyed == 0;

    // Create fails: no PShot change, no Init, defs still released
    CCreatureAction_BriarRoseShotFarOutof action2;
    action2.PCreature.m_pItem = &creature; action2.PTarget.m_pItem = &target; action2.m_world = &world;
    g_created = 0;
    action2.PerformAction();
    ok = ok && action2.PShot.GetPItem() == 0 && g_inits == 1 && g_strings == 0;
    ok = ok && g_weaponDef.RefCount == 1 && g_shotDef.RefCount == 1 && g_destroyed == 0;

    // last references: both defs destroyed
    g_weaponDef.RefCount = 0; g_shotDef.RefCount = 0;
    action2.PerformAction();
    ok = ok && g_destroyed == 2;

    if (!ok) { std::printf("008c0900_TEST FAIL\n"); return 1; }
    std::printf("008c0900_TEST PASS\n");
    return 0;
}