#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CTree {
    void* m_pHead; unsigned long m_keys[3]; void* m_nodes[3];
    void* Find(const unsigned long& key) { for (int i = 0; i < 3; ++i) if (m_keys[i] == key) return m_nodes[i]; return m_pHead; }
};
struct CKeyedSet {
    CTree m_tree;
    bool Contains(unsigned long key, unsigned long) { CTree* tree = &m_tree; return tree->Find(key) != tree->m_pHead; }
};
static bool Check()
{
    CKeyedSet s; s.m_tree.m_pHead = (void*)&s; s.m_tree.m_keys[0] = 3; s.m_tree.m_keys[1] = 5; s.m_tree.m_keys[2] = 9;
    for (int i = 0; i < 3; ++i) s.m_tree.m_nodes[i] = (void*)&s.m_tree.m_keys[i];
    return s.Contains(5, 0) && s.Contains(9, 0) && !s.Contains(4, 0) && !s.Contains(0, 0);
}
int main()
{
    if (!Check()) { std::printf("0051f010_TEST FAIL\n"); return 1; }
    std::printf("0051f010_TEST PASS\n");
    return 0;
}
