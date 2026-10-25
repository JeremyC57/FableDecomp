// Behaviour test for 0x00AD64C0 map<unsigned long, CContractionReference>::operator[] (insert an empty counted
// reference when absent; the pair temporary's counted pointer is released after the insert).
#include <cstdio>
#include <map>

static int g_deletes = 0;
struct CCPPointerInfo { unsigned int RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeleteData(void* p) { if (p) ++g_deletes; }

struct CCountedPointer {
    void* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& x) : Data(x.Data), Info(x.Info) { if (Info) ++Info->RefCount; }
    CCountedPointer& operator=(const CCountedPointer& x) { if (x.Info) ++x.Info->RefCount; Release(); Data = x.Data; Info = x.Info; return *this; }
    ~CCountedPointer() { Release(); }
    void Release() { CCPPointerInfo* info = Info; if (info && --info->RefCount == 0) { info->DeleteFunc(info->Data); delete info; } Info = 0; Data = 0; }
};
struct CContractionReference { CCountedPointer Contraction; unsigned int SubIndex; };
typedef std::map<unsigned long, CContractionReference> SRefMap;
static int g_inserts = 0;

static CContractionReference& Index(SRefMap& m, const unsigned long& k)
{
    SRefMap::iterator i = m.lower_bound(k);
    if (i == m.end() || m.key_comp()(k, (*i).first)) { ++g_inserts; i = m.insert(i, SRefMap::value_type(k, CContractionReference())); }
    return (*i).second;
}

int main()
{
    int ok = 1;
    {
        SRefMap m;
        CContractionReference& r = Index(m, 9);
        if (r.Contraction.Data != 0 || r.Contraction.Info != 0 || m.size() != 1 || g_inserts != 1) ok = 0;
        static int payload = 7;
        CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteData; info->Data = &payload;
        r.Contraction.Info = info; r.Contraction.Data = &payload; r.SubIndex = 3;
        Index(m, 2).SubIndex = 4; Index(m, 0xFFFFFFF0u).SubIndex = 5;
        if (Index(m, 9).Contraction.Info != info || Index(m, 9).SubIndex != 3 || info->RefCount != 1 || g_inserts != 3) ok = 0;
        if (m.begin()->first != 2 || m.rbegin()->first != 0xFFFFFFF0u || m.size() != 3 || g_deletes != 0) ok = 0;
    }
    if (g_deletes != 1) ok = 0;                 // the map's last reference released the contraction
    std::printf(ok ? "00ad64c0_TEST PASS\n" : "00ad64c0_TEST FAIL\n");
    return ok ? 0 : 1;
}