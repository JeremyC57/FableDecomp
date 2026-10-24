// Behaviour test for 0x00530690 NUISystem::CComponent::CanScroll: returns the child scroll amount of largest magnitude (first wins on ties), 0 with no children
#include <cstdio>
#include <cmath>
template<class T> struct CRetailVector {
    T* first; T* last; T* end_of_storage;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() { return iterator(first); }
    unsigned long size() const { return (unsigned long)(last - first); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct C2DBoxF { float a, b, c, d; };
struct CComponent;
struct CCountedComponentPointer {
    CComponent* m_pObject;
    void* m_pCount;
    CComponent* operator->() const { return m_pObject; }
};
struct CComponent {
    virtual float CanScroll(bool horizontal, C2DBoxF& box);
    CRetailVector<CCountedComponentPointer> Children;
    CComponent() { Children.first = Children.last = Children.end_of_storage = 0; }
    virtual ~CComponent() { }
};
float CComponent::CanScroll(bool horizontal, C2DBoxF& box)
{
    float result = 0.0f;
    for (unsigned long i = 0; i < Children.size(); ++i) {
        float amount = Children[i]->CanScroll(horizontal, box);
        if ((float)std::fabs(amount) > (float)std::fabs(result))
            result = amount;
    }
    return result;
}
struct CLeaf : CComponent {
    float h, v;
    CLeaf(float hh, float vv) : h(hh), v(vv) { }
    virtual float CanScroll(bool horizontal, C2DBoxF&) { return horizontal ? h : v; }
};
int main()
{
    C2DBoxF box = { 0, 0, 1, 1 };
    CComponent root;
    if (root.CanScroll(true, box) != 0.0f) { std::printf("00530690_TEST FAIL\n"); return 1; }
    CLeaf a(1.0f, -5.0f), b(-3.0f, 2.0f), c(3.0f, 5.0f);
    CCountedComponentPointer kids[3];
    kids[0].m_pObject = &a; kids[1].m_pObject = &b; kids[2].m_pObject = &c;
    kids[0].m_pCount = kids[1].m_pCount = kids[2].m_pCount = 0;
    root.Children.first = kids; root.Children.last = kids + 3; root.Children.end_of_storage = kids + 3;
    if (root.CanScroll(true, box) != -3.0f) { std::printf("00530690_TEST FAIL\n"); return 1; }
    if (root.CanScroll(false, box) != -5.0f) { std::printf("00530690_TEST FAIL\n"); return 1; }
    root.Children.first = root.Children.last = root.Children.end_of_storage = 0;
    std::printf("00530690_TEST PASS\n");
    return 0;
}