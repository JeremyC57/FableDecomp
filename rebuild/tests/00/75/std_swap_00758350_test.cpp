// behaviour test for 0x00758350 std::swap over a 44-byte entry (4-byte key + 10-dword value): both entries are exchanged
// word for word, including when both references name the same entry
#include <cstdio>
struct Key { unsigned int Value; };
struct Val { unsigned int Words[10]; };
struct Entry {
    Key first; Val second;
    Entry() { }
    Entry(const Entry& o) : first(o.first), second(o.second) { }
};
static void Swap(Entry& a, Entry& b) { Entry tmp = a; a = b; b = tmp; }
int main()
{
    int fails = 0;
    Entry a, b;
    a.first.Value = 1; b.first.Value = 2;
    for (int k = 0; k < 10; ++k) { a.second.Words[k] = 100 + k; b.second.Words[k] = 200 + k; }
    Swap(a, b);
    if (a.first.Value != 2 || b.first.Value != 1) fails |= 1;
    for (int k = 0; k < 10; ++k) if (a.second.Words[k] != (unsigned int)(200 + k) || b.second.Words[k] != (unsigned int)(100 + k)) fails |= 2;
    Swap(a, a);
    if (a.first.Value != 2 || a.second.Words[10 - 1] != (unsigned int)(200 + 10 - 1)) fails |= 4;
    if (sizeof(Entry) != (unsigned int)44) fails |= 8;
    if (fails) { std::printf("00758350_TEST FAIL %d\n", fails); return 1; }
    std::printf("00758350_TEST PASS\n");
    return 0;
}