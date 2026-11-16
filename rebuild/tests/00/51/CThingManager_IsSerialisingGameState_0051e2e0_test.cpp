#include <cstdio>

struct CTCInventoryBase { char pad[0x9a]; unsigned char canQuickAssign; };

unsigned char __fastcall IsSerialisingGameState(const CTCInventoryBase* self)
{
    return self->canQuickAssign;
}

int main()
{
    CTCInventoryBase o;
    o.canQuickAssign = 1;
    if (IsSerialisingGameState(&o) != 1) { std::printf("FAIL true\n"); return 1; }
    o.canQuickAssign = 0;
    if (IsSerialisingGameState(&o) != 0) { std::printf("FAIL false\n"); return 1; }
    std::printf("CTCInventoryBase_0051e2e0_TEST PASS\n");
    return 0;
}