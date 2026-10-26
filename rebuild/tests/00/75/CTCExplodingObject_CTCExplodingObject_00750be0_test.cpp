// Behaviour test for 00750be0: src/00750be0.cpp copied verbatim; the constructor sets the default blast (10, 10, 10), no
// countdown, proximity radius 5, reads the exploding-object def's trigger settings, then (when the explosion thing def and
// its explosion sub-def exist) copies the explosion's damage, radius and fire damage; both defs are released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCExplodingObject::CTCExplodingObject(CThing& parent) -- constructor: CTCBase, default blast (10 damage, radius 10, 10 fire damage), no countdown, proximity trigger radius 5; then read the exploding-object def and, through its explosion thing def, the explosion's damage, radius and fire damage
// Leaf: ??0CTCExplodingObject
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00750BE0, length 232 bytes (+ int3 padding). `ret 4`; vtable 0x01269674.
//         Ego_r.pdb: `??0CTCExplodingObject@@QAE@AAVCThing@@@Z`.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800; CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10
//          (CIntelligentPointer<CThing> vtable 0x01238C6C); CDefClassBase::GetSubDef<CExplodingObjectDef> @ 0x007510D0 and
//          GetSubDef<CExplosionDef> @ 0x00751190 (both `bool (CDefPointer<const T>*) const`); CTCBase::GetWorld @ 0x00686A70;
//          CDefinitionManager::GetDef<T>(long, CDefPointer<const T>*) const @ 0x0042AF3C (T inferred as CThingBaseDef: the
//          shared instantiation called from 119 sites including CThing's own code); the defs' virtual slot 1 (destroy).
// Shape: the float defaults share one register (10.0f) and the other words are immediates, all after the vtable store;
//        PDef is read once (edi) for its three fields. GetDef and the explosion GetSubDef have same-TU out-slot-only
//        stand-in bodies (neither out pointer escapes); the explosion def (declared first, starting null in ebp, which
//        also zeroes both out pointers) is homed in the dead `parent` slot, the thing def in the push-ecx word; each is
//        read once after its call (a GetSubDef result tested directly, with the def read in both arms) and released by
//        hand at the end (`dec [reg+4] ; jne`).
// Layout: Ego_r.pdb CTCExplodingObject matches retail (MaxDamage +0x0C .. PDef +0x24, PProximityParticle +0x28);
//         CExplodingObjectDef/CExplosionDef offsets are the PDB's; retail CWorld definition manager +0x10.
class CThing;
class CBaseObject;

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
};
template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() {}
};

class CDefClassBase {
public:
    virtual void v00();
    virtual void Destroy() const;                   // slot 1
    mutable long RefCount;                          // +0x04
};

template<class T> class CDefPointer {
public:
    T* Object;
    CDefPointer() : Object(0) {}
    T* operator->() const { return Object; }
};

class CExplosionDef : public CDefClassBase {
public:
    unsigned char _pad08[0x28 - 0x08];
    float Damage;                                   // +0x28
    float ExplosionRadius;                          // +0x2C
    long FireDamage;                                // +0x30
};
class CThingBaseDef : public CDefClassBase {
public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x00751190 for T = CExplosionDef
};
void* Find_CThingBaseDef_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CThingBaseDef::GetSubDef(CDefPointer<const T>* out) const
{
    void* def = Find_CThingBaseDef_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (T*)def;
    return true;
}
#pragma auto_inline(on)
class CExplodingObjectDef : public CDefClassBase {
public:
    unsigned char _pad08[0x25 - 0x08];
    bool TriggeredOnCreatureProximity;              // +0x25
    unsigned char _pad26[2];
    float TriggerRadius;                            // +0x28
    long ExplosionDef;                              // +0x2C
};
class CThingDefClass {                              // the thing's base def as a CDefClassBase
public:
    template<class T> bool GetSubDef(CDefPointer<const T>* out) const;   // 0x007510D0 for T = CExplodingObjectDef
};

class CDefinitionManager {
public:
    template<class T> bool GetDef(long index, CDefPointer<const T>* out) const;   // 0x0042AF3C
};
void* Find_CDefinitionManager_GetDef(const void* owner, long index);   // stand-in callee of the stand-in body below
// Stand-in body (out of line in retail): it writes the out slot only.
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefinitionManager::GetDef(long index, CDefPointer<const T>* out) const
{
    void* def = Find_CDefinitionManager_GetDef(this, index);
    if (!def)
        return false;
    out->Object = (const T*)def;
    return true;
}
#pragma auto_inline(on)

class CWorld { public:
    unsigned char _pad00[0x10];
    CDefinitionManager* DefinitionManager;          // +0x10
};

class CThing {
public:
    unsigned char _pad00[0x70];
    CThingDefClass* PBaseDef;                       // +0x70 CDefPointer<const CThingBaseDef>::Object
};

class CTCBase {
public:
    CTCBase(CThing* parent);                        // 0x00686800
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    unsigned char _pad08[0x4];
    CWorld* GetWorld();                             // 0x00686A70
};

class CTCExplodingObject : public CTCBase {         // vtable 0x01269674
public:
    float MaxDamage;                                // +0x0C
    float Radius;                                   // +0x10
    long FireDamage;                                // +0x14
    long CountdownTimer;                            // +0x18
    bool TriggeredOnCreatureProximity;              // +0x1C
    float TriggerRadius;                            // +0x20
    CDefPointer<const CExplodingObjectDef> PDef;    // +0x24
    CIntelligentPointer<CThing> PProximityParticle; // +0x28
    CTCExplodingObject(CThing& parent);
    virtual void v00();
};

