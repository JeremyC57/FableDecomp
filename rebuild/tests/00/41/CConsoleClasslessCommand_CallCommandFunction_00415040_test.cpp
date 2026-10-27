// behaviour test for 0x00415040 CConsoleClasslessCommand::CallCommandFunction(params) const: calls the stored free
// command function (+0x14) with the parameter vector, through the virtual slot
#include <cstdio>
struct Params { int count; };
static const Params* gSeen = 0; static int gCalls = 0;
static void Command(const Params& p) { gSeen = &p; ++gCalls; }
struct Base { virtual ~Base() { } virtual void CallCommandFunction(const Params&) const { } };
struct Classless : Base {
    void (*CommandFunc)(const Params&);
    virtual void CallCommandFunction(const Params& p) const { CommandFunc(p); }
};
int main()
{
    int fails = 0;
    Classless c; c.CommandFunc = Command;
    Params p; p.count = 3;
    const Base& b = c;
    b.CallCommandFunction(p);
    if (gCalls != 1 || gSeen != &p) fails |= 1;
    b.CallCommandFunction(p);
    if (gCalls != 2) fails |= 2;
    if (fails) { std::printf("00415040_TEST FAIL %d\n", fails); return 1; }
    std::printf("00415040_TEST PASS\n");
    return 0;
}