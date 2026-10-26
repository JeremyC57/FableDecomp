#include <cstdio>
#include <cstring>
static char g_log[512]; static int g_n;
static void Log(const char* s) { int l = (int)std::strlen(s); if (g_n + l + 1 < (int)sizeof(g_log)) { std::memcpy(g_log + g_n, s, l); g_n += l; g_log[g_n++] = ' '; g_log[g_n] = 0; } }
static int Check(const char* addr, const char* want) { if (std::strcmp(g_log, want) != 0) { std::printf("%s_TEST FAIL\n%s\n", addr, g_log); return 1; } std::printf("%s_TEST PASS\n", addr); return 0; }
class CThing { public:};
struct CBaseIntelligentPointer {
    virtual void v00() {} void* m_pItem;
    CBaseIntelligentPointer() : m_pItem(0) { Log("ctor"); }
    CThing* GetPItem() const { return (CThing*)m_pItem; }
    void SetPItem(CThing* p) { m_pItem = p; Log(p ? "set-thing" : "set-null"); }
    ~CBaseIntelligentPointer() { Log("dtor"); }
};
struct CIntelligentPointer : CBaseIntelligentPointer {
    CIntelligentPointer() { }
    CIntelligentPointer(CThing* p) : CBaseIntelligentPointer() { SetPItem(p); }
    CIntelligentPointer& operator=(const CIntelligentPointer& o) { SetPItem(o.GetPItem()); return *this; }
};
class CAIStateGroupBase { public: virtual ~CAIStateGroupBase() {} virtual void Cleanup() {} virtual void CleanupLeaf() { Log("leaf"); } unsigned char _pad[0x10]; };
struct CAIStateGroupBehaviourBase : CAIStateGroupBase { CIntelligentPointer PTargetThing; virtual void Cleanup() { PTargetThing = 0; CleanupLeaf(); } };
int main()
{
    CThing t; CAIStateGroupBehaviourBase b; b.PTargetThing.m_pItem = &t; g_n = 0; g_log[0] = 0;
    b.Cleanup();
    if (b.PTargetThing.m_pItem != 0) return Check("00957790", "x");
    return Check("00957790", "ctor set-null set-null dtor leaf ");
}