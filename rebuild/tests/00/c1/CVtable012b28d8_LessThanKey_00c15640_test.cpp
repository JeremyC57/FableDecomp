// behaviour test for 00c15640 (standalone copy of the model)
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
    if (!(c.LessThanKey(five, &it[2]) == 1 && c.LessThanKey(big, &it[2]) == 0 && c.LessThanKey(five, &it[1]) == 0)) { std::printf("00c15640_TEST FAIL\n"); return 1; }
    std::printf("00c15640_TEST PASS\n");
    return 0;
}