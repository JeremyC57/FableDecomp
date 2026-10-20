// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::HasDummyObject (retail 0x004BE2C0): model copy + stub callees.
#include <cstdio>
#include <cstdlib>
#include <cstring>

static int g_destroyCalls = 0;

struct CCharStringData { char* m_pData; unsigned long m_length; };
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
    const char* GetPString() const { return m_pStringData == 0 ? "" : m_pStringData->m_pData; }
};
unsigned long Calc_004014a0(unsigned long seed, const char* data, unsigned long length)
{
    unsigned long h = seed;
    for (unsigned long i = 0; i < length; ++i) h = h * 31 + (unsigned char)data[i];
    return h + 1;
}

struct SCountBlock { long m_references; void (*m_pfnDestroy)(void*); void* m_pObject; };
static void DestroyStats(void*) { ++g_destroyCalls; }
template <class T> struct CCountedPointer {
    T* m_pObject;
    SCountBlock* m_pCount;
    T* operator->() const { return m_pObject; }
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

struct C3DMeshStats {
    unsigned long dummyKey;
    bool HasDummyObject(unsigned long key) const { return key == dummyKey; }
};
struct C3DMeshInfo {
    C3DMeshStats* stats;
    CCountedPointer<const C3DMeshStats> GetMeshStats() const
    {
        CCountedPointer<const C3DMeshStats> p;
        p.m_pObject = stats;
        p.m_pCount = new SCountBlock;
        p.m_pCount->m_references = 1; p.m_pCount->m_pfnDestroy = DestroyStats; p.m_pCount->m_pObject = stats;
        return p;
    }
};
static C3DMeshStats g_stats[3];
static C3DMeshInfo g_infos[3];
struct CMeshDataBank { const C3DMeshInfo* GetMeshInfo(long index) { return &g_infos[index]; } };
CMeshDataBank* g_pMeshDataBank = 0;

namespace NGraphicAppearance { struct CMeshInfo { long Index; float SizeX; }; }

template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CRetailVector() { if (_M_start) std::free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    void reserve(unsigned long) { if (!_M_start) { _M_start = _M_finish = (T*)std::malloc(sizeof(T) * 16); _M_end = _M_start + 16; } }
    void push_back(const T& v) { *_M_finish++ = v; }
};
typedef CRetailVector<NGraphicAppearance::CMeshInfo> CMeshVector;

namespace NGraphicAppearance {
struct CAGraphicAppearanceComponent {
    long index;
    void GetAllMeshes(CMeshVector& meshes) const { CMeshInfo a = { index, 1.0f }; meshes.push_back(a); }
};
}
struct CComponentListNode { CComponentListNode* m_next; CComponentListNode* m_prev; NGraphicAppearance::CAGraphicAppearanceComponent* m_value; };
struct CComponentList { CComponentListNode* m_head; };

namespace NGraphicAppearance {
struct CTCGraphicAppearance {
    CComponentList Components;
    void GetAllMeshes(CMeshVector& meshes) const
    {
        meshes.reserve(4);
        for (CComponentListNode* node = Components.m_head->m_next; node != Components.m_head; node = node->m_next)
            node->m_value->GetAllMeshes(meshes);
    }
    bool HasDummyObject(const CCharString& name) const;
};

bool CTCGraphicAppearance::HasDummyObject(const CCharString& name) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    unsigned long key = Calc_004014a0(0, name.GetPString(), name.GetLength());
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        const C3DMeshInfo* meshInfo = g_pMeshDataBank->GetMeshInfo(mesh->Index);
        if (meshInfo->GetMeshStats()->HasDummyObject(key))
            return true;
    }
    return false;
}
}

int main()
{
    int fail = 0;
    CMeshDataBank bank; g_pMeshDataBank = &bank;
    char text[] = "hand_r";
    CCharStringData data = { text, 6 };
    CCharString name = { &data };
    unsigned long key = Calc_004014a0(0, text, 6);
    for (int i = 0; i < 3; ++i) { g_infos[i].stats = &g_stats[i]; g_stats[i].dummyKey = 0; }
    g_stats[2].dummyKey = key;
    NGraphicAppearance::CAGraphicAppearanceComponent c1 = { 1 }, c2 = { 2 };
    CComponentListNode head, n1, n2;
    head.m_next = &n1; n1.m_next = &n2; n2.m_next = &head; n1.m_value = &c1; n2.m_value = &c2;
    NGraphicAppearance::CTCGraphicAppearance ga; ga.Components.m_head = &head;
    if (!ga.HasDummyObject(name) || g_destroyCalls != 2) fail = 1;
    g_stats[2].dummyKey = 0;
    if (ga.HasDummyObject(name) || g_destroyCalls != 4) fail = 2;
    // a null name hashes the empty string
    CCharString empty = { 0 };
    g_stats[1].dummyKey = Calc_004014a0(0, "", 0);
    if (!ga.HasDummyObject(empty)) fail = 3;
    if (fail) { std::printf("004be2c0_TEST FAIL\n"); return 1; }
    std::printf("004be2c0_TEST PASS\n");
    return 0;
}