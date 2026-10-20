#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    float Dot(const C3DVector& v) const { return X * v.X + Y * v.Y + Z * v.Z; }
};
enum ECollideType { COLLIDE_NONE = 0, COLLIDE_LANDSCAPE = 1 };
struct CTCBase;
struct CInterfaceMap {                          // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44: {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    SEntry* LowerBound(const long* type) const; // 0x004365B0
};
struct CThing;
struct CCreatureMode {
    virtual void v00();
    virtual long GetType() const;               // slot 1
};
struct CTCCreatureModeManager {
    CCreatureMode* GetCurrentMode() const;      // 0x0073A6E0
};
struct CTCHitOnCollision {
    void OnCollision(C3DVector* position, C3DVector* normal, CThing* other);   // 0x007C81F0
};

struct CThing {
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char Dead : 1;                     // +0x91 bit 0
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
    }
    template<class T> __forceinline const T* GetTC(long interfaceType) const
    {
        long type = interfaceType;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (const T*)it->component;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

struct CThingGameObject : CThing {
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);     // 0x00839080
};

struct CCharString {
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
};
struct CBaseIntelligentPointer {
    void* m_vtable;
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CThingCreatureBase;
struct CCombatActionBase {                      // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[0x114 - 4];
    ~CCombatActionBase();                       // 0x0062E900
};
struct CCombatAction_GenericStrikeResponseKnockdown : CCombatActionBase {
    CCharString             AnimName;           // +0x114
    CBaseIntelligentPointer PThingThatHit;      // +0x118
    unsigned char           _hitInfo[0x1c];     // +0x120 PointOfHit, GotPointOfHit, DirectionOfHit, flags (PDB packing; size 0x13c)
    CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase* creature, CThing* hitter, C3DVector* point,
                                                 C3DVector* direction);   // 0x008679C0
};

struct CThingCreatureBase : CThingGameObject {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
    void OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other);
};

void CThingCreatureBase::OnCollision(ECollideType type, C3DVector* position, C3DVector* normal, CThing* other)
{
    CThingGameObject::OnCollision(type, position, normal, other);
    if (HasTC(0x31)) {
        if (GetTC<CTCCreatureModeManager>(0x31)->GetCurrentMode()->GetType() == 0x42
            && normal->Dot(C3DVector(0.0f, 0.0f, 1.0f)) < 0.866f)
            SetCurrentAction(CCombatAction_GenericStrikeResponseKnockdown(this, other, position, normal));
    }
    if (!Dead && other && !other->Dead && HasTC(0xC8))
        GetTC<CTCHitOnCollision>(0xC8)->OnCollision(position, normal, other);
}

static int g_baseCalls, g_hitCalls, g_actionCalls, g_dtorCalls, g_modeType;
static CThing* g_hitOther;
static C3DVector* g_hitPos;
void CThingGameObject::OnCollision(ECollideType, C3DVector*, C3DVector*, CThing*) { ++g_baseCalls; }
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    SEntry* it = first;
    while (it != last && it->type < *type) ++it;
    return it;
}
void CCreatureMode::v00() {}
long CCreatureMode::GetType() const { return g_modeType; }
static CCreatureMode g_mode;
CCreatureMode* CTCCreatureModeManager::GetCurrentMode() const { return &g_mode; }
void CTCHitOnCollision::OnCollision(C3DVector* position, C3DVector*, CThing* other) { ++g_hitCalls; g_hitOther = other; g_hitPos = position; }
CCharString::~CCharString() { ++g_dtorCalls; }
CBaseIntelligentPointer::~CBaseIntelligentPointer() { ++g_dtorCalls; }
void CCombatActionBase::v00() {}
CCombatActionBase::~CCombatActionBase() { ++g_dtorCalls; }
static CThing* g_actionHitter;
CCombatAction_GenericStrikeResponseKnockdown::CCombatAction_GenericStrikeResponseKnockdown(CThingCreatureBase*, CThing* hitter,
                                                                                         C3DVector*, C3DVector*)
{
    g_actionHitter = hitter;
}
bool CThingCreatureBase::SetCurrentAction(const CCombatActionBase&) { ++g_actionCalls; return true; }

static CTCCreatureModeManager g_modeManager;
static CTCHitOnCollision g_hitComponent;

static void Reset(CThingCreatureBase& c, CInterfaceMap::SEntry* entries, int count)
{
    std::memset(c.m_presentBits, 0, sizeof(c.m_presentBits));
    c.m_interfaces.first = entries;
    c.m_interfaces.last = entries + count;
    for (int i = 0; i < count; ++i)
        c.m_presentBits[entries[i].type >> 5] |= 1UL << (entries[i].type & 31);
    c.Dead = 0;
    g_baseCalls = g_hitCalls = g_actionCalls = g_dtorCalls = 0;
    g_hitOther = 0; g_actionHitter = 0;
}

int main()
{
    int fails = 0;
    CInterfaceMap::SEntry entries[2];
    entries[0].type = 0x31; entries[0].component = (CTCBase*)&g_modeManager;
    entries[1].type = 0xC8; entries[1].component = (CTCBase*)&g_hitComponent;
    CThingCreatureBase c;
    CThingCreatureBase other;
    other.Dead = 0;
    C3DVector pos(1.0f, 2.0f, 3.0f);
    C3DVector wall(1.0f, 0.0f, 0.0f);
    C3DVector floor(0.0f, 0.0f, 1.0f);

    // knockdown mode against a wall: action set, temporary fully destroyed, hit component called
    Reset(c, entries, 2); g_modeType = 0x42;
    c.OnCollision(COLLIDE_LANDSCAPE, &pos, &wall, &other);
    if (g_baseCalls != 1 || g_actionCalls != 1 || g_dtorCalls != 3 || g_actionHitter != &other) ++fails;
    if (g_hitCalls != 1 || g_hitOther != &other || g_hitPos != &pos) ++fails;
    // floor-like normal: no knockdown
    Reset(c, entries, 2); g_modeType = 0x42;
    c.OnCollision(COLLIDE_LANDSCAPE, &pos, &floor, &other);
    if (g_actionCalls != 0 || g_dtorCalls != 0 || g_hitCalls != 1) ++fails;
    // other mode: no knockdown
    Reset(c, entries, 2); g_modeType = 0x41;
    c.OnCollision(COLLIDE_LANDSCAPE, &pos, &wall, &other);
    if (g_actionCalls != 0 || g_hitCalls != 1) ++fails;
    // no other thing / dead other / dead self: no hit component call
    Reset(c, entries, 2); g_modeType = 0;
    c.OnCollision(COLLIDE_NONE, &pos, &wall, 0);
    if (g_hitCalls != 0 || g_baseCalls != 1) ++fails;
    Reset(c, entries, 2); other.Dead = 1;
    c.OnCollision(COLLIDE_NONE, &pos, &wall, &other);
    if (g_hitCalls != 0) ++fails;
    other.Dead = 0;
    Reset(c, entries, 2); c.Dead = 1;
    c.OnCollision(COLLIDE_NONE, &pos, &wall, &other);
    if (g_hitCalls != 0) ++fails;
    // no components at all: only the base call
    Reset(c, entries, 0); g_modeType = 0x42;
    c.OnCollision(COLLIDE_LANDSCAPE, &pos, &wall, &other);
    if (g_baseCalls != 1 || g_actionCalls != 0 || g_hitCalls != 0) ++fails;

    if (fails == 0) std::printf("00667310_TEST PASS\n");
    else std::printf("00667310_TEST FAIL %d\n", fails);
    return fails;
}