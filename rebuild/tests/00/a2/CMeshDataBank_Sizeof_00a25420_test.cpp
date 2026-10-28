#include <cstdio>
// Behaviour test for CMeshDataBank::Sizeof (0x00A25420): 12 + 4 per entry pointer, plus 4 + Sizeof() per non-null entry and 4 per null one.
struct Entry { unsigned int Size; virtual ~Entry() {} virtual unsigned int Sizeof() const { return Size; } };
struct Bank {
    Entry** first; Entry** last;
    unsigned int Sizeof() const
    {
        unsigned int size = 12 + (unsigned int)(last - first) * 4;
        for (Entry** it = first; it != last; ++it)
            size += *it ? 4 + (*it)->Sizeof() : 4;
        return size;
    }
};
int main()
{
    Entry a, b; a.Size = 100; b.Size = 20;
    Entry* list[3] = { &a, 0, &b };
    Bank bank = { list, list + 3 };
    bool ok = bank.Sizeof() == 12 + 12 + 104 + 4 + 24;
    bank.last = list; ok = ok && bank.Sizeof() == 12;
    if (!ok) { std::printf("00a25420_TEST FAIL\n"); return 1; }
    std::printf("00a25420_TEST PASS\n");
    return 0;
}