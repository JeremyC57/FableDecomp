#include <cstdio>
#include <cstring>
#include <cstdlib>
// behaviour test: HasHelperPoint CRCs the name, walks the gathered meshes, skips meshes without bank info and returns true on the
// first mesh whose stats carry the helper point, releasing every counted stats temporary it created
struct CCharStringData { char* m_pData; unsigned long m_length; };
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
static unsigned long Calc(unsigned long seed, const char* data, unsigned long length)
{ unsigned long c = seed; for (unsigned long i = 0; i < length; ++i) c = c * 31 + (unsigned char)data[i]; return c; }
static int g_liveStats = 0;
struct C3DMeshStats { unsigned long helperKey; bool HasHelperPoint(unsigned long key) const { return key == helperKey; } };
struct CStatsPtr {
    const C3DMeshStats* p;
    CStatsPtr(const C3DMeshStats* s) : p(s) { ++g_liveStats; }
    CStatsPtr(const CStatsPtr& o) : p(o.p) { ++g_liveStats; }
    ~CStatsPtr() { --g_liveStats; }
    const C3DMeshStats* operator->() const { return p; }
};
struct C3DMeshInfo { C3DMeshStats stats; CStatsPtr GetMeshStats() const { return CStatsPtr(&stats); } };
static C3DMeshInfo g_infos[8]; static bool g_present[8];
static int g_calls = 0;
static const C3DMeshInfo* GetMeshInfo(long index) { ++g_calls; return g_present[index] ? &g_infos[index] : 0; }
struct CMeshInfo { long Index; float SizeX; unsigned char PrimaryMesh; };
struct CMeshVector {
    CMeshInfo* _M_start; CMeshInfo* _M_finish;
    CMeshVector() : _M_start(0), _M_finish(0) {}
    ~CMeshVector() { if (_M_start) std::free(_M_start); }
    void reserve(unsigned long n) { _M_start = _M_finish = (CMeshInfo*)std::malloc(n * 4 * sizeof(CMeshInfo)); }
    CMeshInfo* begin() const { return _M_start; }
    CMeshInfo* end() const { return _M_finish; }
};
struct CTCGraphicAppearance {
    long idx[4]; int n;
    void GetAllMeshes(CMeshVector& meshes) const { meshes.reserve(4); for (int i = 0; i < n; ++i) { CMeshInfo m = { idx[i], 0, 0 }; *meshes._M_finish++ = m; } }
    bool HasHelperPoint(const CCharString& name) const
    {
        CMeshVector meshes;
        GetAllMeshes(meshes);
        unsigned long key = Calc(0, name.GetPString(), name.GetLength());
        for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
            const C3DMeshInfo* meshInfo = GetMeshInfo(mesh->Index);
            if (meshInfo && meshInfo->GetMeshStats()->HasHelperPoint(key))
                return true;
        }
        return false;
    }
};
int main()
{
    char text[] = "HDL_HAND";
    CCharStringData d = { text, 8 };
    CCharString name = { &d }, empty = { 0 };
    unsigned long key = Calc(0, text, 8);
    g_present[1] = false; g_present[2] = true; g_present[3] = true;
    g_infos[2].stats.helperKey = 1; g_infos[3].stats.helperKey = key;
    CTCGraphicAppearance a = { { 1, 2, 3, 0 }, 3 };
    bool ok = a.HasHelperPoint(name) && g_calls == 3 && g_liveStats == 0;
    g_calls = 0;
    ok = ok && !a.HasHelperPoint(empty) && g_calls == 3 && g_liveStats == 0;     // empty name: CRC 0 matches nothing
    if (!ok) { std::printf("004be170_TEST FAIL\n"); return 1; }
    std::printf("004be170_TEST PASS\n");
    return 0;
}