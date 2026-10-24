#include <cstdio>
#include <list>
#include <algorithm>
struct CComponent;
struct CHoverable {
    int pad;
    static std::list<CComponent*> GHoveredComponents;
    CComponent* AsComponent() { return (CComponent*)this; }
    void RemoveFromHoveredList()
    {
        std::list<CComponent*>::iterator it = std::find(GHoveredComponents.begin(), GHoveredComponents.end(), AsComponent());
        if (it != GHoveredComponents.end()) GHoveredComponents.erase(it);
    }
};
std::list<CComponent*> CHoverable::GHoveredComponents;
int main()
{
    CHoverable a, b;
    CHoverable::GHoveredComponents.push_back(a.AsComponent()); CHoverable::GHoveredComponents.push_back(b.AsComponent());
    a.RemoveFromHoveredList(); a.RemoveFromHoveredList();
    if (CHoverable::GHoveredComponents.size() != 1 || CHoverable::GHoveredComponents.front() != b.AsComponent()) { std::printf("0055bbf0_TEST FAIL\n"); return 1; }
    std::printf("0055bbf0_TEST PASS\n");
    return 0;
}