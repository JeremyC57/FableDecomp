// Behaviour test for NCreatureMode::CModeOutOfActionBase::~CModeOutOfActionBase (0x008D7580): the emitter thing is killed
// (immediately) only when the pointer resolves and the thing is not already dead; then the pointer and the stand base are destroyed.
#include <cstdio>
static int g_log[16]; static int g_n = 0;
class CThing {
public:
    CThing() : Dead(0), KilledWith(-1) {}
    unsigned char Dead : 1;
    int KilledWith;
    bool IsDead() const { return Dead; }
    void Kill(bool immediately) { KilledWith = immediately ? 1 : 0; g_log[g_n++] = 30; }
};
struct CBaseIntelligentPointer {
    CThing* Item; int gets;
    CBaseIntelligentPointer() : Item(0), gets(0) {}
    virtual ~CBaseIntelligentPointer() { g_log[g_n++] = 2; }
    CThing* GetPItem() { ++gets; return Item; }
};
struct CModeStandBase { virtual ~CModeStandBase() { g_log[g_n++] = 1; } };
struct CModeOutOfActionBase : CModeStandBase {
    long RecoveryCount; bool Recoverable; CBaseIntelligentPointer PEmitter;
    virtual ~CModeOutOfActionBase()
    {
        if (PEmitter.GetPItem() && !PEmitter.GetPItem()->IsDead())
            PEmitter.GetPItem()->Kill(true);
    }
};
int main()
{
    CThing alive, dead; dead.Dead = 1;
    { CModeOutOfActionBase m; m.PEmitter.Item = &alive; }
    if (alive.KilledWith != 1 || g_n != 3 || g_log[0] != 30 || g_log[1] != 2 || g_log[2] != 1) { std::printf("008d7580_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CModeOutOfActionBase m; m.PEmitter.Item = &dead; }
    if (dead.KilledWith != -1 || g_n != 2) { std::printf("008d7580_TEST FAIL\n"); return 1; }
    g_n = 0;
    { CModeOutOfActionBase m; }
    if (g_n != 2 || g_log[0] != 2) { std::printf("008d7580_TEST FAIL\n"); return 1; }
    std::printf("008d7580_TEST PASS\n");
    return 0;
}