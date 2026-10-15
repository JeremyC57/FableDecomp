// Behaviour test for 0x007554C0: the draw request ends with type 10, its scalars and flags cleared, an
// empty vector and an empty tree whose freshly allocated header is red and linked to itself.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
struct SRbTreeNode { unsigned char m_color; SRbTreeNode* m_pParent; SRbTreeNode* m_pLeft; SRbTreeNode* m_pRight; long m_value; };
struct CRbTreeBase {
    SRbTreeNode* m_pHeader;
    CRbTreeBase() : m_pHeader(0) { m_pHeader = (SRbTreeNode*)std::malloc(sizeof(SRbTreeNode)); }
};
struct CRbTree : CRbTreeBase {
    unsigned long m_nodeCount;
    CRbTree() : m_nodeCount(0)
    {
        m_pHeader->m_color = 0;
        m_pHeader->m_pParent = 0;
        m_pHeader->m_pLeft = m_pHeader;
        m_pHeader->m_pRight = m_pHeader;
    }
};
struct CRetailVector { void* m_pFirst; void* m_pLast; void* m_pEnd; CRetailVector() : m_pFirst(0), m_pLast(0), m_pEnd(0) { } };
struct CLookDrawRequestBase {
    long m_type; long m_count; bool m_flag08, m_flag09, m_flag0a;
    unsigned char _pad[0x30];
    long m_count3c; bool m_flag40, m_flag41, m_flag42;
    unsigned char _pad43[8];
    CRetailVector m_items;
    CRbTree       m_tree;
    CLookDrawRequestBase() : m_type(8), m_count(0), m_flag08(false), m_flag09(false), m_flag0a(false), m_count3c(0), m_flag40(false), m_flag41(false), m_flag42(false) { }
};
struct CLookDrawRequest : CLookDrawRequestBase { CLookDrawRequest() { m_type = 10; } };
static double g_storage[64];

int main()
{
    std::memset(g_storage, 0xAB, sizeof g_storage);
    CLookDrawRequest* p = new (g_storage) CLookDrawRequest;
    SRbTreeNode* h = p->m_tree.m_pHeader;
    if (p->m_type != 10 || p->m_count != 0 || p->m_flag08 || p->m_flag09 || p->m_flag0a || p->m_count3c != 0 || p->m_flag40 || p->m_flag41 || p->m_flag42) { std::printf("007554c0_TEST FAIL\n"); return 1; }
    if (p->m_items.m_pFirst || p->m_items.m_pLast || p->m_items.m_pEnd || !h || h->m_color != 0 || h->m_pParent != 0 || h->m_pLeft != h || h->m_pRight != h || p->m_tree.m_nodeCount != 0) { std::printf("007554c0_TEST FAIL\n"); return 1; }
    std::free(h);
    std::printf("007554c0_TEST PASS\n");
    return 0;
}
