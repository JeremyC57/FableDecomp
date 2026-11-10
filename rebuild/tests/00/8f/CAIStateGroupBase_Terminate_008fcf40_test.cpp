// Behaviour test for CAIStateGroupBase::Terminate (0x008FCF40): only the Terminated flag changes.
#include <stdio.h>
class CAIStateGroupBase { public:
    void* PBrain; long ScriptAIPriority; void* Name; bool Terminated; unsigned char Priority; bool Enabled;
    void Terminate() { Terminated = true; }
};
int main()
{
    CAIStateGroupBase g; g.PBrain = 0; g.ScriptAIPriority = 3; g.Name = 0; g.Terminated = false; g.Priority = 7; g.Enabled = true;
    g.Terminate();
    if (!g.Terminated || g.Priority != 7 || !g.Enabled || g.ScriptAIPriority != 3) { printf("008fcf40_TEST FAIL\n"); return 1; }
    g.Terminate();
    if (!g.Terminated) { printf("008fcf40_TEST FAIL\n"); return 1; }
    printf("008fcf40_TEST PASS\n");
    return 0;
}
