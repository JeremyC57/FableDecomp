// Behaviour test for CFrontendGameComponent::ClearString @ 0x004944C0 (private). Links against the row object. The private member
// is reached with the explicit-instantiation access idiom (C++03 14.7.2/8: access is not checked in an explicit instantiation), so
// the test calls the row's own ?ClearString@CFrontendGameComponent@@AAEXXZ (a `#define private public` would change the mangled
// access code to QAE and no longer link against the row).
#include <cstdio>
#include <cstring>

#define FABLE_RETAIL_CFRONTENDGAMECOMPONENT_CLEARSTRING
#include "engine/CFrontendGameComponent.h"

typedef void (CFrontendGameComponent::*ClearStringMember)();
struct ClearStringTag { friend ClearStringMember GetClearString(ClearStringTag); };
template <ClearStringMember M> struct ClearStringAccess {
    friend ClearStringMember GetClearString(ClearStringTag) { return M; }
};
template struct ClearStringAccess<&CFrontendGameComponent::ClearString>;

int main()
{
    static unsigned char storage[sizeof(CFrontendGameComponent)];
    CFrontendGameComponent& component = *reinterpret_cast<CFrontendGameComponent*>(storage);
    unsigned char* bytes = storage;
    std::memset(bytes, 0x5a, sizeof(storage));

    (component.*GetClearString(ClearStringTag()))();

    for (unsigned long index = 0; index < 0x168; ++index)
    {
        if (bytes[index] != 0x5a)
        {
            std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test FAIL\n");
            return 1;
        }
    }

    for (unsigned long index = 0x168; index < 0x1ec; ++index)
    {
        if (bytes[index] != 0)
        {
            std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test FAIL\n");
            return 1;
        }
    }

    for (unsigned long index = 0x1ec; index < sizeof(storage); ++index)
    {
        if (bytes[index] != 0x5a)
        {
            std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test FAIL\n");
            return 1;
        }
    }

    std::printf("CFrontendGameComponent_ClearTextInput_004944c0_test PASS\n");
    return 0;
}
