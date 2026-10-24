// Behaviour test for 0x00A36140 C3DMeshObject::GetNoHelperPoints: End - Begin of the 20-byte entries under the name's CRC.
#include <cstdio>

struct CCharStringData { const char* m_pChars; unsigned long m_length; };
static const char g_empty[] = "";
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; }
};
static unsigned long Calc(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i];
    return h;
}
struct C3DVector { float X, Y, Z; };
struct C3DHelperPoint { C3DVector Position; int BoneIndex; };
struct TEntry { unsigned int first; C3DHelperPoint second; };
struct C3DMeshStats {
    TEntry m_entries[6];
    int m_size;
    const TEntry* FindHelperPointsBegin(unsigned long key) const
    {
        int i = 0;
        while (i < m_size && m_entries[i].first < key) ++i;
        return m_entries + i;
    }
    const TEntry* FindHelperPointsEnd(unsigned long key) const
    {
        int i = 0;
        while (i < m_size && !(key < m_entries[i].first)) ++i;
        return m_entries + i;
    }
};
struct CCountedPointer {
    const C3DMeshStats* Data; void* Info;
    const C3DMeshStats* operator->() const { return Data; }
};
struct C3DMeshObject {
    void* MeshInfo;
    CCountedPointer Mesh;
    long GetNoHelperPoints(const CCharString& name) const
    {
        unsigned long nameHash = Calc(0, name.GetChars(), name.GetLength());
        const TEntry* last = Mesh->FindHelperPointsEnd(nameHash);
        return last - Mesh->FindHelperPointsBegin(nameHash);
    }
};

int main()
{
    CCharStringData d = { "HP", 2 };
    CCharString name = { &d };
    unsigned long h = Calc(0, "HP", 2);
    C3DMeshStats stats;
    unsigned long keys[6] = { 1, h, h, h, h + 5, h + 9 };
    for (int i = 0; i < 6; ++i) { stats.m_entries[i].first = keys[i]; stats.m_entries[i].second.BoneIndex = i; }
    stats.m_size = 6;
    C3DMeshObject obj;
    obj.MeshInfo = 0;
    obj.Mesh.Data = &stats;
    obj.Mesh.Info = 0;
    if (sizeof(TEntry) != 20 || obj.GetNoHelperPoints(name) != 3) { std::printf("00a36140_TEST FAIL\n"); return 1; }
    stats.m_entries[2].first = h + 1; stats.m_entries[3].first = h + 2;
    if (obj.GetNoHelperPoints(name) != 1) { std::printf("00a36140_TEST FAIL\n"); return 1; }
    CCharStringData d2 = { "XX", 2 };
    CCharString other = { &d2 };
    if (Calc(0, "XX", 2) > h + 9 && obj.GetNoHelperPoints(other) != 0) { std::printf("00a36140_TEST FAIL\n"); return 1; }
    std::printf("00a36140_TEST PASS\n");
    return 0;
}