// Behaviour test for 00726340: src/00726340.cpp copied verbatim, out-of-line callees stubbed below.
#include <cstdio>
#include <cstring>
#include <cmath>
// ---- source (verbatim) ----
// CApplyAdditionalPlayerImpulseIfTooClose::operator()(CThingCreatureBase* other) -- when the other thing overlaps the player vertically, is within the combined radius plus 0.1 in XY, both collide with things, it is pushable by the hero and the player's collision filter lets them collide: push it away from the player (harder the deeper the overlap) or, when on the same spot, along the player's velocity, turned by a random angle; a moving player also tells a creature it is being pushed
// Grade: RELOCATION_MATCH (retry A 11a) -- the unit scaling is written per component (X = X / dist ...), not as offset *= 1/dist, and GetXYLengthSquared is sequenced (s = X*X; s += Y*Y)
// Leaf: ??RCApplyAdditionalPlayerImpulseIfTooClose
#include <math.h>
#pragma intrinsic(fabs, sqrt)

class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const; // 0x004365B0 (the const copy)
};
class CThing;
struct CTCCollisionFilter {                     // the interface-0x4D component
    bool CanCollideWith(CThing* other);         // 0x00770DE0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    __forceinline C3DVector& operator*=(float s) { X = s * X; Y = s * Y; Z = s * Z; return *this; }
    float GetXYLengthSquared() const { float s = X * X; s += Y * Y; return s; }
    void RotateAboutZ(float turns);             // 0x00A14040
};

class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56();
    virtual bool GetCollidesWithThings();       // slot 57 (+0xe4)
    virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63();
    virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetVelocity();            // slot 70 (+0x118)
    virtual void v71(); virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76();
    virtual void v77(); virtual void v78(); virtual void v79(); virtual void v80();
    virtual void AddAdditionalImpulse(const C3DVector& impulse);   // slot 81 (+0x144)
    virtual void v82();
    virtual bool IsPushableByHero();            // slot 83 (+0x14c)
    CThing* ParentThing;                        // +0x04
    unsigned char _pad08[0x34 - 0x08];
    float Radius;                               // +0x34
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    __forceinline unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    __forceinline CTCBase* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
    }
    const C3DVector& GetPosition() const;       // 0x004C73D0
};
class C2DVector { public:
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
};
class CThingCreatureBase : public CThing {
public:
};
// Layout: retail CThing type byte +0x10 (1 = creature); the player's flag byte +0x6c (bit 3) and 2D move vector +0x10c.
struct CThingView {
    unsigned char _pad00[0x10];
    unsigned char Type;                         // +0x10
    unsigned char _pad11[0x6c - 0x11];
    unsigned char Flags6c;                      // +0x6c
    unsigned char _pad6d[0x10c - 0x6d];
    C2DVector MoveVector;                       // +0x10c
};
class CThingAICreature : public CThingCreatureBase {
public:
    void OnHeroPush(CThing* pusher, float distance, const C3DVector& direction);   // 0x008328D0
};
namespace NCreatureMode {
bool __fastcall IsPhysicsPushableByHero(const CThing& thing);   // 0x0085FD70
}

