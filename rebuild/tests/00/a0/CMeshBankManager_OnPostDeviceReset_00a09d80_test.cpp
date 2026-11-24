// Test for 0x00A09D80 CMeshBankManager::OnPostDeviceReset on the shared header (NF235): the row's define, include and body. Case: on an
// object filled with a pattern, the call returns true and changes no byte (retail `mov al,1 ; ret`).
#include <cstdio>
#include <cstring>
#define FABLE_RETAIL_CMESHBANKMANAGER_CTOR_DTOR
#include "engine/CMeshBankManager.h"
bool CMeshBankManager::OnPostDeviceReset()
{
    return true;
}
int main()
{
    static unsigned char raw[sizeof(CMeshBankManager)];
    static unsigned char want[sizeof(CMeshBankManager)];
    std::memset(raw, 0xA5, sizeof(raw)); std::memset(want, 0xA5, sizeof(want));
    CMeshBankManager& m = *(CMeshBankManager*)raw;
    bool r = m.CMeshBankManager::OnPostDeviceReset();
    bool ok = r == true && std::memcmp(raw, want, sizeof(raw)) == 0;
    std::printf(ok ? "AUTO_TINY_00a09d80_TEST PASS\n" : "AUTO_TINY_00a09d80_TEST FAIL\n");
    return ok ? 0 : 1;
}
