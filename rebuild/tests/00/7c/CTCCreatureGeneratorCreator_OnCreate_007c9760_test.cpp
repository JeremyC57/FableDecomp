// Behaviour test for 0x007C9760: OnCreate sets the parent's LevelPersistent bit without touching the others
#include <stdio.h>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d\n", __LINE__); g_fail = 1; } } while (0)

struct SThing { unsigned char Flags92; };
struct SCreator { SThing* ParentThing; void OnCreate() { ParentThing->Flags92 |= 0x10; } };
int main()
{
    SThing t; t.Flags92 = 0x05; SCreator c; c.ParentThing = &t;
    c.OnCreate(); CHECK(t.Flags92 == 0x15);
    c.OnCreate(); CHECK(t.Flags92 == 0x15);
    if (!g_fail) printf("007c9760_TEST PASS\n");
    return g_fail;
}