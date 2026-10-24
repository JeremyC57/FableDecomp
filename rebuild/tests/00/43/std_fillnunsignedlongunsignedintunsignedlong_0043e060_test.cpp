// Behaviour test for std::fill_n<unsigned long*, unsigned int, unsigned long> (0x0043E060): n words get the value, the word after
// them is untouched, the return is first + n; n == 0 writes nothing and returns first.
#include <cstdio>
template <class OutputIter, class Size, class T>
OutputIter fill_n_model(OutputIter first, Size n, const T& val)
{
    for (; n > 0; --n, ++first)
        *first = val;
    return first;
}
int main()
{
    int bad = 0;
    unsigned long buf[6] = { 1, 2, 3, 4, 5, 6 };
    unsigned long v = 0xdeadbeef;
    unsigned long* end = fill_n_model(buf + 1, 3u, v);
    if (end != buf + 4 || buf[0] != 1 || buf[1] != v || buf[2] != v || buf[3] != v || buf[4] != 5) bad = 1;
    if (fill_n_model(buf, 0u, v) != buf || buf[0] != 1) bad = 1;
    if (bad) { std::printf("0043e060_TEST FAIL\n"); return 1; }
    std::printf("0043e060_TEST PASS\n");
    return 0;
}