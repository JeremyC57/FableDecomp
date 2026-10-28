// 00783380 behaviour test: CTCBoastingCrowdControl::SpawnCrowdMember (control flow + data of the final.cpp body)
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif
#include <stdio.h>
#include <string.h>
#include <math.h>

class CCharString { public:
    const char* text;
    CCharString(const char* t, long length = -1) : text(t) { (void)length; }
    CCharString(const CCharString& o) : text(o.text) { }
    const CCharString& operator=(const CCharString& o) { text = o.text; return *this; }
};
static CCharString g_CrowdMemberScriptName("CrowdMember");
class C3DVector { public: float X, Y, Z; };
static unsigned long __fastcall GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static float g_floatRandom;
static float __fastcall GFFloatRandom(float range, unsigned long* seed) { (void)seed; return g_floatRandom * range; }
__forceinline unsigned long GFRandom(unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return *seed;
}
__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
class CThingAICreatureInit { public:
    CCharString ScriptName;
    bool EnableCreatureAutoPlacing;
    CThingAICreatureInit() : ScriptName(""), EnableCreatureAutoPlacing(true) { }
};
static float g_facing;
struct CTCPlacement { void SetFacingAngleXY(float a) { g_facing = a; } };
class CThing;
static CThing* g_added;
class CTCVillage { public:
    CThing* ParentThing;
    void AddThingToVillage(CThing* t) { g_added = t; }
};
struct STCEntry { long type; void* component; };
struct CTCMap {
    STCEntry* m_pFirst;
    STCEntry* m_pLast;
    STCEntry* m_pEnd;
    STCEntry* LowerBound(const unsigned long& kind)
    {
        STCEntry* first = m_pFirst;
        long count = m_pLast - m_pFirst;
        while (count > 0) {
            long half = count >> 1;
            STCEntry* mid = first + half;
            if ((unsigned long)mid->type < kind) {
                first = mid + 1;
                count -= half + 1;
            } else
                count = half;
        }
        return first;
    }
    __forceinline void* Find(long kind)
    {
        STCEntry* it = LowerBound((unsigned long)kind);
        return ((it == m_pLast || kind < it->type) ? m_pLast : it)->component;
    }
};
static int g_destroys;
struct CCrowdDef {
    long RefCount;
    float MaleChance;
    long MaleDef;
    long FemaleDef;
    void Destroy() { ++g_destroys; }
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
static CCrowdDef* g_crowdDef;
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCrowdDef>* out) { if (!g_crowdDef) return false; ++g_crowdDef->RefCount; out->Object = g_crowdDef; return true; }
};
static bool g_persisted;
class CThing { public:
    unsigned long  m_tcMask[9];
    CTCMap         m_tcs;
    CDefClassBase* m_pDef;
    unsigned char  InCrowd : 1;
    unsigned char  IsCrowdMember : 1;
    unsigned char  NoLOD : 1;
    C3DVector      Pos;
    CDefClassBase* GetDef() const { return m_pDef; }
    const C3DVector& GetPosition() const { return Pos; }
    void SetAsGameStatePersisted(bool p) { g_persisted = p; }
    unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
    __forceinline void* GetTC(long kind) { return m_tcs.Find(kind); }
};
static long g_createDef = -1; static long g_createPlayer; static bool g_createAuto; static const char* g_createScript;
static float g_autoRadius; static float g_sexuality;
class CThingAICreature : public CThing { public:
    void AutoPlaceNear(const C3DVector&, float radius) { g_autoRadius = radius; }
    void AutoSetupSexuality(float c) { g_sexuality = c; }
    static CThingAICreature* g_next;
    static CThingAICreature* __fastcall Create(long defIndex, const C3DVector&, long player, CThingAICreatureInit* init)
    {
        g_createDef = defIndex; g_createPlayer = player; g_createAuto = init->EnableCreatureAutoPlacing;
        g_createScript = init->ScriptName.text;
        return g_next;
    }
};
CThingAICreature* CThingAICreature::g_next;
static unsigned long g_seed;
class CTCBoastingCrowdControl { public:
    CThing* ParentThing;
    unsigned long* GetRandomSeed() { return &g_seed; }
    bool SpawnCrowdMember(CThing* lookAt, CThing* spawnPoint, CThing* village);
};

bool CTCBoastingCrowdControl::SpawnCrowdMember(CThing* lookAt, CThing* spawnPoint, CThing* village)
{
    if (village->HasTC(0x22)) {
        CTCVillage* tcVillage = (CTCVillage*)village->GetTC(0x22);
        CDefPointer<CCrowdDef> def;
        if (tcVillage->ParentThing->GetDef()->GetSubDef(&def)) {
            if (def->MaleDef == 0 && def->FemaleDef == 0)
                return false;
            bool female;
            if (def->FemaleDef == 0)
                female = false;
            else if (def->MaleDef == 0)
                female = true;
            else
                female = (GFRandom(GetRandomSeed()) & 1) != 0;
            CThingAICreatureInit init;
            init.ScriptName = g_CrowdMemberScriptName;
            init.EnableCreatureAutoPlacing = false;
            CThingAICreature* creature = CThingAICreature::Create(female ? def->FemaleDef : def->MaleDef,
                                                                  spawnPoint->GetPosition(), 4, &init);
            if (creature) {
                creature->AutoPlaceNear(spawnPoint->GetPosition(), (GFFloatRandom(0.5f, GetRandomSeed()) + 0.5f) * 5.0f);
                CTCPlacement* placement;
                if (creature->HasTC(2))
                    placement = (CTCPlacement*)creature->GetTC(2);
                const C3DVector& from = creature->GetPosition();
                const C3DVector& to = lookAt->GetPosition();
                placement->SetFacingAngleXY(WrapAngle((float)atan2(to.X - from.X, to.Y - from.Y) * 0.15915494f));
                tcVillage->AddThingToVillage(creature);
                creature->AutoSetupSexuality(def->MaleChance);
                creature->SetAsGameStatePersisted(false);
                creature->InCrowd = false;
                creature->NoLOD = true;
                creature->IsCrowdMember = true;
                return true;
            }
        }
    }
    return false;
}

