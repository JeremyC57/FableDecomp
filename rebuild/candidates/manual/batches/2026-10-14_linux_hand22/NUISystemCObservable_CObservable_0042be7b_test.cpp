#include <cstdio>
#include <cstdlib>
namespace NUISystem {
struct CObserver;
struct SRetailAllocator { SRetailAllocator() { } };
struct SNode { SNode* next; SNode* prev; void* data; };
struct CRetailListBase {
    void* m_pNode;
    CRetailListBase(const SRetailAllocator& allocator)
    {
        m_pNode = 0;
        SNode* n = static_cast<SNode*>(std::malloc(sizeof(SNode)));
        n->next = n; n->prev = n;
        m_pNode = n;
    }
};
struct CRetailList : CRetailListBase {
    CRetailList(const SRetailAllocator& allocator = SRetailAllocator()) : CRetailListBase(allocator) { }
};
struct CObservable {
    virtual void v00() { }
    CRetailList Observers;
    CObserver*  m_pExclusiveObserver;
    CRetailList ConcurrentExclusiveObservers;
    CObservable();
};
CObservable::CObservable()
    : Observers(), m_pExclusiveObserver(0), ConcurrentExclusiveObservers()
{
}
}
int main()
{
    NUISystem::CObservable o;
    NUISystem::SNode* a = static_cast<NUISystem::SNode*>(o.Observers.m_pNode);
    NUISystem::SNode* b = static_cast<NUISystem::SNode*>(o.ConcurrentExclusiveObservers.m_pNode);
    if (!a || !b || a == b || a->next != a || a->prev != a || b->next != b || o.m_pExclusiveObserver != 0 || sizeof(o) != 0x10) { std::printf("0042be7b_TEST FAIL\n"); return 1; }
    std::printf("0042be7b_TEST PASS\n");
    return 0;
}
