#include <cstdio>
#include <stddef.h>
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage;
    struct iterator { T* p; explicit iterator(T* q) : p(q) { } iterator operator+(long n) const { return iterator(p + n); } T& operator*() const { return *p; } };
    iterator begin() { return iterator(first); } unsigned long size() const { return (unsigned long)(last - first); } T& operator[](long n) { return *(begin() + n); } };
struct CComponent; struct CCountedComponentPointer { CComponent* m_pObject; void* m_pCount; };
static int g_calls = 0;
struct CComponent {
    virtual void v00() { } virtual void v01() { } virtual void v02() { } virtual void v03() { } virtual void v04() { } virtual void v05() { } virtual void v06() { } virtual void v07() { } virtual void v08() { } virtual void v09() { }
    virtual bool HasChild(CComponent* child);
    unsigned char _body[0xb0 - 0x04]; CRetailVector<CCountedComponentPointer> Children;
    CComponent() { Children.first = Children.last = Children.end_of_storage = 0; }
};
bool CComponent::HasChild(CComponent* child)
{
    ++g_calls;
    if (this == child) return true;
    for (long i = Children.size() - 1; i != -1; --i) { bool found = Children[i].m_pObject->HasChild(child); if (found) return found; }
    return false;
}
int main()
{
    CComponent root, a, b, grandchild, stranger; CCountedComponentPointer rootKids[2] = { { &a, 0 }, { &b, 0 } }; CCountedComponentPointer bKids[1] = { { &grandchild, 0 } };
    root.Children.first = rootKids; root.Children.last = rootKids + 2; b.Children.first = bKids; b.Children.last = bKids + 1;
    if (offsetof(CComponent, Children) != 0xb0 || !root.HasChild(&root) || g_calls != 1) { std::printf("005309e0_TEST FAIL\n"); return 1; }
    g_calls = 0;
    if (!root.HasChild(&grandchild) || g_calls != 3) { std::printf("005309e0_TEST FAIL\n"); return 1; }   // root -> b (last first) -> grandchild
    g_calls = 0;
    if (root.HasChild(&stranger) || g_calls != 4 || a.HasChild(&b)) { std::printf("005309e0_TEST FAIL\n"); return 1; }
    std::printf("005309e0_TEST PASS\n"); return 0;
}