// Behaviour test for NGraphicAppearance::CTCGraphicAppearance::GetAllHelperPointNames (retail 0x04BDA30): model copy + stub callees.
#include <cstdio>
#include <cstdlib>

static int g_destroyCalls = 0;
static int g_baseDtors = 0;
static int g_reserveArg = 0;

class CCharString { public: long id; };
template <class T> struct CStdVector { T items[16]; int count; };

struct SCountBlock {
    long m_references;
    void (*m_pfnDestroy)(void*);
    void* m_pObject;
};
static void DestroyMesh(void*) { ++g_destroyCalls; }

struct CCountedPointer {
    void* m_pObject;
    SCountBlock* m_pCount;
    CCountedPointer() : m_pObject(0), m_pCount(0) { }
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

class CBaseClass {
public:
    CBaseClass() {}
    virtual ~CBaseClass() { ++g_baseDtors; }
};

class C3DMeshObject : public CBaseClass { public:
    CCountedPointer    Mesh;
    bool               InversePositionMatrixSetup;
    float              Scale;
    C3DMeshObject() : CBaseClass(), Mesh(), InversePositionMatrixSetup(false) { }
    virtual ~C3DMeshObject() { }
    void GetAllHelperPointNames(CStdVector<CCharString>& names) const
    {
        long id = (long)(size_t)Mesh.m_pObject;
        names.items[names.count].id = id * 10; ++names.count;
        names.items[names.count].id = id * 10 + 1; ++names.count;
    }
};

namespace NGraphicAppearance {
class CMeshInfo { public: long Index; float SizeX; };
}

template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end;
    CRetailVector() : _M_start(0), _M_finish(0), _M_end(0) {}
    ~CRetailVector() { if (_M_start) std::free(_M_start); }
    T* begin() const { return _M_start; }
    T* end() const { return _M_finish; }
    void reserve(unsigned long count) { g_reserveArg = (int)count; if (!_M_start) { _M_start = _M_finish = (T*)std::malloc(sizeof(T) * 16); _M_end = _M_start + 16; } }
    void push_back(const T& v) { *_M_finish++ = v; }
};
typedef CRetailVector<NGraphicAppearance::CMeshInfo> CMeshVector;

namespace NGraphicAppearance {
class CAGraphicAppearanceComponent { public:
    long first, second;
    void GetAllMeshes(CMeshVector& meshes) const
    {
        CMeshInfo a = { first, 1.0f }; meshes.push_back(a);
        CMeshInfo b = { second, 2.0f }; meshes.push_back(b);
    }
};
}

struct CComponentListNode {
    CComponentListNode* m_next;
    CComponentListNode* m_prev;
    NGraphicAppearance::CAGraphicAppearanceComponent* m_value;
};
struct CComponentList { CComponentListNode* m_head; };

static float g_lastScale = 0.0f;

namespace NGraphicAppearance {
class CTCGraphicAppearance { public:
    CComponentList Components;
    void GetAllMeshes(CMeshVector& meshes) const
    {
        meshes.reserve(4);
        for (CComponentListNode* node = Components.m_head->m_next; node != Components.m_head; node = node->m_next)
            node->m_value->GetAllMeshes(meshes);
    }
    void GetMeshObject(const CMeshInfo& info, float scale, C3DMeshObject& out) const
    {
        g_lastScale = scale;
        out.Mesh.m_pObject = (void*)(size_t)info.Index;
        SCountBlock* block = new SCountBlock;
        block->m_references = 1; block->m_pfnDestroy = DestroyMesh; block->m_pObject = out.Mesh.m_pObject;
        out.Mesh.m_pCount = block;
    }
    void GetAllHelperPointNames(CStdVector<CCharString>& names) const;
};

void CTCGraphicAppearance::GetAllHelperPointNames(CStdVector<CCharString>& names) const
{
    CMeshVector meshes;
    GetAllMeshes(meshes);
    for (CMeshInfo* mesh = meshes.begin(); mesh != meshes.end(); ++mesh) {
        if (mesh->Index != 0) {
            C3DMeshObject object;
            GetMeshObject(*mesh, 1.0f, object);
            object.GetAllHelperPointNames(names);
        }
    }
}
}

int main()
{
    int fail = 0;
    NGraphicAppearance::CAGraphicAppearanceComponent c1 = { 3, 0 }, c2 = { 5, 7 };
    CComponentListNode head, n1, n2;
    head.m_next = &n1; n1.m_next = &n2; n2.m_next = &head; n1.m_value = &c1; n2.m_value = &c2;
    NGraphicAppearance::CTCGraphicAppearance ga; ga.Components.m_head = &head;
    CStdVector<CCharString> names; names.count = 0;
    ga.GetAllHelperPointNames(names);
    if (names.count != 6 || names.items[0].id != 30 || names.items[1].id != 31 || names.items[2].id != 50 || names.items[5].id != 71) fail = 1;
    if (g_reserveArg != 4 || g_lastScale != 1.0f || g_destroyCalls != 3 || g_baseDtors != 3) fail = 2;
    // no components: nothing appended
    head.m_next = &head; names.count = 0;
    ga.GetAllHelperPointNames(names);
    if (names.count != 0 || g_destroyCalls != 3) fail = 3;
    if (fail) { std::printf("004bda30_TEST FAIL\n"); return 1; }
    std::printf("004bda30_TEST PASS\n");
    return 0;
}