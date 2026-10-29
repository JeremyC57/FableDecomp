// Behaviour test for 0083cd70 CThingSwitch::WorldUpdate (every fifth frame, staggered by UniqueID, run the def's trigger)
#include <cstdio>
#ifdef _MSC_VER
typedef unsigned __int64 U64;
#else
typedef unsigned long long U64;
#endif
static int g_destroyed = 0;
static long g_frame = 0;
struct CRefCounted { long m_count; void DecRef() { --m_count; } long GetRefCount() const { return m_count; } };
struct CDefVirtual { virtual void v00() {} virtual void Destroy() { ++g_destroyed; } virtual ~CDefVirtual() {} };
struct CDefBase : CDefVirtual, CRefCounted {
    void ReleaseTemporary() { DecRef(); if (GetRefCount() == 0) Destroy(); }
};
class CThingSwitchDef : public CDefBase { public: long TriggerType; };
struct CSwitchDefPointer {
    CThingSwitchDef* m_p;
    explicit CSwitchDefPointer(CThingSwitchDef* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CSwitchDefPointer(const CSwitchDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CSwitchDefPointer() { if (m_p) { m_p->ReleaseTemporary(); m_p = 0; } }
    CThingSwitchDef* operator->() const { return m_p; }
};
class CWorld { public: static long __fastcall GetFrame() { return g_frame; } };
static CWorld g_world;
class CThingSwitch { public:
    U64 UniqueID;
    unsigned char baseResult;
    CThingSwitchDef* def;
    int calls[6];
    unsigned char GameObjectWorldUpdate() { return baseResult; }
    CWorld* GetWorld() { return &g_world; }
    CSwitchDefPointer GetSwitchDef() { return CSwitchDefPointer(def); }
    void SwitchTriggerPlayerOnlyOnceInArea() { ++calls[1]; }
    void SwitchTriggerPlayerOnlyMultipleTimesInArea() { ++calls[2]; }
    void TriggerPlayerResetWhenLeaves() { ++calls[3]; }
    void TriggerSetPlayerAreaName() { ++calls[4]; }
    void TriggerSetEnvironment() { ++calls[5]; }
    unsigned char WorldUpdate();
};
unsigned char CThingSwitch::WorldUpdate()
{
    unsigned char result = GameObjectWorldUpdate();
    if (result == 1)
        return result;
    CWorld* world = GetWorld();
    U64 id = UniqueID;
    if ((world->GetFrame() + id) % 5 == 0) {
        switch (GetSwitchDef()->TriggerType) {
        case 1: SwitchTriggerPlayerOnlyOnceInArea(); return 0;
        case 2: SwitchTriggerPlayerOnlyMultipleTimesInArea(); return 0;
        case 3: TriggerPlayerResetWhenLeaves(); return 0;
        case 4: TriggerSetPlayerAreaName(); return 0;
        case 5: TriggerSetEnvironment(); return 0;
        }
    }
    return 0;
}
int main()
{
    CThingSwitchDef def; def.m_count = 1; def.TriggerType = 4;
    CThingSwitch s; s.UniqueID = 3; s.baseResult = 0; s.def = &def;
    for (int i = 0; i < 6; ++i) s.calls[i] = 0;
    for (g_frame = 0; g_frame < 10; ++g_frame)
        if (s.WorldUpdate() != 0) { std::printf("0083cd70_TEST FAIL\n"); return 1; }
    if (s.calls[4] != 2 || s.calls[1] || s.calls[5] || def.m_count != 1 || g_destroyed) { std::printf("0083cd70_TEST FAIL\n"); return 1; }
    s.baseResult = 1; g_frame = 2;   // base says "removed": no trigger
    if (s.WorldUpdate() != 1 || s.calls[4] != 2) { std::printf("0083cd70_TEST FAIL\n"); return 1; }
    s.baseResult = 2; def.TriggerType = 7; g_frame = 7;   // out-of-range trigger type
    if (s.WorldUpdate() != 0 || s.calls[4] != 2 || def.m_count != 1) { std::printf("0083cd70_TEST FAIL\n"); return 1; }
    def.TriggerType = 5; s.UniqueID = (U64)1 << 32; g_frame = 4;   // (4 + 2^32) % 5 == 0
    s.WorldUpdate();
    if (s.calls[5] != 1) { std::printf("0083cd70_TEST FAIL\n"); return 1; }
    std::printf("0083cd70_TEST PASS\n");
    return 0;
}