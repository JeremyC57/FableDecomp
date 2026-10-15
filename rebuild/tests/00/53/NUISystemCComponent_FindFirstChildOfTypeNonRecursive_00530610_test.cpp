// Behaviour test for 00530610 NUISystem::CComponent::FindFirstChildOfTypeNonRecursive: this when its
// type matches, else the FIRST direct child of the type; grandchildren are not searched.
#include <cstdio>
#include <vector>

struct CComponent {
    std::vector<CComponent*> Children; long m_type;
    CComponent(long t) : m_type(t) { }
    virtual ~CComponent() { }
    virtual long GetType() { return m_type; }
    CComponent* FindFirstChildOfTypeNonRecursive(long type)
    {
        if (GetType() == type) return this;
        for (unsigned long i = 0; i < Children.size(); ++i)
            if (Children[i]->GetType() == type) return Children[i];
        return 0;
    }
};

int main()
{
    CComponent root(1), a(2), b(3), b2(3), c(4);
    root.Children.push_back(&a); root.Children.push_back(&b); root.Children.push_back(&b2); a.Children.push_back(&c);
    bool ok = root.FindFirstChildOfTypeNonRecursive(1) == &root && root.FindFirstChildOfTypeNonRecursive(3) == &b
        && root.FindFirstChildOfTypeNonRecursive(4) == 0 && a.FindFirstChildOfTypeNonRecursive(4) == &c;
    if (!ok) { std::printf("00530610_TEST FAIL\n"); return 1; }
    std::printf("00530610_TEST PASS\n");
    return 0;
}