// Behaviour test for 0066a530 CCombatAction_GenericStrikeResponseKnockdown::~CCombatAction_GenericStrikeResponseKnockdown(): members destroyed in reverse declaration order (PThingThatHit last-declared first), then the base.
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
class CCharString {
public:
    explicit CCharString(int tag) : Tag(tag), PStringData(0) { }
    ~CCharString() { g_log[g_n++] = Tag; }
    int   Tag;
    void* PStringData;
};
class CCombatAction_GenericStrikeResponseKnockdown : public CCombatActionBase {
public:
    CCombatAction_GenericStrikeResponseKnockdown() : AnimName(1), PThingThatHit(2) { }
    CCharString AnimName;
    CIntelligentPointer<const CThing> PThingThatHit;
};
static bool Check(int a, int b, int c)
{
    return g_n == 3 && g_log[0] == a && g_log[1] == b && g_log[2] == c;
}
int main()
{
    { CCombatAction_GenericStrikeResponseKnockdown action; }
    if (!Check(2, 1, 0)) { std::printf("0066a530_TEST FAIL\n"); return 1; }
    g_n = 0;
    CCombatActionBase* base = new CCombatAction_GenericStrikeResponseKnockdown;
    delete base;                                    // through the virtual destructor
    if (!Check(2, 1, 0)) { std::printf("0066a530_TEST FAIL\n"); return 1; }
    std::printf("0066a530_TEST PASS\n");
    return 0;
}