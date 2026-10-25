// Behaviour test for 0x00A85010 CNavQuadTree::CNavigationLayer copy constructor.
#include <cstdio>
#include <cstring>

struct C2DLineF { float X0, Y0, X1, Y1; };
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
    CNavigationLayer a(11, 22, 33, true);
    CCounted::s_copies = 0;
    CNavigationLayer b(a);
    if (!(CCounted::s_copies == 3 && b.Nodes.Value == 11 && b.LineLists.Value == 22 && b.SwitchableLineLists.Data.Value == 33 && b.SwitchableLineLists.Dirty)) {
        std::printf("00a85010_TEST FAIL\n");
        return 1;
    }
    std::printf("00a85010_TEST PASS\n");
    return 0;
}