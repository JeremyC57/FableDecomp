// Behaviour test for 0x00E0E970 CSpawnedFunc<NScript::CQ_WaspBossScript>::SuspendableProcess (agentFAM08 portable twin; the catalog
// test was a __fastcall shim). Model: the thread body calls the bound member function on its script object, (PClass->*Func)(),
// and then sets StopRunning (+0x05) so the microthread ends. Test-only model types.
#include <cstdio>
struct ModelScript {
    int mainCalls;
    void Main() { ++mainCalls; }
};
struct ModelSpawnedFunc {
    bool running;
    bool stopRunning;
    void (ModelScript::*func)();
    ModelScript* pclass;
    void SuspendableProcess() { (pclass->*func)(); stopRunning = true; }
};
int main()
{
    ModelScript script = { 0 };
    ModelSpawnedFunc f;
    f.running = true;
    f.stopRunning = false;
    f.func = &ModelScript::Main;
    f.pclass = &script;
    f.SuspendableProcess();
    if (script.mainCalls != 1 || !f.stopRunning || !f.running) { std::printf("00e0e970_TEST FAIL\n"); return 1; }
    std::printf("00e0e970_TEST PASS\n");
    return 0;
}
