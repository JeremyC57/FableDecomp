#include <cstdio>

struct CMesh { int id; };
static unsigned long g_reserved = 0;
struct CMeshVector {
    CMesh* m_items[16]; unsigned long m_count;
    void reserve(unsigned long count) { g_reserved = count; }
    void push(CMesh* m) { m_items[m_count++] = m; }
};
struct CAGraphicAppearanceComponent {
    CMesh* m_mesh;
    virtual ~CAGraphicAppearanceComponent() { }
    virtual void GetAllMeshes(CMeshVector& meshes) { meshes.push(m_mesh); }
};
struct CComponentListNode { CComponentListNode* m_next; CComponentListNode* m_prev; CAGraphicAppearanceComponent* m_value; };
struct CComponentList { CComponentListNode* m_head; };
struct CTCGraphicAppearance {
    CComponentList Components;
    void GetAllMeshes(CMeshVector& meshes)
    {
        meshes.reserve(4);
        for (CComponentListNode* node = Components.m_head->m_next; node != Components.m_head; node = node->m_next)
            node->m_value->GetAllMeshes(meshes);
    }
};

int main()
{
    CMesh m1 = { 1 }, m2 = { 2 };
    CAGraphicAppearanceComponent c1, c2; c1.m_mesh = &m1; c2.m_mesh = &m2;
    CComponentListNode head, n1, n2; head.m_next = &n1; n1.m_next = &n2; n2.m_next = &head; n1.m_value = &c1; n2.m_value = &c2;
    CTCGraphicAppearance a; a.Components.m_head = &head;
    CMeshVector v; v.m_count = 0;
    a.GetAllMeshes(v);
    if (g_reserved != 4 || v.m_count != 2 || v.m_items[0] != &m1 || v.m_items[1] != &m2) { std::printf("004bd420_TEST FAIL\n"); return 1; }
    head.m_next = &head; v.m_count = 0; a.GetAllMeshes(v);
    if (v.m_count != 0) { std::printf("004bd420_TEST FAIL\n"); return 1; }
    std::printf("004bd420_TEST PASS\n");
    return 0;
}