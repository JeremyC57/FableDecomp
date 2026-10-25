// Behaviour test for 0x0042CD84 NUISystem::CDeletion::CDeletion(const CDeletion&): Method copied, the list copied through its copy constructor.
#include <cstdio>
static int g_listCopies = 0;
namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class list {
public:
    void* _M_node;
    list(void* n) : _M_node(n) { }
    list(const list<_Tp, _Alloc>& __x) : _M_node(__x._M_node) { ++g_listCopies; }
};
}
namespace NUISystem {
class CComponent;
enum EDeletionMethod { DELETION_METHOD_DEFAULT };
class CDeletion {
public:
    EDeletionMethod        Method;
    std::list<CComponent*> AssociatedParents;
    CDeletion(EDeletionMethod m, void* n) : Method(m), AssociatedParents(n) { }
    CDeletion(const CDeletion& other) : Method(other.Method), AssociatedParents(other.AssociatedParents) { }
};
}
int main()
{
    int node;
    NUISystem::CDeletion a((NUISystem::EDeletionMethod)2, &node);
    NUISystem::CDeletion b(a);
    if (sizeof(NUISystem::CDeletion) != 8 || b.Method != 2 || b.AssociatedParents._M_node != &node || g_listCopies != 1) {
        std::printf("0042cd84_TEST FAIL\n");
        return 1;
    }
    std::printf("0042cd84_TEST PASS\n");
    return 0;
}
