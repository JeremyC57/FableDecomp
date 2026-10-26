// Behaviour test for 007b2ab0: src/007b2ab0.cpp copied verbatim; the def lookup, Create, the money component, the
// random rotation and the physics stubbed so the bag drop (index from the name, at the parent's position, gold =
// bribe pool, rotated velocity, set moving) and the early-outs are checked.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CTCGuard::OnDie() -- a guard holding bribe money drops it: create a small gold bag at his position, put the bribe pool in its money component and throw the bag off in a random direction
// Retry 10b (accessor sweep, FIXES B-4/B-5): the parent thing is read through an inline accessor; the plain member read gives the parked register difference.
// Leaf: OnDie
//
// Retail 0x007B2AB0, 320 bytes incl. padding (plain `ret`). Ego_r.pdb `?OnDie@CTCGuard@@UAEXXZ`.
// Callees: CCharString(const char*, long) @ 0x0099EBF0 ("OBJECT_GOLDBAG_SMALL_WITH_COINS" @ 0x0125D760, "" @ 0x0122D70E),
// ~CCharString @ 0x0099EAE0, CTCBase::GetDefinitionManager @ 0x006869D0, CDefinitionManager::GetDefGlobalIndexFromName
// @ 0x009AD410, CThing::GetPosition @ 0x004C73D0, CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*,
// unsigned long, const CCharString&) @ 0x00703210 (static __fastcall), CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound
// @ 0x0040F020 (inlined GetInterface for CTCMoney, type 0xCC = bit 12 of the bitset word +0x38, `test ah,10h`),
// CTCBase::GetPRandomSeed @ 0x00686DA0, GFFloatRandom(float, unsigned long*) @ 0x004978A0 (__fastcall), C3DVector::RotateXY
// @ 0x00A14040, CTCPhysicsBase::SetAsMoving(bool) @ 0x006B0130.
// Layout: Ego_r.pdb CTCGuard: PDef +0x0c, BribePool +0x10; CTCMoney gold +0x0c (landed model); retail CThing bitset +0x20,
// interface map +0x44, PhysicsTC +0x60, dead bit +0x91; the physics component's velocity at +0x28.
// Shape: the def name is a named local in its own block (the definition-manager getter is called after the name is
// pushed), the empty script name an implicit conversion temporary (built first, re-addressed with `lea`); the money component is null-tested after the lookup; the throw velocity is a C3DVector(0.3, 0, 0.4) local rotated
// by a random angle and copied into the physics component (a named local) with the implicit assignment (`lea edx,[ecx+28h]` then three dword stores).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    void RotateXY(float angle);                             // 0x00A14040
};
class CRightHandedSet;

float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0

class CTCBase;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};

class CTCPhysicsBase { public:
    unsigned char _pad00[0x28];
    C3DVector     Velocity;                                 // +0x28
    void SetAsMoving(bool moving);                          // 0x006B0130
};

struct CTCMoney {
    unsigned char _pad00[0x0c];
    long          Gold;                                     // +0x0c
};

class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                         // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                              // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 bit 0 Dead
    const C3DVector& GetPosition() const;                   // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingObject : public CThing {
public:
    static CThingObject* __fastcall Create(long defIndex, const C3DVector& position, long player,
                                           const CRightHandedSet* orientation, unsigned long uid,
                                           const CCharString& scriptName);   // 0x00703210
};

class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410
};

class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    CThing* GetParentThing() const { return ParentThing; }
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CDefinitionManager* GetDefinitionManager();             // 0x006869D0
    unsigned long* GetPRandomSeed();                        // 0x00686DA0
};

class CGuardDef;
class CTCGuard : public CTCBase {
public:
    const CGuardDef* PDef;                                  // +0x0c CDefPointer<CGuardDef const>
    long BribePool;                                         // +0x10
    void OnDie();
};