CTCExplodingObject::CTCExplodingObject(CThing& parent)
    : CTCBase(&parent), MaxDamage(10.0f), Radius(10.0f), FireDamage(10), CountdownTimer(-1),
      TriggeredOnCreatureProximity(false), TriggerRadius(5.0f)
{
    ParentThing->PBaseDef->GetSubDef(&PDef);
    const CExplodingObjectDef* def = PDef.Object;
    TriggeredOnCreatureProximity = def->TriggeredOnCreatureProximity;
    TriggerRadius = def->TriggerRadius;
    const CExplosionDef* explosionDef = 0;
    CDefPointer<const CExplosionDef> explosionOut;
    CDefPointer<const CThingBaseDef> thingOut;
    CDefinitionManager* manager = GetWorld()->DefinitionManager;
    bool found = manager->GetDef(def->ExplosionDef, &thingOut);
    const CThingBaseDef* thingDef = thingOut.Object;
    if (found) {
        if (thingDef->GetSubDef(&explosionOut)) {
            explosionDef = explosionOut.Object;
            MaxDamage = explosionDef->Damage;
            Radius = explosionDef->ExplosionRadius;
            FireDamage = explosionDef->FireDamage;
        } else
            explosionDef = explosionOut.Object;
    }
    if (thingDef && --thingDef->RefCount == 0)
        thingDef->Destroy();
    if (explosionDef && --explosionDef->RefCount == 0)
        explosionDef->Destroy();
}

// ---- harness ----
static int g_destroyed = 0; static void* g_exploding = 0; static void* g_thingDef = 0; static void* g_explosion = 0;
static long g_index = -1; static int g_getSub = 0; static CThing* g_baseParent = 0;
static CWorld g_world;
CBaseIntelligentPointer::CBaseIntelligentPointer() { m_pItem = 0; }
void CBaseIntelligentPointer::v00() { }
template<class T> void CIntelligentPointer<T>::v00() { }
void CDefClassBase::v00() { }
void CDefClassBase::Destroy() const { ++g_destroyed; }
template<class T> bool CThingDefClass::GetSubDef(CDefPointer<const T>* out) const
{ if (!g_exploding) return false; ++((CDefClassBase*)g_exploding)->RefCount; out->Object = (const T*)g_exploding; return true; }
void* Find_CThingBaseDef_GetSubDef(const void* owner)
{ ++g_getSub; CHECK(owner == g_thingDef); if (g_explosion) ++((CDefClassBase*)g_explosion)->RefCount; return g_explosion; }
void* Find_CDefinitionManager_GetDef(const void* owner, long index)
{ g_index = index; CHECK(owner == g_world.DefinitionManager); if (g_thingDef) ++((CDefClassBase*)g_thingDef)->RefCount; return g_thingDef; }
CTCBase::CTCBase(CThing* parent) { ParentThing = parent; g_baseParent = parent; }
void CTCBase::v00() { }
CWorld* CTCBase::GetWorld() { return &g_world; }
void CTCExplodingObject::v00() { }
int main()
{
    CDefinitionManager mgr; memset(&g_world, 0, sizeof g_world); g_world.DefinitionManager = &mgr;
    CThingDefClass baseDef;
    CThing parent; memset(&parent, 0, sizeof parent); parent.PBaseDef = &baseDef;
    CExplodingObjectDef eo; eo.RefCount = 1; eo.TriggeredOnCreatureProximity = true; eo.TriggerRadius = 7.5f; eo.ExplosionDef = 0x55;
    CThingBaseDef thingDef; thingDef.RefCount = 1;
    CExplosionDef ex; ex.RefCount = 1; ex.Damage = 40.0f; ex.ExplosionRadius = 12.5f; ex.FireDamage = 3;
    g_exploding = &eo; g_thingDef = &thingDef; g_explosion = &ex;

    {
        CTCExplodingObject o(parent);
        CHECK(g_baseParent == &parent && o.ParentThing == &parent && o.PDef.Object == &eo);
        CHECK(o.CountdownTimer == -1 && o.TriggeredOnCreatureProximity && o.TriggerRadius == 7.5f && g_index == 0x55);
        CHECK(o.MaxDamage == 40.0f && o.Radius == 12.5f && o.FireDamage == 3 && o.PProximityParticle.m_pItem == 0);
        CHECK(thingDef.RefCount == 1 && ex.RefCount == 1 && g_destroyed == 0 && g_getSub == 1);
    }
    {   // no explosion sub-def: defaults kept, thing def released
        g_explosion = 0; g_getSub = 0; eo.TriggeredOnCreatureProximity = false;
        CTCExplodingObject o(parent);
        CHECK(o.MaxDamage == 10.0f && o.Radius == 10.0f && o.FireDamage == 10 && !o.TriggeredOnCreatureProximity);
        CHECK(o.TriggerRadius == 7.5f && thingDef.RefCount == 1 && g_getSub == 1 && g_destroyed == 0);
    }
    {   // no explosion thing def: no sub-def lookup
        g_thingDef = 0; g_explosion = &ex; g_getSub = 0;
        CTCExplodingObject o(parent);
        CHECK(o.MaxDamage == 10.0f && o.Radius == 10.0f && o.FireDamage == 10 && g_getSub == 0 && ex.RefCount == 1);
    }
    {   // last references: both destroyed
        g_thingDef = &thingDef; thingDef.RefCount = 0; ex.RefCount = 0; g_destroyed = 0;
        CTCExplodingObject o(parent);
        CHECK(o.MaxDamage == 40.0f && g_destroyed == 2 && thingDef.RefCount == 0 && ex.RefCount == 0);
    }
    if (fails == 0) printf("00750be0_TEST PASS\n");
    return fails;
}