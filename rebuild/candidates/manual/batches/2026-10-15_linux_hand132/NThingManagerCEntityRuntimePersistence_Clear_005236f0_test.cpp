#include <cstdio>
// Behaviour test for 005236f0.

// NThingManager::CEntityRuntimePersistence::Clear: both script arrays emptied (storage released) and the dead-entity map reset
// to an empty tree whose header points at itself.
struct SNode { long color; SNode* parent; SNode* left; SNode* right; };
struct CArray { int count; int deallocated; void Clear() { count = 0; ++deallocated; } };
struct CMap {
    SNode* m_pHead; unsigned int m_size; int erased;
    void _Erase(SNode* n) { if (n) ++erased; }
    void clear()
    {
        if (m_size != 0) { _Erase(m_pHead->parent); m_pHead->left = m_pHead; m_pHead->parent = 0; m_pHead->right = m_pHead; m_size = 0; }
    }
};
struct CEntityRuntimePersistence {
    CArray Level; CArray Game; CMap Dead;
    void Clear() { Level.Clear(); Game.Clear(); Dead.clear(); }
};
int main()
{
    SNode head; SNode root; head.parent = &root; head.left = &root; head.right = &root;
    CEntityRuntimePersistence p; p.Level.count = 3; p.Level.deallocated = 0; p.Game.count = 2; p.Game.deallocated = 0;
    p.Dead.m_pHead = &head; p.Dead.m_size = 1; p.Dead.erased = 0;
    p.Clear();
    int ok = p.Level.count == 0 && p.Game.count == 0 && p.Level.deallocated == 1 && p.Dead.m_size == 0 && p.Dead.erased == 1
          && head.parent == 0 && head.left == &head && head.right == &head;
    p.Clear();
    ok = ok && p.Dead.erased == 1 && p.Game.deallocated == 2;
    if (!ok) { std::printf("005236f0_TEST FAIL\n"); return 1; }
    std::printf("005236f0_TEST PASS\n");
    return 0;
}
