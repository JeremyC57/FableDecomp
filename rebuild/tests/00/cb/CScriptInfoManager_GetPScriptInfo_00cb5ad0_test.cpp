// Behaviour test for 0x00CB5AD0 CScriptInfoManager::GetPScriptInfo: lower-bound the sorted Scripts map, the entry's
// CScriptInfo when its key equals the name, else NULL (also for a name past the end).
#include <cstdio>
#include <cstring>

class CCharString {
public:
    const char* PData;
    CCharString(const char* s = 0) : PData(s) {}
    bool operator<(const CCharString& other) const
    {
        if (other.PData == PData) return false;
        if (PData == 0) return true;
        if (other.PData == 0) return false;
        return std::strcmp(PData, other.PData) < 0;
    }
};
class CScriptInfo { public: void* AllocFunc; void* MasterScriptDataAlloc; CCharString Name; int ID; bool MasterScript; };
struct SPair { CCharString first; CScriptInfo second; };

class CVectorMap {
public:
    SPair* _M_start;
    SPair* _M_finish;
    SPair* _M_end_of_storage;
    bool Dirty;
    SPair* end() const { return _M_finish; }
    SPair* LowerBound(const CCharString& key)
    {
        SPair* first = _M_start;
        int len = (int)(_M_finish - _M_start);
        while (len > 0) {
            int half = len >> 1;
            SPair* mid = first + half;
            if (mid->first < key) { first = mid + 1; len = len - half - 1; }
            else len = half;
        }
        return first;
    }
    SPair* Find(const CCharString& key)
    {
        SPair* it = LowerBound(key);
        SPair* last = end();
        return (it == last || key < it->first) ? last : it;
    }
};

class CScriptInfoManager {
public:
    int CurrentScriptID;
    CVectorMap Scripts;
    CScriptInfo* GetPScriptInfo(const CCharString& name)
    {
        SPair* it = Scripts.Find(name);
        if (it != Scripts.end())
            return &it->second;
        return 0;
    }
};

int main()
{
    SPair data[3];
    const char* names[3] = { "Q_Arena", "Q_Bandits", "Q_Guild" };
    for (int i = 0; i < 3; ++i) { data[i].first = CCharString(names[i]); data[i].second.Name = data[i].first; data[i].second.ID = 100 + i; }
    CScriptInfoManager m;
    m.Scripts._M_start = data; m.Scripts._M_finish = data + 3; m.Scripts._M_end_of_storage = data + 3;
    char key[16];
    std::strcpy(key, "Q_Bandits");                         // equal text, different buffer
    CScriptInfo* p = m.GetPScriptInfo(CCharString(key));
    if (p != &data[1].second || p->ID != 101) { std::printf("00cb5ad0_TEST FAIL\n"); return 1; }
    if (m.GetPScriptInfo(CCharString("Q_Arena"))->ID != 100) { std::printf("00cb5ad0_TEST FAIL\n"); return 1; }
    if (m.GetPScriptInfo(CCharString("Q_Bard")) != 0 || m.GetPScriptInfo(CCharString("Q_Zzz")) != 0 ||
        m.GetPScriptInfo(CCharString("A")) != 0) { std::printf("00cb5ad0_TEST FAIL\n"); return 1; }
    m.Scripts._M_finish = data;                            // empty map
    if (m.GetPScriptInfo(CCharString("Q_Arena")) != 0) { std::printf("00cb5ad0_TEST FAIL\n"); return 1; }
    std::printf("00cb5ad0_TEST PASS\n");
    return 0;
}