void CTCGuard::OnDie()
{
    if (BribePool > 0) {
        long index;
        {
            CCharString name("OBJECT_GOLDBAG_SMALL_WITH_COINS", -1);
            index = GetDefinitionManager()->GetDefGlobalIndexFromName(name);
        }
        if (index > 0) {
            CThingObject* bag = CThingObject::Create(index, GetParentThing()->GetPosition(), 5, 0, 0, "");
            if (bag && !(bag->Flags91 & 1)) {
                CTCMoney* money;
                if (bag->GetInterface(&money, 0xcc) && money)
                    money->Gold = BribePool;
                C3DVector velocity(0.3f, 0.0f, 0.4f);
                velocity.RotateXY(GFFloatRandom(1.0f, GetPRandomSeed()));
                CTCPhysicsBase* physics = bag->PhysicsTC;
                physics->Velocity = velocity;
                physics->SetAsMoving(true);
            }
        }
    }
}
// ---- end source ----
void CTCBase::v00() {}
static int g_ctors = 0, g_dtors = 0; static long g_index = 9;
CCharString::CCharString(const char* text, long) : PStringData((void*)text) { ++g_ctors; }
CCharString::~CCharString() { ++g_dtors; }
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) { return strcmp((const char*)name.PStringData, "OBJECT_GOLDBAG_SMALL_WITH_COINS") == 0 ? g_index : -1; }
static CDefinitionManager g_dm;
CDefinitionManager* CTCBase::GetDefinitionManager() { return &g_dm; }
static unsigned long g_seed = 1;
unsigned long* CTCBase::GetPRandomSeed() { return &g_seed; }
static float g_randRange = -1;
float __fastcall GFFloatRandom(float range, unsigned long* seed) { g_randRange = range; return seed == &g_seed ? 0.25f : 0.0f; }
static float g_rotAngle = -1;
void C3DVector::RotateXY(float angle) { g_rotAngle = angle; float x = X; X = -Y; Y = x; }
static int g_posStore;
const C3DVector& CThing::GetPosition() const { return *(const C3DVector*)&g_posStore; }
static CTCMoney g_money;
static CInterfaceMap::SEntry g_entries[1] = { { 0xcc, (CTCBase*)&g_money } };
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return *type <= 0xcc ? first : last; }
static unsigned char g_bagRaw[sizeof(CThingObject)]; static unsigned char g_physicsRaw[sizeof(CTCPhysicsBase)]; static CTCPhysicsBase& g_physics = *(CTCPhysicsBase*)g_physicsRaw; static int g_creates = 0; static long g_createIndex = 0;
static bool g_createOk = true;
CThingObject* __fastcall CThingObject::Create(long defIndex, const C3DVector& position, long player, const CRightHandedSet* o, unsigned long uid, const CCharString& name)
{
    ++g_creates; g_createIndex = defIndex;
    if (!g_createOk || position.X != 0 || player != 5 || o || uid) return 0;
    return (CThingObject*)g_bagRaw;
}
static int g_moving = 0;
void CTCPhysicsBase::SetAsMoving(bool moving) { if (moving) ++g_moving; }
int main()
{
    CThingObject* bag = (CThingObject*)g_bagRaw; memset(g_bagRaw, 0, sizeof(g_bagRaw));
    bag->m_interfaces.first = g_entries; bag->m_interfaces.last = g_entries + 1; bag->PhysicsTC = &g_physics;
    bag->m_presentBits[0xcc >> 5] |= 1UL << (0xcc & 31);
    static CThing parent; static unsigned char raw[sizeof(CTCGuard)]; memset(raw, 0, sizeof(raw));
    CTCGuard* g = (CTCGuard*)raw; g->ParentThing = &parent;
    g->OnDie(); CHECK(g_creates == 0 && g_ctors == 0);                 // no bribe pool
    g->BribePool = 40;
    g->OnDie();
    CHECK(g_ctors >= 1 && g_ctors == g_dtors && g_creates == 1 && g_createIndex == 9 && g_money.Gold == 40);
    CHECK(g_randRange == 1.0f && g_rotAngle == 0.25f && g_physics.Velocity.X == -0.0f && g_physics.Velocity.Y == 0.3f && g_physics.Velocity.Z == 0.4f && g_moving == 1);
    g_index = 0;
    g->OnDie(); CHECK(g_creates == 1);                                  // unknown def
    g_index = 9; bag->Flags91 = 1; g_money.Gold = 0;
    g->OnDie(); CHECK(g_creates == 2 && g_money.Gold == 0 && g_moving == 1);   // dead bag
    if (fails == 0) printf("007b2ab0_TEST PASS\n"); else printf("007b2ab0_TEST FAIL\n");
    return fails;
}