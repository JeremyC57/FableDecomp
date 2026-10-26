#include <cstdio>
// Behaviour test for 00a1be10 CNavigatorAStar::SmoothCorners: no work below three way points; otherwise each corner's two
// legs (p1-p0, p2-p1) are normalised once per lead step plus the final corner after the loop, in route order.
static int g_calls = 0;
static float g_seen[32][2];
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector& Normalise();
};
class CNavWayPoint { public:                              // 10 bytes
    C2DVector     Position;                        // +0x00
    unsigned char NavLayer;                        // +0x08
    bool          Removable;                       // +0x09
};
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T           _M_data;                           // +0x08
};
template <class T> struct list {                   // STLport list, 4 bytes
    _List_node<T>* _M_node;
    unsigned long size() const
    {
        unsigned long n = 0;
        for (_List_node<T>* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    _List_node<T>* begin() const { return _M_node->_M_next; }
    _List_node<T>* end() const { return _M_node; }
};
class CNavigatorAStar {
public:
    unsigned char      _pad00[0x8c];
    list<CNavWayPoint> Route;                      // +0x8c
    void SmoothCorners();
};

void CNavigatorAStar::SmoothCorners()
{
    if (Route.size() < 3)
        return;
    _List_node<CNavWayPoint>* p0 = Route.begin();
    _List_node<CNavWayPoint>* p1 = p0->_M_next;
    _List_node<CNavWayPoint>* p2 = p1->_M_next;
    _List_node<CNavWayPoint>* p3 = p2->_M_next;
    while (p3 != Route.end()) {
        C2DVector in = p1->_M_data.Position - p0->_M_data.Position;
        C2DVector out = p2->_M_data.Position - p1->_M_data.Position;
        in.Normalise();
        out.Normalise();
        p3 = p3->_M_next;
        p2 = p3->_M_prev;
        p1 = p2->_M_prev;
        p0 = p1->_M_prev;
    }
    C2DVector in = p1->_M_data.Position - p0->_M_data.Position;
    C2DVector out = p2->_M_data.Position - p1->_M_data.Position;
    in.Normalise();
    out.Normalise();
}

C2DVector& C2DVector::Normalise() { if (g_calls < 32) { g_seen[g_calls][0] = X; g_seen[g_calls][1] = Y; } ++g_calls; return *this; }
typedef _List_node<CNavWayPoint> Node;
static void Link(Node* header, Node* nodes, int n)
{
    Node* prev = header;
    for (int i = 0; i < n; ++i) { prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = header; header->_M_prev = prev;
}
int main()
{
    static CNavigatorAStar nav;
    Node header, nodes[4];
    for (int i = 0; i < 4; ++i) { nodes[i]._M_data.Position.X = (float)(i * i); nodes[i]._M_data.Position.Y = (float)(10 * i); }
    nav.Route._M_node = &header;
    Link(&header, nodes, 2);
    nav.SmoothCorners();
    bool ok = g_calls == 0;
    Link(&header, nodes, 3);       // one corner, no loop iteration: legs (1,10) and (3,10)
    nav.SmoothCorners();
    ok = ok && g_calls == 2 && g_seen[0][0] == 1.0f && g_seen[0][1] == 10.0f && g_seen[1][0] == 3.0f && g_seen[1][1] == 10.0f;
    g_calls = 0;
    Link(&header, nodes, 4);       // loop corner (0,1,2) then final corner (1,2,3): legs (1,10),(3,10),(3,10),(5,10)
    nav.SmoothCorners();
    ok = ok && g_calls == 4 && g_seen[2][0] == 3.0f && g_seen[3][0] == 5.0f && g_seen[3][1] == 10.0f;
    if (!ok) { std::printf("00a1be10_TEST FAIL\n"); return 1; }
    std::printf("00a1be10_TEST PASS\n");
    return 0;
}