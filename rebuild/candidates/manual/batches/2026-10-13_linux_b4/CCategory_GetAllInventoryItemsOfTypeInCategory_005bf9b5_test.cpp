#include <cstdio>

struct CItem { long ItemType; long NumberOfItems; unsigned char rest[12]; };
static int g_pushed = 0;
struct Out { void PushBack(const CItem&) { ++g_pushed; } };
struct Cat { unsigned char pad[8]; CItem* First; CItem* Last;
    void Get(long ty, Out* o) { for (CItem* i = First; i < Last; ++i) if (i->NumberOfItems != 0 && i->ItemType == ty) o->PushBack(*i); } };
int main()
{
    CItem it[3]; it[0].ItemType = 5; it[0].NumberOfItems = 1; it[1].ItemType = 5; it[1].NumberOfItems = 0; it[2].ItemType = 6; it[2].NumberOfItems = 3;
    Cat c; c.First = it; c.Last = it + 3; Out o; c.Get(5, &o); if (g_pushed != 1) { std::printf("005bf9b5_TEST FAIL\n"); return 1; }
    std::printf("005bf9b5_TEST PASS\n");
    return 0;
}
