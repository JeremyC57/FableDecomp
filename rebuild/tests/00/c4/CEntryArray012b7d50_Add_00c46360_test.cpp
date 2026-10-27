// Test for 0x00C46360 CEntryArray_012b7d50::Add: the entry lands at the count and the count grows.
#include <cstdio>
struct SPair { int a, b; };
struct SEntry { int key; SPair value; };
struct CArr { SEntry* m_data; int m_count;
    void Add(const SEntry& e) { m_data[m_count].value = e.value; m_data[m_count].key = e.key; ++m_count; } };
int main()
{
    SEntry store[4]; CArr arr; arr.m_data = store; arr.m_count = 0;
    SEntry e1 = {7, {8, 9}}, e2 = {1, {2, 3}};
    arr.Add(e1); arr.Add(e2);
    if (arr.m_count != 2 || store[0].key != 7 || store[0].value.b != 9 || store[1].key != 1 || store[1].value.a != 2)
    { std::printf("00c46360_TEST FAIL\n"); return 1; }
    std::printf("00c46360_TEST PASS\n");
    return 0;
}