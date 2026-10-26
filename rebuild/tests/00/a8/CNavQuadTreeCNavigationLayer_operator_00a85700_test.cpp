// Behaviour test for 0x00A85700 CNavQuadTree::CNavigationLayer::operator= (implicit).
#include <cstdio>
#include <cstring>

class C2DLineF { public: float X0, Y0, X1, Y1; };
class CCounted {                    // stands in for the STLport containers: counts copies and assignments
public:
    int Value;
    static int s_copies;
    static int s_assigns;
    CCounted(int v) : Value(v) {}
    CCounted(const CCounted& o) : Value(o.Value) { ++s_copies; }
    CCounted& operator=(const CCounted& o) { Value = o.Value; ++s_assigns; return *this; }
};
int CCounted::s_copies = 0;
int CCounted::s_assigns = 0;
class CSwitchableMap {              // CVectorMap: the data array, the empty comparator, the dirty flag
public:
    CCounted Data;
    bool     Dirty;
    CSwitchableMap(int v, bool d) : Data(v), Dirty(d) {}
};
class CNavigationLayer {
public:
    CCounted       Nodes;
    CCounted       LineLists;
    CSwitchableMap SwitchableLineLists;
    CNavigationLayer(int n, int l, int s, bool d) : Nodes(n), LineLists(l), SwitchableLineLists(s, d) {}
    CNavigationLayer(const CNavigationLayer& other)
        : Nodes(other.Nodes), LineLists(other.LineLists), SwitchableLineLists(other.SwitchableLineLists) {}
};

int main()
{
    CNavigationLayer a(1, 2, 3, true);
    CNavigationLayer b(4, 5, 6, false);
    CCounted::s_assigns = 0;
    CNavigationLayer* r = &(a = b);
    if (!(r == &a && CCounted::s_assigns == 3 && a.Nodes.Value == 4 && a.LineLists.Value == 5 && a.SwitchableLineLists.Data.Value == 6 && !a.SwitchableLineLists.Dirty)) {
        std::printf("00a85700_TEST FAIL\n");
        return 1;
    }
    std::printf("00a85700_TEST PASS\n");
    return 0;
}