// Behaviour test for 0x00AB44D0 vector<_D3DVERTEXELEMENT9>::resize(n) (an 8-byte POD): shrinking erases the tail [begin + n, end); growing
// appends n - size() value-initialised (all-zero) records; the same size changes nothing.
#include <cstdio>

struct SElement8 { unsigned short first; unsigned short pad; long second; };   // Stream / Offset, Type..UsageIndex word

struct Vec {
    SElement8 items[16];
    unsigned int count;
    unsigned int size() const { return count; }
    void erase(unsigned int from, unsigned int to)
    {
        unsigned int out = from;
        for (unsigned int i = to; i < count; ++i) items[out++] = items[i];
        count = out;
    }
    void fill_insert(unsigned int pos, unsigned int n, const SElement8& x)
    {
        for (unsigned int i = count; i > pos; --i) items[i - 1 + n] = items[i - 1];
        for (unsigned int k = 0; k < n; ++k) items[pos + k] = x;
        count += n;
    }
    void resize(unsigned int n, const SElement8& x)
    {
        if (n < size())
            erase(n, count);
        else
            fill_insert(count, n - size(), x);
    }
    void resize(unsigned int n) { SElement8 zero; zero.first = 0; zero.pad = 0; zero.second = 0; resize(n, zero); }   // the value-initialised default
};

int main()
{
    static Vec v;
    v.count = 0;
    for (unsigned int i = 0; i < 5; ++i) { v.items[v.count].first = (unsigned short)(10 + i); v.items[v.count].pad = 0; v.items[v.count].second = 20 + i; ++v.count; }
    int ok = 1;
    v.resize(3);                                              // shrink
    if (v.size() != 3 || v.items[2].first != 12 || v.items[2].second != 22) ok = 0;
    for (unsigned int i = 3; i < 16; ++i) { v.items[i].first = (unsigned short)0xFFFF; v.items[i].pad = (unsigned short)0xFFFF; v.items[i].second = -1; }
    v.resize(6);                                              // grow: three zeroed records appended
    if (v.size() != 6 || v.items[0].first != 10) ok = 0;
    for (unsigned int i = 3; i < 6; ++i) if (v.items[i].first != 0 || v.items[i].pad != 0 || v.items[i].second != 0) ok = 0;
    v.resize(6);                                              // same size: nothing appended
    if (v.size() != 6) ok = 0;
    std::printf(ok ? "00ab44d0_TEST PASS\n" : "00ab44d0_TEST FAIL\n");
    return ok ? 0 : 1;
}