// Behaviour test for 0x009AD580 CDefinitionManager::GetPDefClassInfo(const CCharString&): the class name's crc0 is
// looked up in the sorted DefClassInfo map; a hit returns the entry's record, a miss (or an empty name not present) null.
#include <cstdio>
#include <cstring>

static unsigned int g_table[256];
static void InitTable()
{
    for (unsigned int i = 0; i < 256; ++i) {
        unsigned int c = i;
        for (int k = 0; k < 8; ++k) c = (c & 1) ? (c >> 1) ^ 0xEDB88320u : (c >> 1);
        g_table[i] = c;
    }
}
static unsigned int Calc(unsigned int seed, const void* data, unsigned int n)
{
    const unsigned char* p = (const unsigned char*)data;
    unsigned int crc = seed;
    for (unsigned int i = 0; i < n; ++i) crc = g_table[(crc ^ p[i]) & 0xFF] ^ (crc >> 8);
    return crc;
}

struct CCharStringData { const char* PData; unsigned int StringLength; };
struct CCharString {
    CCharStringData* PStringData;
    unsigned int GetLength() const { return PStringData == 0 ? 0 : PStringData->StringLength; }
    const char* GetPString() const { return PStringData == 0 ? "" : PStringData->PData; }
    unsigned int GetCRC() const { return Calc(0, GetPString(), GetLength()); }
};

struct CDefClassInfo { int Data[5]; };
struct SEntry { unsigned int first; CDefClassInfo second; };
struct CDefClassInfoMap {
    SEntry* First; SEntry* Last;
    SEntry* LowerBound(const unsigned int& key)
    {
        SEntry* first = First;
        int count = (int)(Last - First);
        while (count > 0) {
            int half = count / 2;
            SEntry* middle = first + half;
            if (middle->first < key) { first = middle + 1; count -= half + 1; }
            else count = half;
        }
        return first;
    }
    SEntry* end() { return Last; }
    SEntry* Find(const unsigned int& key)
    {
        SEntry* it = LowerBound(key);
        if (it == end() || key < it->first) it = end();
        return it;
    }
};
struct CDefinitionManager {
    CDefClassInfoMap DefClassInfo;
    CDefClassInfo* GetPDefClassInfo(unsigned int key)
    {
        SEntry* it = DefClassInfo.Find(key);
        if (it == DefClassInfo.end()) return 0;
        return &it->second;
    }
    CDefClassInfo* GetPDefClassInfo(const CCharString& name)
    {
        unsigned int crc = name.GetCRC();
        return GetPDefClassInfo(crc);
    }
};

int main()
{
    InitTable();
    const char* names[3] = { "CThingDef", "CCreatureDef", "CObjectDef" };
    SEntry e[3];
    for (int i = 0; i < 3; ++i) { e[i].first = Calc(0, names[i], (unsigned int)std::strlen(names[i])); e[i].second.Data[0] = i; }
    // sort the three entries by key
    for (int i = 0; i < 3; ++i) for (int j = i + 1; j < 3; ++j) if (e[j].first < e[i].first) { SEntry t = e[i]; e[i] = e[j]; e[j] = t; }
    CDefinitionManager m; m.DefClassInfo.First = e; m.DefClassInfo.Last = e + 3;
    int bad = 0;
    for (int i = 0; i < 3; ++i) {
        CCharStringData d = { names[i], (unsigned int)std::strlen(names[i]) };
        CCharString s; s.PStringData = &d;
        CDefClassInfo* info = m.GetPDefClassInfo(s);
        if (info == 0 || std::strcmp(names[info->Data[0]], names[i]) != 0) bad = 1;
    }
    CCharStringData miss = { "CMissingDef", 11 };
    CCharString sm; sm.PStringData = &miss;
    if (m.GetPDefClassInfo(sm) != 0) bad = 1;
    CCharString empty; empty.PStringData = 0;         // crc0 of "" is 0: absent
    if (m.GetPDefClassInfo(empty) != 0) bad = 1;
    if (bad) { std::printf("009ad580_TEST FAIL\n"); return 1; }
    std::printf("009ad580_TEST PASS\n");
    return 0;
}