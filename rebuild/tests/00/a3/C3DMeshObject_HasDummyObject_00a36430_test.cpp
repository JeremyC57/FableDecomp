// Behaviour test for 0x00A36430 C3DMeshObject::HasDummyObject: false without statistics, else the statistics' answer for the name's CRC.
#include <cstdio>
#include <cstring>

class CCharStringData { public: const char* m_pChars; unsigned long m_length; };
static const char g_empty[] = "";
class CCharString { public:
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; }
};
static unsigned long Calc(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i];
    return h + 1;
}
static unsigned long g_asked = 0;
class C3DMeshStats { public:
    unsigned long m_known;
    bool HasDummyObject(unsigned long nameHash) const { g_asked = nameHash; return nameHash == m_known; }
};
struct CCountedPointer { const C3DMeshStats* Data; void* Info; };
class C3DMeshObject { public:
    void* MeshInfo;
    CCountedPointer Mesh;
    bool HasDummyObject(const CCharString& name) const
    {
        const C3DMeshStats* stats = Mesh.Data;
        if (stats != 0)
            return stats->HasDummyObject(Calc(0, name.GetChars(), name.GetLength()));
        return false;
    }
};

int main()
{
    CCharStringData d = { "DUMMY_HAND", 10 };
    CCharString name = { &d };
    CCharString none = { 0 };
    C3DMeshStats stats;
    stats.m_known = Calc(0, "DUMMY_HAND", 10);
    C3DMeshObject obj;
    obj.MeshInfo = 0;
    obj.Mesh.Data = 0;
    obj.Mesh.Info = 0;
    if (obj.HasDummyObject(name) || g_asked != 0) { std::printf("00a36430_TEST FAIL\n"); return 1; }
    obj.Mesh.Data = &stats;
    if (!obj.HasDummyObject(name) || g_asked != stats.m_known) { std::printf("00a36430_TEST FAIL\n"); return 1; }
    if (obj.HasDummyObject(none) || g_asked != Calc(0, "", 0)) { std::printf("00a36430_TEST FAIL\n"); return 1; }
    std::printf("00a36430_TEST PASS\n");
    return 0;
}