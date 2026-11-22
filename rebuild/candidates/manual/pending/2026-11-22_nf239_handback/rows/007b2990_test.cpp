// Behaviour test for CTCGuard::GetWaitForRebribeInSeconds() const (0x007B2990): links the row object and checks the constant 15
//   (NF239: on the shared engine/CTCGuard.h declaration; the HEAD test modelled the stand-in C::AddChildPrimitive).
#include <cstdio>
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_GUARD_REACT_TO_BRIBE_PASS27
#include "fable_string.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCGuard.h"
int main()
{
    static void* buf[64] = { 0 };
    const CTCGuard* guard = (const CTCGuard*)buf;
    if (guard->GetWaitForRebribeInSeconds() != 15) { std::printf("007b2990_TEST FAIL\n"); return 1; }
    std::printf("007b2990_TEST PASS\n");
    return 0;
}
