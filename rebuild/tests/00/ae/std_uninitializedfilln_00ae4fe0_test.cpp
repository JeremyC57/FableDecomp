// behaviour test for 0x00AE4FE0 std::uninitialized_fill_n over a 4-byte element (model)
#include <cstdio>
#include <new>
template <class T> static T* Fill(T* first, unsigned int n, const T& x)
{
    T* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) T(x);
    return cur;
}
int main()
{
    int raw[4] = { 0, 0, 0, 9 };
    int* end = Fill(raw, 3u, 7);
    int ok = end == raw + 3 && raw[0] == 7 && raw[2] == 7 && raw[3] == 9 && Fill(raw, 0u, 5) == raw;
    if (!ok) { std::printf("00ae4fe0_TEST FAIL\n"); return 1; }
    std::printf("00ae4fe0_TEST PASS\n");
    return 0;
}