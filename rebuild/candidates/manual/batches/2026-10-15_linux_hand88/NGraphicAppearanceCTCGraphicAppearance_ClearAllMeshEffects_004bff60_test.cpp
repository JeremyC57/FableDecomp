#include <cstdio>

// A small red-black-shaped tree with an STLport header node (header->left = leftmost, header->parent = root,
// root->parent = header, header->right = rightmost); ClearMeshEffect erases nothing here, it only records keys.
struct CMeshEffectPriorityEntry { void* PEffect; long Priority; bool EffectsCarriedItems; bool EffectsWeapons; bool InheritedFromCarrier; };
struct SMeshEffectNode { long m_color; SMeshEffectNode* m_parent; SMeshEffectNode* m_left; SMeshEffectNode* m_right; void* m_key; CMeshEffectPriorityEntry m_value; };
struct CMeshEffectMapIterator {
    SMeshEffectNode* m_node;
    void Increment()
    {
        if (m_node->m_right != 0) {
            m_node = m_node->m_right;
            while (m_node->m_left != 0)
                m_node = m_node->m_left;
        } else {
            SMeshEffectNode* parent = m_node->m_parent;
            while (m_node == parent->m_right) {
                m_node = parent;
                parent = parent->m_parent;
            }
            if (m_node->m_right != parent)
                m_node = parent;
        }
    }
};
struct CMeshEffectMap { SMeshEffectNode* m_header; };
static void* g_cleared[8]; static int g_count = 0;
struct CTCGraphicAppearance {
    CMeshEffectMap MeshEffects;
    void ClearMeshEffect(void* key) { g_cleared[g_count++] = key; }
    void ClearAllMeshEffects()
    {
        SMeshEffectNode* cur = MeshEffects.m_header->m_left;
        CMeshEffectMapIterator it;
        it.m_node = cur;
        while (cur != MeshEffects.m_header) {
            it.Increment();
            if (cur->m_value.InheritedFromCarrier)
                ClearMeshEffect(cur->m_key);
            cur = it.m_node;
        }
    }
};

static void Init(SMeshEffectNode& n, void* key, bool inherited)
{
    n.m_color = 0; n.m_parent = n.m_left = n.m_right = 0; n.m_key = key; n.m_value.InheritedFromCarrier = inherited;
}

int main()
{
    // keys 1..5 in order: root 2 (left 1, right 4 (left 3, right 5))
    static int k[6];
    SMeshEffectNode header, n1, n2, n3, n4, n5;
    Init(n1, &k[1], true); Init(n2, &k[2], false); Init(n3, &k[3], true); Init(n4, &k[4], false); Init(n5, &k[5], true);
    n2.m_left = &n1; n1.m_parent = &n2; n2.m_right = &n4; n4.m_parent = &n2;
    n4.m_left = &n3; n3.m_parent = &n4; n4.m_right = &n5; n5.m_parent = &n4;
    header.m_parent = &n2; n2.m_parent = &header; header.m_left = &n1; header.m_right = &n5;
    CTCGraphicAppearance a; a.MeshEffects.m_header = &header;
    a.ClearAllMeshEffects();
    if (g_count != 3 || g_cleared[0] != &k[1] || g_cleared[1] != &k[3] || g_cleared[2] != &k[5]) { std::printf("004bff60_TEST FAIL\n"); return 1; }
    SMeshEffectNode empty; Init(empty, 0, false); empty.m_left = empty.m_right = &empty;
    a.MeshEffects.m_header = &empty; a.ClearAllMeshEffects();
    if (g_count != 3) { std::printf("004bff60_TEST FAIL\n"); return 1; }
    std::printf("004bff60_TEST PASS\n");
    return 0;
}
