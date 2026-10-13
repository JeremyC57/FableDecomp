#include <cstdio>

static int g_calls = 0;
struct CItem { long ItemType; long NumberOfItems; unsigned char rest[0xc]; void GetItemThings(void*) { ++g_calls; } };
struct Cat { unsigned char pad[8]; CItem* First; CItem* Last;
    void Get(void* o) { for (CItem* i = First; i < Last; ++i) if (i->NumberOfItems != 0) i->GetItemThings(o); } };
int main()
{
    CItem it[3]; it[0].NumberOfItems = 1; it[1].NumberOfItems = 0; it[2].NumberOfItems = 4; Cat c; c.First = it; c.Last = it + 3; c.Get(0);
    if (g_calls != 2) { std::printf("005bf2c6_TEST FAIL\n"); return 1; }
    std::printf("005bf2c6_TEST PASS\n");
    return 0;
}
