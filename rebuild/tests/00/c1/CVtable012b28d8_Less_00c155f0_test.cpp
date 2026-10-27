// behaviour test for 00c155f0 (standalone copy of the model)
#include <cstdio>
#include <cstring>

struct Item { unsigned long* m_pKey; };
struct Cmp {
    int Less(const Item* a, const Item* b) const { unsigned long kb = *b->m_pKey; unsigned long ka = *a->m_pKey; return ka == kb ? a < b : ka < kb; }
    int EqualToKey(const unsigned long& key, const Item* b) const { return key == *b->m_pKey; }
    int LessThanKey(const unsigned long& key, const Item* b) const { return key < *b->m_pKey; } };

int main()
{
    unsigned long k[3] = { 5, 5, 0xFFFFFFF0UL }; Item it[3]; for (int i = 0; i < 3; i++) it[i].m_pKey = &k[i]; Cmp c; unsigned long five = 5, big = 0xFFFFFFFFUL;
    if (!(c.Less(&it[0], &it[2]) == 1 && c.Less(&it[2], &it[0]) == 0 && c.Less(&it[0], &it[1]) == 1 && c.Less(&it[1], &it[0]) == 0 && c.Less(&it[0], &it[0]) == 0)) { std::printf("00c155f0_TEST FAIL\n"); return 1; }
    std::printf("00c155f0_TEST PASS\n");
    return 0;
}