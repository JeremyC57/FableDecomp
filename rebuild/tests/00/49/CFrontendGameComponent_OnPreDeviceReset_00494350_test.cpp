// Test for 0x00494350 CFrontendGameComponent::OnPreDeviceReset on the shared header (NF235): the row's include and body. Case: on an object
// filled with a pattern, the call changes no byte (retail is a bare `ret`).
#include <cstdio>
#include <cstring>
#include "engine/CFrontendGameComponent.h"
void CFrontendGameComponent::OnPreDeviceReset()
{
}
int main()
{
    static unsigned char raw[sizeof(CFrontendGameComponent)];
    static unsigned char want[sizeof(CFrontendGameComponent)];
    std::memset(raw, 0x5A, sizeof(raw)); std::memset(want, 0x5A, sizeof(want));
    CFrontendGameComponent& c = *(CFrontendGameComponent*)raw;
    c.CFrontendGameComponent::OnPreDeviceReset();
    bool ok = std::memcmp(raw, want, sizeof(raw)) == 0;
    std::printf(ok ? "AUTO_TINY_00494350_TEST PASS\n" : "AUTO_TINY_00494350_TEST FAIL\n");
    return ok ? 0 : 1;
}
