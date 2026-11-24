// Test for 0x00494370 CFrontendGameComponent::SetQuit on the shared header (NF235): the row's define, include and body. Case 1: on a
// zeroed object, SetQuit sets the base's Quit byte (+8) to 1 and leaves Running (+9) and the rest untouched. Case 2: a second call keeps it 1.
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_SETQUIT
#include "engine/CFrontendGameComponent.h"
void CFrontendGameComponent::SetQuit()
{
    Quit = true;
}
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)];
    static unsigned char want[sizeof(CFrontendGameComponent)];
    CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    c.CFrontendGameComponent::SetQuit();
    want[8] = 1;
    bool ok = std::memcmp(raw, want, sizeof(raw)) == 0;
    c.CFrontendGameComponent::SetQuit();
    ok = ok && std::memcmp(raw, want, sizeof(raw)) == 0;
    std::printf(ok ? "PASS_00494370\n" : "FAIL_00494370\n");
    return ok ? 0 : 1;
}
