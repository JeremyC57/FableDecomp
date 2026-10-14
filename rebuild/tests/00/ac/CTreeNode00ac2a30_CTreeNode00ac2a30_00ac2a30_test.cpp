#include <cstdio>
#include <cstdlib>
#include <cstring>
static int g_freed = 0; static void* g_freedPtr[8];
static void free_(void* p) { g_freedPtr[g_freed++] = p; }
#define free free_
static int g_dtors = 0;
struct CTreeNode_00ac2a30 { unsigned char _pad[0x30]; void* m_pBuffer; unsigned char _pad34[8]; CTreeNode_00ac2a30* m_pLeft; CTreeNode_00ac2a30* m_pRight;
    ~CTreeNode_00ac2a30() { ++g_dtors; delete m_pLeft; delete m_pRight; if (m_pBuffer) free(m_pBuffer); } };
int main() {
    int x1, x2; CTreeNode_00ac2a30* root = new CTreeNode_00ac2a30; CTreeNode_00ac2a30* l = new CTreeNode_00ac2a30; root->m_pLeft = l; root->m_pRight = 0; root->m_pBuffer = &x1; l->m_pLeft = 0; l->m_pRight = 0; l->m_pBuffer = &x2;
    delete root; if (g_freed != 2 || g_freedPtr[0] != &x2 || g_freedPtr[1] != &x1 || g_dtors != 2) { std::printf("00ac2a30_TEST FAIL\n"); return 1; }
    std::printf("00ac2a30_TEST PASS\n"); return 0;
}