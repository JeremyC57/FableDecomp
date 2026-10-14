#include <cstdio>
static int g_processed = 0; static bool g_flagDuringProcess = false;
struct CAIBrain;
static CAIBrain* g_brain;
struct CAIStateGroupBase { void* __vftable; void* PBrain; unsigned char ScriptAIPriority[4]; void* Name_PStringData; bool Terminated; unsigned char Priority; bool Enabled; void Process(); };
struct CAIBrain { void* __vftable; unsigned char _pad0[0x34]; CAIStateGroupBase* PActiveStateGroup; unsigned char _pad1[0x51]; bool UpdatingStateGroup; void SuspendableProcess(); };
void CAIStateGroupBase::Process() { ++g_processed; g_flagDuringProcess = g_brain->UpdatingStateGroup; }
void CAIBrain::SuspendableProcess()
{
    CAIStateGroupBase* group = PActiveStateGroup;
    if (!group->Terminated) { UpdatingStateGroup = true; group->Process(); UpdatingStateGroup = false; }
}
int main()
{
    CAIStateGroupBase g = { 0, 0, { 0 }, 0, true, 0, true }; CAIBrain b; b.PActiveStateGroup = &g; b.UpdatingStateGroup = false; g_brain = &b;
    b.SuspendableProcess();
    if (g_processed != 0) { std::printf("0088b7a0_TEST FAIL\n"); return 1; }
    g.Terminated = false; b.SuspendableProcess();
    if (g_processed != 1 || !g_flagDuringProcess || b.UpdatingStateGroup) { std::printf("0088b7a0_TEST FAIL\n"); return 1; }
    std::printf("0088b7a0_TEST PASS\n");
    return 0;
}
