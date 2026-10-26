// Behaviour test for 0x00870930: CThingComponentSet::Remove erases every entry with the given name (adjacent duplicates
// included) and keeps the others in order.
#include <cstdio>
#include <cstring>

struct CEntry { int NameTablePos; unsigned int DriverType; bool Override; };
static const char* g_Names[4] = { "Physics", "Driver", "Health", "Other" };

class CThingComponentSet
{ public:
    CEntry Storage[8];
    CEntry* Start;
    signed char Size;
    CEntry* erase(CEntry* position)
    {
        CEntry* end = Start + Size;
        for (CEntry* p = position; p + 1 != end; ++p) *p = *(p + 1);
        --Size;
        return position;
    }
    void Remove(const char* name)
    {
        for (CEntry* it = Start; it != Start + Size; )
        {
            if (std::strcmp(g_Names[it->NameTablePos], name) == 0) it = erase(it);
            else ++it;
        }
    }
};

int main()
{
    CThingComponentSet set; set.Start = set.Storage; set.Size = 6;
    int names[6] = { 0, 2, 2, 1, 2, 3 };
    for (int i = 0; i < 6; ++i) { set.Storage[i].NameTablePos = names[i]; set.Storage[i].DriverType = (unsigned int)i; set.Storage[i].Override = false; }
    int fails = 0;
    set.Remove("Health");
    if (set.Size != 3) ++fails;
    if (set.Storage[0].DriverType != 0u || set.Storage[1].DriverType != 3u || set.Storage[2].DriverType != 5u) ++fails;
    set.Remove("Missing");
    if (set.Size != 3) ++fails;
    set.Remove("Physics"); set.Remove("Driver"); set.Remove("Other");
    if (set.Size != 0) ++fails;
    if (fails) { std::printf("00870930_TEST FAIL\n"); return 1; }
    std::printf("00870930_TEST PASS\n");
    return 0;
}