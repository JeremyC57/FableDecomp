#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CSceneNode_00b91980 { int m_destroyed; CSceneNode_00b91980() : m_destroyed(0), m_pNext(0), m_pPrev(0), m_pNextSkipped(0), m_pPrevSkipped(0) { } virtual void v00() { } virtual void Destroy() { ++m_destroyed; }
    long m_references; CSceneNode_00b91980* m_pNext; CSceneNode_00b91980* m_pPrev; CSceneNode_00b91980* m_pNextSkipped; CSceneNode_00b91980* m_pPrevSkipped; };
struct CEngineScene { CSceneNode_00b91980* m_pNonPersistent;
    void ClearNonPersistent() { CSceneNode_00b91980* n = m_pNonPersistent; while (n) { CSceneNode_00b91980* next = n->m_pNext; n->m_pNext = 0; n->m_pPrev = 0; n->m_pNextSkipped = 0; n->m_pPrevSkipped = 0; if (--n->m_references == 0) n->Destroy(); n = next; } m_pNonPersistent = 0; } };
int main() {
    CSceneNode_00b91980 a, b; a.m_references = 1; b.m_references = 2; a.m_pNext = &b; b.m_pNext = 0; a.m_pPrev = &a; CEngineScene s; s.m_pNonPersistent = &a; s.ClearNonPersistent();
    if (a.m_destroyed != 1 || b.m_destroyed != 0 || b.m_references != 1 || a.m_pNext || a.m_pPrev || s.m_pNonPersistent) { std::printf("00b91980_TEST FAIL\n"); return 1; }
    std::printf("00b91980_TEST PASS\n"); return 0;
}