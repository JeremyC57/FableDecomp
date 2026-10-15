// Behaviour test for 00530530 NUISystem::CComponent::HasStateInChildren: true when a child has the
// state itself or (recursively) in its own children; the component's own state does not count.
#include <cstdio>
#include <vector>

struct CComponent {
    std::vector<CComponent*> Children; unsigned long m_state;
    CComponent() : m_state(0) { }
    virtual ~CComponent() { }
    virtual bool HasState(unsigned long state) { return m_state == state; }
    virtual bool HasStateInChildren(unsigned long state)
    {
        for (unsigned long i = 0; i < Children.size(); ++i) {
            if (Children[i]->HasState(state)) return true;
            if (Children[i]->HasStateInChildren(state)) return true;
        }
        return false;
    }
};

int main()
{
    CComponent root, a, b, c;
    root.Children.push_back(&a); root.Children.push_back(&b); b.Children.push_back(&c);
    root.m_state = 9; a.m_state = 1; b.m_state = 2; c.m_state = 3;
    bool ok = root.HasStateInChildren(1) && root.HasStateInChildren(3) && !root.HasStateInChildren(9)
        && !root.HasStateInChildren(4) && !a.HasStateInChildren(1) && b.HasStateInChildren(3);
    if (!ok) { std::printf("00530530_TEST FAIL\n"); return 1; }
    std::printf("00530530_TEST PASS\n");
    return 0;
}
