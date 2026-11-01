// Behaviour test for 008c9580 CCreatureAction_BanditKingStuck::~CCreatureAction_BanditKingStuck(): members destroyed in reverse declaration order (PObstruction last-declared first), then the base.
#include <cstdio>
#include <new>
static int g_log[8];
static int g_n = 0;
class CThing;
class CBaseIntelligentPointer {
public:
    explicit CBaseIntelligentPointer(int tag) : Tag(tag), PData(0) { }
    virtual ~CBaseIntelligentPointer() { g_log[g_n++] = Tag; }
    int   Tag;
    void* PData;
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    explicit CIntelligentPointer(int tag) : CBaseIntelligentPointer(tag) { }
};
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase() { }
};
class CCombatActionBase : public CCreatureActionBase {
public:
    virtual ~CCombatActionBase() { g_log[g_n++] = 0; }
    unsigned char _body[0x114 - 4];
};
class CCreatureAction_BanditKingStuck : public CCombatActionBase {
public:
    CCreatureAction_BanditKingStuck() : PStuckEffect(1), PObstruction(2) { }
    long TimesToLoop;
    CIntelligentPointer<CThing> PStuckEffect;
    CIntelligentPointer<CThing> PObstruction;
};
static bool Check(int a, int b, int c)
{
    return g_n == 3 && g_log[0] == a && g_log[1] == b && g_log[2] == c;
}
int main()
{
    { CCreatureAction_BanditKingStuck action; }
    if (!Check(2, 1, 0)) { std::printf("008c9580_TEST FAIL\n"); return 1; }
    g_n = 0;
    CCombatActionBase* base = new CCreatureAction_BanditKingStuck;
    delete base;                                    // through the virtual destructor
    if (!Check(2, 1, 0)) { std::printf("008c9580_TEST FAIL\n"); return 1; }
    std::printf("008c9580_TEST PASS\n");
    return 0;
}