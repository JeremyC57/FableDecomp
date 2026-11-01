// Behaviour test for 00628ee0 CCreatureAction_GiveItemToThing::~CCreatureAction_GiveItemToThing(): members destroyed in reverse declaration order (PItemBeingGiven last-declared first), then the base.
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
    virtual ~CCreatureActionBase() { g_log[g_n++] = 0; }
    unsigned char _body[0xa8 - 4];
};
class CCreatureAction_GiveItemToThing : public CCreatureActionBase {
public:
    CCreatureAction_GiveItemToThing() : PReceivingThing(1), PItemBeingGiven(2) { }
    CIntelligentPointer<CThing> PReceivingThing;
    CIntelligentPointer<CThing> PItemBeingGiven;
};
static bool Check(int a, int b, int c)
{
    return g_n == 3 && g_log[0] == a && g_log[1] == b && g_log[2] == c;
}
int main()
{
    { CCreatureAction_GiveItemToThing action; }
    if (!Check(2, 1, 0)) { std::printf("00628ee0_TEST FAIL\n"); return 1; }
    g_n = 0;
    CCreatureActionBase* base = new CCreatureAction_GiveItemToThing;
    delete base;                                    // through the virtual destructor
    if (!Check(2, 1, 0)) { std::printf("00628ee0_TEST FAIL\n"); return 1; }
    std::printf("00628ee0_TEST PASS\n");
    return 0;
}