#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>
static int g_rootCalls = 0, g_componentCalls = 0;
struct CEngineRoot_0099a2f0 { virtual void v00() { } CEngineRoot_0099a2f0() { ++g_rootCalls; } };
struct CEngineComponent {
    virtual void v00() { }
    long GlobalPointerAddress; bool m_enabled; bool KeepAlive;
    CEngineComponent() : GlobalPointerAddress(0), m_enabled(true), KeepAlive(false) { ++g_componentCalls; }
};
struct CEngineInternalAttachedPrimitiveInfo { long m_words[3]; };
template <class T> struct CRetailSTLportList {
    struct SNode { SNode* next; SNode* prev; T value; };
    SNode* m_node;
    CRetailSTLportList()
    {
        m_node = 0;
        SNode* node = (SNode*)std::malloc(sizeof(SNode));
        node->next = node;
        node->prev = node;
        m_node = node;
    }
};
struct CEnginePrimitiveAttachmentManager : CEngineRoot_0099a2f0, CEngineComponent {
    CRetailSTLportList<CEngineInternalAttachedPrimitiveInfo> AttachedPrimitives;
    virtual void v01() { }
    CEnginePrimitiveAttachmentManager() { }
};
int main()
{
    static unsigned char buf[sizeof(CEnginePrimitiveAttachmentManager)];
    std::memset(buf, 0xCD, sizeof(buf));
    CEnginePrimitiveAttachmentManager* m = new (buf) CEnginePrimitiveAttachmentManager();
    bool ok = g_rootCalls == 1 && g_componentCalls == 1 && m->m_enabled && !m->KeepAlive
        && m->AttachedPrimitives.m_node != 0
        && m->AttachedPrimitives.m_node->next == m->AttachedPrimitives.m_node
        && m->AttachedPrimitives.m_node->prev == m->AttachedPrimitives.m_node;
    std::free(m->AttachedPrimitives.m_node);
    if (!ok) { std::printf("00b39f30_TEST FAIL\n"); return 1; }
    std::printf("00b39f30_TEST PASS\n");
    return 0;
}