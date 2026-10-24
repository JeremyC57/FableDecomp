// Behaviour test for 0x00531E90 NUISystem::CComponent::SetDeletion: assigns the by-value deletion record (method and the associated-parents list) to the Deletion member; the argument is destroyed by the callee
#include <cstdio>
static int g_assigns = 0, g_destroys = 0;
class CComponent;
enum EDeletionMethod { DELETION_METHOD_DEFAULT, DELETION_METHOD_FADE };
struct CComponentList {
    void* _M_node;
    CComponentList() : _M_node(0) { }
    CComponentList(const CComponentList& other) : _M_node(other._M_node) { }
    CComponentList& operator=(const CComponentList& other) { _M_node = other._M_node; ++g_assigns; return *this; }
    ~CComponentList() { ++g_destroys; }
};
struct CDeletion {
    EDeletionMethod Method;
    CComponentList  AssociatedParents;
};
class CComponent {
public:
    virtual void SetDeletion(CDeletion deletion);
    virtual ~CComponent() { }
    CDeletion Deletion;
};
void CComponent::SetDeletion(CDeletion deletion)
{
    Deletion = deletion;
}
int main()
{
    int marker = 0;
    int destroysBefore;
    CComponent c;
    c.Deletion.Method = DELETION_METHOD_DEFAULT;
    {
        CDeletion d;
        d.Method = DELETION_METHOD_FADE;
        d.AssociatedParents._M_node = &marker;
        destroysBefore = g_destroys;
        c.SetDeletion(d);
        if (g_destroys != destroysBefore + 1) { std::printf("00531e90_TEST FAIL\n"); return 1; }
    }
    if (c.Deletion.Method != DELETION_METHOD_FADE || c.Deletion.AssociatedParents._M_node != &marker || g_assigns != 1)
        { std::printf("00531e90_TEST FAIL\n"); return 1; }
    std::printf("00531e90_TEST PASS\n");
    return 0;
}