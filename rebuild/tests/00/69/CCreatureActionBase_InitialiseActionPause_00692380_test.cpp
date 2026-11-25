// Behaviour test for 0x00692380 CCreatureActionBase::InitialiseActionPause (NF415: linked): links the row object (the function is declared
// here, not defined) and defines its callees under the row's spellings. A positive action id (virtual slot 7) registers the action with
// the description ACTION_PAUSE_ON_THIS_FRAME (0x013BAC90); an id <= 0 registers nothing.
#include <cstdio>

class CCharString { public: int Tag; };
extern const CCharString ACTION_PAUSE_ON_THIS_FRAME = { 1 };          // the row's data symbol, defined here
extern const CCharString TARGETING_CURSOR_POSITION_NAME = { 2 };      // a decoy (the mutant control passes this one)
class CCreatureActionBase;
struct ActionRegistrar { void RegisterAction(int actionId, void* description); };
extern "C" ActionRegistrar* __cdecl GetActionRegistrar();
void __fastcall CCreatureActionBase_InitialiseActionPause(CCreatureActionBase* action);   // the row (linked)

static ActionRegistrar g_reg; static int g_calls = 0, g_lastId = 0; static void* g_lastDesc = 0;
extern "C" ActionRegistrar* __cdecl GetActionRegistrar() { return &g_reg; }
void ActionRegistrar::RegisterAction(int actionId, void* description) { ++g_calls; g_lastId = actionId; g_lastDesc = description; }
struct TestAction {                                     // the row reads GetActionId from vtable slot 7
    int Id;
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {}
    virtual void v4() {} virtual void v5() {} virtual void v6() {}
    virtual int GetActionId() { return Id; }
};

int main() {
    TestAction a; a.Id = 5;
    CCreatureActionBase_InitialiseActionPause((CCreatureActionBase*)&a);
    bool ok = g_calls == 1 && g_lastId == 5 && g_lastDesc == (void*)&ACTION_PAUSE_ON_THIS_FRAME;
    TestAction b; b.Id = 0;
    CCreatureActionBase_InitialiseActionPause((CCreatureActionBase*)&b);
    ok = ok && g_calls == 1;
    TestAction c; c.Id = -3;
    CCreatureActionBase_InitialiseActionPause((CCreatureActionBase*)&c);
    ok = ok && g_calls == 1;
    std::printf(ok ? "PARITY_OK\n" : "PARITY_FAIL\n");
    return ok ? 0 : 1;
}
