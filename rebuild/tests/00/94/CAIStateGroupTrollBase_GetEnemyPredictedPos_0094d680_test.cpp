// Behaviour test for 0094d680: src/0094d680.cpp copied verbatim; GetEnemyPredictedPos returns the enemy position plus
// ten times (physics position - enemy position) scaled by the troll throw def's lead time (0 without the sub-def); the
// sub-def is released.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
// ---- source (verbatim) ----
// CAIStateGroup_TrollBase::GetEnemyPredictedPos() -- predict where the enemy will be when a thrown rock lands: its
// position plus ten times the physics displacement scaled by the troll def's lead time (0 without the sub-def).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094D680, length 249 bytes (+ int3 padding), C3DVector by hidden
// pointer (`ret 4`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PEnemy x3), CThing::GetPosition @ 0x004C73D0 (x2),
// CDefClassBase::GetSubDef<...> @ 0x008C78F0 on the creature's def (+0x70; manifest label propagated), the def's
// virtual slot 1 (destroy on the last release). Constant 10.0f @ 0x01237F0C.
// Shape: GetSubDef is a same-TU noinline stand-in body that writes only the out slot (the holder's word is not
// address-exposed, the def lands in ebx); leadTime is declared before the holder (its zero store goes first); the final
// sum builds its three components through an aggregate initialiser (evaluated x, y, z, stored after `fxch st(2)`),
// while the subtraction and scaling use the member-wise constructor.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CWorld;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound (this TU's copy) -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020 / 0x004365B0 (per-TU copies)
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x60 - 0x4c];
    void* m_pSub60;                             // +0x60 (typed per use below)
    unsigned char _pad2[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    unsigned char m_flags92;                    // +0x92
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    bool TestTC(long interfaceType) const       // std::bitset::test: `shr ; and cl,1` (kept as a byte when reused)
    {
        return ((m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1) != 0;
    }
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
    // entry-returning lookup (the caller tests HasTC first and uses the entry's component directly)
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
    // the bitset::test form: the test's bool is materialised (`shr ; and dl,1 ; mov [esp+x],dl`) and returned
    template<class T> __forceinline bool GetInterfaceT(T** ppOut, long interfaceType)
    {
        bool has = TestTC(interfaceType);
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
        return has;
    }
    // the ternary form (landed 0x00954690): the end/hit choice is ONE ternary, so the component lands straight in
    // the register of its use (ecx for a call on it); the miss path reads the never-written slot
    template<class T> __forceinline bool GetInterfaceX(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    // the KidCleanup form (landed 0x00915190): `if (mask) *pp = Lookup();` with an entry-value lookup
    __forceinline CTCBase* LookupValue(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap::SEntry* p = m_interfaces.LowerBound(&type);
        if (p == m_interfaces.last) p = m_interfaces.last;
        else if (p->type > interfaceType) p = m_interfaces.last;
        return p->component;
    }
    template<class T> __forceinline void GetInterfaceK(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            *ppOut = (T*)LookupValue(interfaceType);
    }
    // a NON-template lookup core shared by every interface type (its key local packs into one frame slot across
    // several inlined lookups); the typed wrappers only cast
    __forceinline bool GetInterfaceCore(CTCBase** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = ((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template<class T> __forceinline bool GetInterfaceN(T** ppOut, long interfaceType)
    {
        return GetInterfaceCore((CTCBase**)ppOut, interfaceType);
    }
    // GetInterface<T>() returning the component: hit = the entry's component, miss = the never-written local
    template<class T> __forceinline T* PeekInterface(long interfaceType)
    {
        T* component;
        if (HasTC(interfaceType))
            component = (T*)FindEntry(interfaceType)->component;
        return component;
    }
    // the out-parameter form whose result is ignored (no bool to keep: the component lands in its use register)
    template<class T> __forceinline void GetInterfaceV(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13();
    virtual long GetTimeToInitiate();           // slot 14 (+0x38)
    virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    unsigned long* GetPWorldSeed();             // 0x008FCFD0
    // the landed GetRandomObject random step: seed = ror13(seed*0x24A1 + 0x24DF), r = seed % range
    unsigned long Random(unsigned long range)
    {
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        return rotated % range;
    }
};

// vtable padding helpers: V4/V16 declare 4/16 placeholder virtual slots
#define V4(n) virtual void n##0(); virtual void n##1(); virtual void n##2(); virtual void n##3();
#define V16(n) V4(n##a) V4(n##b) V4(n##c) V4(n##d)
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)
class C3DVector { public:
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector(const float* v) : x(v[0]), y(v[1]), z(v[2]) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(x - o.x, y - o.y, z - o.z); }
    C3DVector operator+(const C3DVector& o) const { float r[3] = { x + o.x, y + o.y, z + o.z }; return C3DVector(r); }
    C3DVector operator*(float f) const { return C3DVector(x * f, y * f, z * f); }
};
struct CThingPos : CThing {
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CTCPhysicsPos {                          // the enemy's +0x60 physics sub-object
    unsigned char _pad[0x18];
    C3DVector m_position;                       // +0x18
};
struct CTrollThrowDef {
    virtual void v00();
    virtual void Destroy();                     // slot 1
    long m_refCount;                            // +0x04
    unsigned char _pad08[0x44 - 0x08];
    float LeadTime;                             // +0x44 (descriptive)
    void Release() { if (--m_refCount == 0) Destroy(); }
};
template <class T>
class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }               // a plain holder filled by GetSubDef
};
template <class T>
struct CDefReleaser {                           // releases a def copied out of its holder when the scope ends
    T* Object;
    CDefReleaser(T* object) : Object(object) { }
    ~CDefReleaser() { if (Object) Object->Release(); }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CTrollThrowDef>* out);   // 0x008C78F0
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CTrollThrowDef>* out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out->Object = (CTrollThrowDef*)def;
    return true;
}
#pragma auto_inline(on)
inline CDefClassBase* GetThingDef(CThing* thing) { return *(CDefClassBase**)((unsigned char*)thing + 0x70); }

struct CAIStateGroup_TrollBase : CAIStateGroupBase {
    CBaseIntelligentPointer PEnemy;             // +0x14 CIntelligentPointer<CThingCreatureBase>
    C3DVector GetEnemyPredictedPos();
};

C3DVector CAIStateGroup_TrollBase::GetEnemyPredictedPos()
{
    CTCPhysicsPos* physics = (CTCPhysicsPos*)PEnemy.GetPItem()->m_pSub60;
    const C3DVector& enemyPos = ((CThingPos*)PEnemy.GetPItem())->GetPosition();
    C3DVector movement = physics->m_position - enemyPos;
    float leadTime = 0.0f;
    CDefPointer<CTrollThrowDef> holder;
    bool found = GetThingDef(PBrain->PCreature)->GetSubDef(&holder);
    CDefReleaser<CTrollThrowDef> def(holder.Object);
    if (found)
        leadTime = def.Object->LeadTime;
    C3DVector offset = movement * 10.0f * leadTime;
    return offset + ((CThingPos*)PEnemy.GetPItem())->GetPosition();
}
// ---- harness ----
static int g_destroyed = 0, g_posCalls = 0;
static void* g_subDef = 0; static const void* g_findOwner = 0;
static C3DVector g_enemyPos(1.0f, 2.0f, 3.0f);
static CThing* g_enemy = 0;
void CTrollThrowDef::v00() { }
void CTrollThrowDef::Destroy() { ++g_destroyed; }
void* Find_CDefClassBase_GetSubDef(const void* owner)
{ g_findOwner = owner; if (g_subDef) ++((CTrollThrowDef*)g_subDef)->m_refCount; return g_subDef; }
const C3DVector& CThingPos::GetPosition() { ++g_posCalls; CHECK(this == (CThingPos*)g_enemy); return g_enemyPos; }
CThing* CBaseIntelligentPointer::GetPItem() const { return m_pItem; }
static bool near(float a, float b) { float d = a - b; return d < 1e-4f && d > -1e-4f; }
int main()
{
    static unsigned char enemyBuf[sizeof(CThing)]; CThing& enemy = *(CThing*)enemyBuf; g_enemy = &enemy;
    static unsigned char physBuf[sizeof(CTCPhysicsPos)]; CTCPhysicsPos& phys = *(CTCPhysicsPos*)physBuf;
    phys.m_position = C3DVector(1.5f, 1.0f, 3.25f);
    enemy.m_pSub60 = &phys;
    static unsigned char creatureBuf[0x80]; CThing* creature = (CThing*)creatureBuf;
    static unsigned char defClassBuf[16]; *(void**)(creatureBuf + 0x70) = defClassBuf;
    CAIBrain brain; brain.PCreature = creature;
    static unsigned char groupBuf[sizeof(CAIStateGroup_TrollBase)];
    CAIStateGroup_TrollBase& group = *(CAIStateGroup_TrollBase*)groupBuf;
    group.PBrain = &brain; group.PEnemy.m_pItem = &enemy;
    static CTrollThrowDef defObj; CTrollThrowDef* def = &defObj; def->m_refCount = 1; def->LeadTime = 0.5f;

    g_subDef = def;
    C3DVector r = group.GetEnemyPredictedPos();
    // movement (0.5, -1, 0.25) * 10 * 0.5 = (2.5, -5, 1.25); + enemy (1, 2, 3)
    CHECK(near(r.x, 3.5f) && near(r.y, -3.0f) && near(r.z, 4.25f));
    CHECK(g_findOwner == defClassBuf && g_posCalls == 2 && def->m_refCount == 1 && g_destroyed == 0);

    // no sub-def: lead time 0, the enemy position comes back
    g_subDef = 0; g_posCalls = 0;
    r = group.GetEnemyPredictedPos();
    CHECK(r.x == 1.0f && r.y == 2.0f && r.z == 3.0f && g_posCalls == 2 && g_destroyed == 0);

    // last reference: destroyed
    g_subDef = def; def->m_refCount = 0;
    r = group.GetEnemyPredictedPos();
    CHECK(near(r.x, 3.5f) && def->m_refCount == 0 && g_destroyed == 1);

    if (fails == 0) printf("0094d680_TEST PASS\n");
    return fails;
}