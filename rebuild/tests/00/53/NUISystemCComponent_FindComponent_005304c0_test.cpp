// Behaviour test for 005304c0 NUISystem::CComponent::FindComponent: the component itself, a nested
// descendant (depth first through each child's FindComponent), or null when absent.
#include <cstdio>
#include <vector>

struct CComponent {
    std::vector<CComponent*> Children;
    virtual ~CComponent() { }
    virtual CComponent* FindComponent(CComponent* component)
    {
        if (component == this) return this;
        for (unsigned long i = 0; i < Children.size(); ++i) {
            CComponent* found = Children[i]->FindComponent(component);
            if (found) return found;
        }
        return 0;
    }
};

int main()
{
    CComponent root, a, b, c, stranger;
    root.Children.push_back(&a); root.Children.push_back(&b); b.Children.push_back(&c);
    bool ok = root.FindComponent(&root) == &root && root.FindComponent(&c) == &c
        && root.FindComponent(&a) == &a && root.FindComponent(&stranger) == 0 && a.FindComponent(&c) == 0;
    if (!ok) { std::printf("005304c0_TEST FAIL\n"); return 1; }
    std::printf("005304c0_TEST PASS\n");
    return 0;
}