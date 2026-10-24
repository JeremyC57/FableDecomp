// Behaviour test for 0x008F78E0 NCombatActionHelpers::GetDustCreationHelperNames: appends left then right foot helper.
#include <cstdio>
#include <cstring>

static const char* g_left = "weapon_pos_left_foot";
static const char* g_right = "weapon_pos_right_foot";

struct NameVector {
    const char* items[4];
    int count, capacity, overflows;
    void push_back(const char* s)
    {
        if (count != capacity) items[count++] = s;
        else { ++overflows; ++capacity; items[count++] = s; }        // the _M_insert_overflow path
    }
};

static bool GetDustCreationHelperNames(const void*, NameVector& helperNames)
{
    helperNames.push_back(g_left);
    helperNames.push_back(g_right);
    return true;
}

int main()
{
    NameVector v;
    v.count = 1; v.capacity = 2; v.overflows = 0; v.items[0] = "existing";
    int fail = 0;
    if (!GetDustCreationHelperNames(0, v)) fail = 1;
    if (v.count != 3 || v.overflows != 1) fail = 1;
    if (std::strcmp(v.items[1], "weapon_pos_left_foot") || std::strcmp(v.items[2], "weapon_pos_right_foot")) fail = 1;
    if (fail) { std::printf("008f78e0_TEST FAIL\n"); return 1; }
    std::printf("008f78e0_TEST PASS\n");
    return 0;
}