static int g_fail;
static void Check(const char* name, bool cond) { if (!cond) { printf("FAIL %s\n", name); g_fail = 1; } }

int main()
{
    CDefClassBase defBase;
    CThing villageParent; memset(&villageParent, 0, sizeof(villageParent)); villageParent.m_pDef = &defBase;
    CTCVillage tcVillage; tcVillage.ParentThing = &villageParent;
    STCEntry vEntries[2] = { { 0x22, &tcVillage }, { 0x30, 0 } };
    CThing village; memset(&village, 0, sizeof(village)); village.m_tcMask[1] = 1UL << 2;   // 0x22
    village.m_tcs.m_pFirst = vEntries; village.m_tcs.m_pLast = vEntries + 2; village.m_tcs.m_pEnd = vEntries + 2;
    CThing lookAt; memset(&lookAt, 0, sizeof(lookAt)); lookAt.Pos.X = 0; lookAt.Pos.Y = 10;
    CThing spawn; memset(&spawn, 0, sizeof(spawn));
    CTCPlacement placement;
    STCEntry cEntries[1] = { { 2, &placement } };
    CThingAICreature creature; memset((CThing*)&creature, 0, sizeof(CThing));
    creature.m_tcMask[0] = 1UL << 2; creature.m_tcs.m_pFirst = cEntries; creature.m_tcs.m_pLast = cEntries + 1;
    creature.InCrowd = 1; creature.NoLOD = 0; creature.IsCrowdMember = 0; creature.Pos.X = 0; creature.Pos.Y = 0;
    CCrowdDef def; def.RefCount = 1; def.MaleChance = 0.25f; def.MaleDef = 11; def.FemaleDef = 22;
    CTCBoastingCrowdControl ctl; ctl.ParentThing = 0;
    g_floatRandom = 0.5f;

#define RESET() (g_createDef = -1, g_added = 0, g_destroys = 0, g_crowdDef = &def, CThingAICreature::g_next = &creature, \
                 creature.InCrowd = 1, creature.NoLOD = 0, creature.IsCrowdMember = 0, g_persisted = true)
    // no village TC
    { CThing bare; memset(&bare, 0, sizeof(bare)); RESET();
      Check("no village tc", !ctl.SpawnCrowdMember(&lookAt, &spawn, &bare) && g_createDef == -1); }
    // no crowd def
    RESET(); g_crowdDef = 0; Check("no def", !ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && g_createDef == -1);
    // neither def -> false, def released
    RESET(); def.MaleDef = 0; def.FemaleDef = 0;
    Check("no defs", !ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && g_createDef == -1 && def.RefCount == 1);
    // male only -> male def
    RESET(); def.MaleDef = 11; def.FemaleDef = 0;
    bool ok = ctl.SpawnCrowdMember(&lookAt, &spawn, &village);
    Check("male only", ok && g_createDef == 11 && g_createPlayer == 4 && !g_createAuto
        && strcmp(g_createScript, "CrowdMember") == 0 && g_added == &creature && g_sexuality == 0.25f && !g_persisted
        && creature.InCrowd == 0 && creature.NoLOD == 1 && creature.IsCrowdMember == 1
        && g_autoRadius == 3.75f && fabs(g_facing - 0.0f) < 1e-6f && def.RefCount == 1);
    // female only -> female def
    RESET(); def.MaleDef = 0; def.FemaleDef = 22;
    Check("female only", ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && g_createDef == 22);
    // both -> random bit picks
    RESET(); def.MaleDef = 11; def.FemaleDef = 22; g_seed = 1;
    { unsigned long s = 1; s = GFROR13(s * 0x24a1 + 0x24df); long expect = (s & 1) ? 22 : 11;
      Check("both", ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && g_createDef == expect && g_seed == s); }
    // create fails -> false
    RESET(); CThingAICreature::g_next = 0; Check("create fails", !ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && g_added == 0);
    // facing: look at (10,0) from origin -> atan2(10,0)/2pi = 0.25
    RESET(); lookAt.Pos.X = 10; lookAt.Pos.Y = 0;
    Check("facing", ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && fabs(g_facing - 0.25f) < 1e-4f);
    // negative angle wraps
    RESET(); lookAt.Pos.X = -10; lookAt.Pos.Y = 0;
    Check("wrap", ctl.SpawnCrowdMember(&lookAt, &spawn, &village) && fabs(g_facing - 0.75f) < 1e-4f);
    if (g_fail) return 1;
    printf("00783380_TEST PASS\n");
    return 0;
}