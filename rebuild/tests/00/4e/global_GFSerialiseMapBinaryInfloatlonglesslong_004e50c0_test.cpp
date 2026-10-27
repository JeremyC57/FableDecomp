// behaviour test for 0x004E50C0 GFSerialiseMapBinaryIn (model: count, then key/value pairs inserted)
#include <cstdio>
#include <map>
static const int g_data[] = { 2, 5, 0, 7, 0 };
int main()
{
    const float vals[2] = { 1.5f, 2.5f };
    std::map<int, float> m; m[99] = 9.0f;
    m.clear();
    int count = g_data[0];
    for (int i = 0; count > 0; --count, ++i) m.insert(std::make_pair(g_data[1 + 2 * i], vals[i]));
    int ok = m.size() == 2 && m[5] == 1.5f && m[7] == 2.5f;
    if (!ok) { std::printf("004e50c0_TEST FAIL\n"); return 1; }
    std::printf("004e50c0_TEST PASS\n");
    return 0;
}