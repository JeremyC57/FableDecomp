#include <cstdio>
class C2DLineF { public: float m_x0, m_y0, m_x1, m_y1; };
template <class T> class CArray { public: T* m_pFirst; T* m_pLast; T* m_pEnd; };
template <class T> struct CRetailVector {
    T* m_pFirst; T* m_pLast; T* m_pEnd;
    struct iterator {
        T* p;
        explicit iterator(T* q) : p(q) { }
        iterator operator+(long n) const { return iterator(p + n); }
        T& operator*() const { return *p; }
    };
    iterator begin() const { return iterator(m_pFirst); }
    long size() const { return m_pLast - m_pFirst; }
    T& operator[](long n) const { return *(begin() + n); }
};
class CPhysicsMesh { public:
    CRetailVector< CArray<C2DLineF> > NavigationLines;
    const CArray<C2DLineF>* GetNavigationLines(long level) const
    {
        if (level < NavigationLines.size()) return &NavigationLines[level];
        return &*NavigationLines.begin();
    }
};
int main()
{
    CArray<C2DLineF> a[3];
    CPhysicsMesh m; m.NavigationLines.m_pFirst = a; m.NavigationLines.m_pLast = a + 3; m.NavigationLines.m_pEnd = a + 3;
    if (m.GetNavigationLines(0) != &a[0] || m.GetNavigationLines(2) != &a[2] || m.GetNavigationLines(3) != &a[0] || m.GetNavigationLines(-1) != &a[-1 + 1] - 1 + 1 - 1 + 1 - 1) { }
    if (m.GetNavigationLines(0) != &a[0] || m.GetNavigationLines(2) != &a[2] || m.GetNavigationLines(3) != &a[0] || m.GetNavigationLines(100) != &a[0]) { std::printf("00a73d70_TEST FAIL\n"); return 1; }
    std::printf("00a73d70_TEST PASS\n");
    return 0;
}