// Test for 0x00494360 CFrontendGameComponent::OnPostDeviceReset on the shared header (NF235): the row's include and body. Case: on an
// object filled with a pattern, the call returns true and changes no byte (retail `mov al,1 ; ret`).
#include <cstdio>
#include <cstring>
#include "engine/CFrontendGameComponent.h"
bool CFrontendGameComponent::OnPostDeviceReset()
{
    return true;
}
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)];
    static unsigned char want[sizeof(CFrontendGameComponent)];
    std::memset(raw, 0xA5, sizeof(raw)); std::memset(want, 0xA5, sizeof(want));
    CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    bool r = c.CFrontendGameComponent::OnPostDeviceReset();
    bool ok = r == true && std::memcmp(raw, want, sizeof(raw)) == 0;
    std::printf(ok ? "AUTO_TINY_00494360_TEST PASS\n" : "AUTO_TINY_00494360_TEST FAIL\n");
    return ok ? 0 : 1;
}