class CWorld { public:
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* World;                              // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0
extern long __fastcall GFGetCurrentFrame();        // 0x0049D870
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0


class CApplyAdditionalPlayerImpulseIfTooClose { public:   // 0x14 bytes
    C3DVector       Position;                   // +0x00
    float           Radius;                     // +0x0c
    CTCPhysicsBase* Owner;                      // +0x10
    void operator()(CThingCreatureBase* other);
};

void CApplyAdditionalPlayerImpulseIfTooClose::operator()(CThingCreatureBase* other)
{
    const C3DVector& pos = other->GetPosition();
    float dz = pos.Z - Position.Z;
    C3DVector offset(pos.X - Position.X, pos.Y - Position.Y, 0.0f);
    float radii = other->PhysicsTC->Radius + Radius;
    if (fabs(dz) < radii) {
        float distSq = offset.GetXYLengthSquared();
        float range = radii + 0.1f;
        if (Owner->GetCollidesWithThings() && other->PhysicsTC->GetCollidesWithThings() && range * range > distSq
            && other->PhysicsTC->IsPushableByHero() && NCreatureMode::IsPhysicsPushableByHero(*other)) {
            CThing* owner = Owner->ParentThing;
            if (owner->HasTC(0x4d)) {
                if (!((CTCCollisionFilter*)owner->GetTC(0x4d))->CanCollideWith(other))
                    return;
            }
            float penetration = 0.0f;
            if (distSq > 0.0001f) {
                float scale = 1.0f;
                float dist = sqrt(distSq);
                penetration = dist - radii;
                if (penetration > 0.0f)
                    scale = (0.1f - penetration) * 10.0f;
                offset.X = offset.X / dist; offset.Y = offset.Y / dist; offset.Z = offset.Z / dist;
                offset *= scale * 0.2f;
            } else {
                offset = Owner->GetVelocity();
                offset *= 0.1f;
            }
            CWorld* world = g_pMainGameComponent->World;
            float sign = (GFGetCurrentFrame() % 2 == 0) ? -1.0f : 1.0f;
            float angle = GFFloatRandom(45.0f, world->GetPRandomSeed()) * sign;
            float turns = angle * (1.0f / 360.0f);
            offset.RotateAboutZ(turns * sign);
            other->PhysicsTC->AddAdditionalImpulse(offset);
            if (((CThingView*)other)->Type == 1 && (((CThingView*)Owner->ParentThing)->Flags6c & 8)) {
                CThing* player = Owner->ParentThing;
                if (((CThingView*)player)->MoveVector.GetLengthSquared() > 0.0001f)
                    ((CThingAICreature*)other)->OnHeroPush(player, 0.1f - penetration, offset);
            }
        }
    }
}

// ---- stubs ----
void CTCPhysicsBase::v00(){}
void CTCPhysicsBase::v01(){}
void CTCPhysicsBase::v02(){}
void CTCPhysicsBase::v03(){}
void CTCPhysicsBase::v04(){}
void CTCPhysicsBase::v05(){}
void CTCPhysicsBase::v06(){}
void CTCPhysicsBase::v07(){}
void CTCPhysicsBase::v08(){}
void CTCPhysicsBase::v09(){}
void CTCPhysicsBase::v10(){}
void CTCPhysicsBase::v11(){}
void CTCPhysicsBase::v12(){}
void CTCPhysicsBase::v13(){}
void CTCPhysicsBase::v14(){}
void CTCPhysicsBase::v15(){}
void CTCPhysicsBase::v16(){}
void CTCPhysicsBase::v17(){}
void CTCPhysicsBase::v18(){}
void CTCPhysicsBase::v19(){}
void CTCPhysicsBase::v20(){}
void CTCPhysicsBase::v21(){}
void CTCPhysicsBase::v22(){}
void CTCPhysicsBase::v23(){}
void CTCPhysicsBase::v24(){}
void CTCPhysicsBase::v25(){}
void CTCPhysicsBase::v26(){}
void CTCPhysicsBase::v27(){}
void CTCPhysicsBase::v28(){}
void CTCPhysicsBase::v29(){}
void CTCPhysicsBase::v30(){}
void CTCPhysicsBase::v31(){}
void CTCPhysicsBase::v32(){}
void CTCPhysicsBase::v33(){}
void CTCPhysicsBase::v34(){}
void CTCPhysicsBase::v35(){}
void CTCPhysicsBase::v36(){}
void CTCPhysicsBase::v37(){}
void CTCPhysicsBase::v38(){}
void CTCPhysicsBase::v39(){}
void CTCPhysicsBase::v40(){}
void CTCPhysicsBase::v41(){}
void CTCPhysicsBase::v42(){}
void CTCPhysicsBase::v43(){}
void CTCPhysicsBase::v44(){}
void CTCPhysicsBase::v45(){}
void CTCPhysicsBase::v46(){}
void CTCPhysicsBase::v47(){}
void CTCPhysicsBase::v48(){}
void CTCPhysicsBase::v49(){}
void CTCPhysicsBase::v50(){}
void CTCPhysicsBase::v51(){}
void CTCPhysicsBase::v52(){}
void CTCPhysicsBase::v53(){}
void CTCPhysicsBase::v54(){}
void CTCPhysicsBase::v55(){}
void CTCPhysicsBase::v56(){}
void CTCPhysicsBase::v58(){}
void CTCPhysicsBase::v59(){}
void CTCPhysicsBase::v60(){}
void CTCPhysicsBase::v61(){}
void CTCPhysicsBase::v62(){}
void CTCPhysicsBase::v63(){}
void CTCPhysicsBase::v64(){}
void CTCPhysicsBase::v65(){}
void CTCPhysicsBase::v66(){}
void CTCPhysicsBase::v67(){}
void CTCPhysicsBase::v68(){}
void CTCPhysicsBase::v69(){}
void CTCPhysicsBase::v71(){}
void CTCPhysicsBase::v72(){}
void CTCPhysicsBase::v73(){}
void CTCPhysicsBase::v74(){}
void CTCPhysicsBase::v75(){}
void CTCPhysicsBase::v76(){}
void CTCPhysicsBase::v77(){}
void CTCPhysicsBase::v78(){}
void CTCPhysicsBase::v79(){}
void CTCPhysicsBase::v80(){}
void CTCPhysicsBase::v82(){}

struct PhysData { bool collides; bool pushable; C3DVector vel; int impulses; C3DVector last; };
static PhysData g_pd[4];
static int idx(CTCPhysicsBase* p);
bool CTCPhysicsBase::GetCollidesWithThings() { return g_pd[idx(this)].collides; }
C3DVector CTCPhysicsBase::GetVelocity() { return g_pd[idx(this)].vel; }
void CTCPhysicsBase::AddAdditionalImpulse(const C3DVector& i) { PhysData& d = g_pd[idx(this)]; d.impulses++; d.last = i; }
bool CTCPhysicsBase::IsPushableByHero() { return g_pd[idx(this)].pushable; }
static CTCPhysicsBase* g_phys[2];
static int idx(CTCPhysicsBase* p) { return p == g_phys[0] ? 0 : 1; }

static bool g_filterAllows = true; static int g_filterCalls = 0;
struct FilterTC { int dummy; };
static FilterTC g_filter;
static CInterfaceMap::SEntry g_entries[2];
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const {
    for (SEntry* e = first; e != last; ++e) if (e->type >= *type) return e;
    return last;
}
bool CTCCollisionFilter::CanCollideWith(CThing*) { g_filterCalls++; return g_filterAllows; }
static float g_turns = 0; static int g_rot = 0;
void C3DVector::RotateAboutZ(float turns) { g_turns = turns; g_rot++; }
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)((const char*)this + 0x140); }
static int g_push = 0; static float g_pushDist = 0;
void CThingAICreature::OnHeroPush(CThing*, float d, const C3DVector&) { g_push++; g_pushDist = d; }
static bool g_creaturePushable = true;
bool __fastcall NCreatureMode::IsPhysicsPushableByHero(const CThing&) { return g_creaturePushable; }
static unsigned long g_seed = 1;
unsigned long* CWorld::GetPRandomSeed() { return &g_seed; }
static CWorld g_world; static CMainGameComponent g_mgc;
CMainGameComponent* g_pMainGameComponent = &g_mgc;
static long g_frame = 3;
long __fastcall GFGetCurrentFrame() { return g_frame; }
float __fastcall GFFloatRandom(float, unsigned long*) { return 9.0f; }

