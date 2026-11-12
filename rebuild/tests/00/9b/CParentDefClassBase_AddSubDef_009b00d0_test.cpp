// Behaviour test for CParentDefClassBase::AddSubDef (0x009B00D0): the sub-def is filed under the CRC-32 of its name with
// {DefIndex, OriginalParentDefIndex} and the map is kept sorted by key (Insert sets Dirty, Update sorts and clears it).
#include <cstdio>
#include <cstring>
#include <vector>
#include <algorithm>

static unsigned long CRCTable[256];
static void InitCRC()
{
    for (unsigned long i = 0; i < 256; ++i) {
        unsigned long c = i;
        for (int k = 0; k < 8; ++k)
            c = (c & 1) ? (0xEDB88320UL ^ (c >> 1)) : (c >> 1);
        CRCTable[i] = c;
    }
}
static unsigned long Calc(unsigned long seed, const void* data, unsigned long byteCount)   // CCRC::Calc model
{
    const unsigned char* bytes = static_cast<const unsigned char*>(data);
    for (unsigned long i = 0; i < byteCount; ++i)
        seed = CRCTable[bytes[i] ^ (seed & 0xFF)] ^ (seed >> 8);
    return seed;
}
struct CSubDefInfo { long DefIndex; long OriginalParentDefIndex; CSubDefInfo(long d, long p) : DefIndex(d), OriginalParentDefIndex(p) {} };
typedef std::pair<unsigned long, CSubDefInfo> Entry;
static bool Less(const Entry& a, const Entry& b) { return a.first < b.first; }
struct Map {
    std::vector<Entry> Data; bool Dirty;
    Map() : Dirty(false) {}
    void Insert(unsigned long k, const CSubDefInfo& v) { Data.push_back(Entry(k, v)); Dirty = true; }
    void Update() { if (Dirty) { std::sort(Data.begin(), Data.end(), Less); Dirty = false; } }
};
class CParentDefClassBase { public:
    Map SubDefInfoMap;
    void AddSubDef(const char* name, long def_index, long original_parent_def_index)
    {
        SubDefInfoMap.Insert(Calc(0, name, (unsigned long)std::strlen(name)), CSubDefInfo(def_index, original_parent_def_index));
        SubDefInfoMap.Update();
    }
};
int main()
{
    InitCRC();
    CParentDefClassBase def;
    def.AddSubDef("Sword", 10, 1);
    def.AddSubDef("Axe", 20, 2);
    def.AddSubDef("Bow", 30, 3);
    if (def.SubDefInfoMap.Data.size() != 3 || def.SubDefInfoMap.Dirty) { std::printf("009b00d0_TEST FAIL\n"); return 1; }
    for (size_t i = 1; i < def.SubDefInfoMap.Data.size(); ++i)
        if (def.SubDefInfoMap.Data[i - 1].first > def.SubDefInfoMap.Data[i].first) { std::printf("009b00d0_TEST FAIL\n"); return 1; }
    bool found = false;
    for (size_t i = 0; i < def.SubDefInfoMap.Data.size(); ++i)
        if (def.SubDefInfoMap.Data[i].first == Calc(0, "Axe", 3) && def.SubDefInfoMap.Data[i].second.DefIndex == 20
            && def.SubDefInfoMap.Data[i].second.OriginalParentDefIndex == 2)
            found = true;
    if (!found) { std::printf("009b00d0_TEST FAIL\n"); return 1; }
    std::printf("009b00d0_TEST PASS\n");
    return 0;
}