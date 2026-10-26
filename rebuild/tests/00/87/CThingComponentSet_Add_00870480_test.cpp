#include <cstdio>
#include <cstdlib>
// behaviour test for CThingComponentSet::Add (0x00870480)
#pragma pack(push, 1)
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
struct CEntry {
    CDefString Name;
    unsigned int DriverType;
    bool Override;
    CEntry() : DriverType(0), Override(false) { }
};
#pragma pack(pop)
static int g_resizes = 0;
template<class T, int N> class CSmallVector { public:
    T* Start;
    signed char Size;
    void resize(long n, const T& fill)
    {
        ++g_resizes;
        T* p = (T*)std::malloc(sizeof(T) * n);
        for (long i = 0; i < n; ++i) p[i] = i < Size ? Start[i] : fill;
        std::free(Start);
        Start = p; Size = (signed char)n;
    }
    T& back() { return Start[Size - 1]; }
};
class CThingComponentSet { public:
    CSmallVector<CEntry, 8> Entries;
    void Add(const CEntry& entry);
};
void CThingComponentSet::Add(const CEntry& entry)
{
    Entries.resize(Entries.Size + 1, CEntry());
    Entries.back() = entry;
}
int main()
{
    if (sizeof(CEntry) != 9) { std::printf("00870480_TEST FAIL\n"); return 1; }
    CThingComponentSet set; set.Entries.Start = 0; set.Entries.Size = 0;
    CEntry a; a.Name.TablePos = 11; a.DriverType = 2; a.Override = true;
    CEntry b; b.Name.TablePos = 22; b.DriverType = 5; b.Override = false;
    set.Add(a); set.Add(b);
    if (g_resizes != 2 || set.Entries.Size != 2) { std::printf("00870480_TEST FAIL\n"); return 1; }
    if (set.Entries.Start[0].Name.TablePos != 11 || set.Entries.Start[0].DriverType != 2 || !set.Entries.Start[0].Override) { std::printf("00870480_TEST FAIL\n"); return 1; }
    if (set.Entries.Start[1].Name.TablePos != 22 || set.Entries.Start[1].DriverType != 5 || set.Entries.Start[1].Override) { std::printf("00870480_TEST FAIL\n"); return 1; }
    std::free(set.Entries.Start);
    std::printf("00870480_TEST PASS\n");
    return 0;
}