static double ownerBuf[0x40], otherBuf[0x40], pBuf0[0x20], pBuf1[0x20];
static int fails = 0;
static void check(bool c, const char* m) { if (!c) { std::printf("FAIL %s\n", m); fails++; } }
static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

static CThing* owner; static CThing* other;
static void setPos(CThing* t, float x, float y, float z) { C3DVector* p = (C3DVector*)((char*)t + 0x140); p->X = x; p->Y = y; p->Z = z; }
static void reset() {
    std::memset(g_pd, 0, sizeof g_pd);
    g_pd[0].collides = g_pd[1].collides = true; g_pd[1].pushable = true;
    g_filterAllows = true; g_filterCalls = 0; g_rot = 0; g_push = 0; g_creaturePushable = true; g_frame = 3;
}

int main()
{
    g_mgc.World = &g_world;
    std::memset(ownerBuf, 0, sizeof ownerBuf); std::memset(otherBuf, 0, sizeof otherBuf);
    owner = (CThing*)ownerBuf; other = (CThing*)otherBuf;
    // physics objects: need a vptr, so construct via a derived concrete type
    struct P : CTCPhysicsBase {};
    static P p0, p1; g_phys[0] = &p0; g_phys[1] = &p1;
    p0.ParentThing = owner; p0.Radius = 1.0f; p1.ParentThing = other; p1.Radius = 1.0f;
    owner->PhysicsTC = &p0; other->PhysicsTC = &p1;
    owner->m_interfaces.first = owner->m_interfaces.last = g_entries;
    CApplyAdditionalPlayerImpulseIfTooClose f;
    f.Position = C3DVector(0, 0, 0); f.Radius = 0.5f; f.Owner = &p0;   // radii = 1.5, range 1.6

    // 1: vertically apart -> nothing
    reset(); setPos(other, 1, 0, 5); f((CThingCreatureBase*)other);
    check(g_pd[1].impulses == 0, "vertical");
    // 2: too far in XY -> nothing
    reset(); setPos(other, 2, 0, 0); f((CThingCreatureBase*)other);
    check(g_pd[1].impulses == 0, "far");
    // 3: overlapping (dist 1.0, penetration -0.5 -> scale stays 1) -> unit dir * 0.2
    reset(); setPos(other, 0.6f, 0.8f, 0.2f); f((CThingCreatureBase*)other);
    check(g_pd[1].impulses == 1, "push count");
    check(near(g_pd[1].last.X, 0.12f) && near(g_pd[1].last.Y, 0.16f) && near(g_pd[1].last.Z, 0.0f), "push dir");
    check(g_rot == 1 && near(g_turns, 9.0f / 360.0f), "rotation +");
    // 4: just outside radii (dist 1.55, penetration 0.05 -> scale 0.5) and an even frame
    reset(); g_frame = 4; setPos(other, 1.55f, 0, 0); f((CThingCreatureBase*)other);
    check(near(g_pd[1].last.X, 0.1f) && near(g_pd[1].last.Y, 0.0f), "scaled push");
    check(near(g_turns, 9.0f / 360.0f), "rotation sign squared");
    // 5: same spot -> owner velocity * 0.1
    reset(); g_pd[0].vel = C3DVector(3, -2, 1); setPos(other, 0, 0, 0); f((CThingCreatureBase*)other);
    check(near(g_pd[1].last.X, 0.3f) && near(g_pd[1].last.Y, -0.2f) && near(g_pd[1].last.Z, 0.1f), "velocity push");
    // 6: gating
    reset(); g_pd[1].pushable = false; setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other); check(g_pd[1].impulses == 0, "not pushable");
    reset(); g_creaturePushable = false; setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other); check(g_pd[1].impulses == 0, "mode");
    reset(); g_pd[0].collides = false; setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other); check(g_pd[1].impulses == 0, "owner collides");
    // 7: collision filter present and refusing
    g_entries[0].type = 0x4d; g_entries[0].component = (CTCBase*)&g_filter; owner->m_interfaces.last = g_entries + 1;
    owner->m_presentBits[0x4d >> 5] |= 1UL << (0x4d & 31);
    reset(); g_filterAllows = false; setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other);
    check(g_filterCalls == 1 && g_pd[1].impulses == 0, "filter refuses");
    reset(); setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other);
    check(g_filterCalls == 1 && g_pd[1].impulses == 1, "filter allows");
    // 8: hero push notification (filter removed first: on a 64-bit host the flag byte overlays the interface map)
    owner->m_presentBits[0x4d >> 5] = 0;
    ((CThingView*)other)->Type = 1; ((CThingView*)owner)->Flags6c = 8;
    reset(); setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other); check(g_push == 0, "player still");
    ((CThingView*)owner)->MoveVector.X = 1.0f;
    reset(); setPos(other, 0.6f, 0.8f, 0); f((CThingCreatureBase*)other); check(g_push == 1 && near(g_pushDist, 0.6f), "hero push");
    if (fails == 0) std::printf("00726340_TEST PASS\n");
    return fails;
}