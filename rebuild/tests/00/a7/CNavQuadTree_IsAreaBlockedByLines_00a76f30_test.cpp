#include <cstdio>
#include "engine/CNavQuadTree.h"

template <class T>
inline const T& Max(const T& left, const T& right)
{
    return left < right ? right : left;
}

template <class T>
inline const T& Min(const T& left, const T& right)
{
    return right < left ? right : left;
}

class C2DVector { public:
    float X;
    float Y;
};
class C2DBoxF { public:
    float TLX;
    float TLY;
    float BRX;
    float BRY;
    bool ContainsPoint(const C2DVector& point) const;          // 0x00535CD0
};
class C2DLineF { public:
    C2DVector Point1;                                         // +0x00
    C2DVector Point2;                                         // +0x08
    float GetLowestX() const { return (float)Min<double>(Point1.X, Point2.X); }
    float GetHighestX() const { return (float)Max<double>(Point1.X, Point2.X); }
    float GetLowestY() const { return (float)Min<double>(Point1.Y, Point2.Y); }
    float GetHighestY() const { return (float)Max<double>(Point1.Y, Point2.Y); }
    bool IntersectsWith(const C2DBoxF& box) const;            // 0x00A57BD0
};
extern float g_C2DTolerance;                                  // 0x0129BA3C

namespace std {
template <class T> struct _List_node {
    _List_node* _M_next;
    _List_node* _M_prev;
    T _M_data;
};
template <class T> class list {
public:
    _List_node<T>* _M_node;
    class const_iterator {
    public:
        const _List_node<T>* _M_node;
        const_iterator(const _List_node<T>* node) : _M_node(node) {}
        const T& operator*() const { return _M_node->_M_data; }
        const T* operator->() const { return &_M_node->_M_data; }
        const_iterator& operator++() { _M_node = _M_node->_M_next; return *this; }
        bool operator!=(const const_iterator& other) const { return _M_node != other._M_node; }
    };
    const_iterator begin() const { return const_iterator(_M_node->_M_next); }
    const_iterator end() const { return const_iterator(_M_node); }
};
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    typedef const T* const_iterator;
    const_iterator begin() const { return _M_start; }
    const_iterator end() const { return _M_finish; }
};
}

struct CNavQuadTree_Methods : CNavQuadTree {
    bool IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const;
};

bool CNavQuadTree_Methods::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const
{
    for (std::vector<const std::list<C2DLineF>*>::const_iterator it = lineLists.begin(); it != lineLists.end(); ++it) {
        const std::list<C2DLineF>* lines = *it;
        for (std::list<C2DLineF>::const_iterator it2 = lines->begin(); it2 != lines->end(); ++it2) {
            const C2DLineF& line = *it2;
            if (line.GetHighestX() - area.TLX <= -g_C2DTolerance)
                continue;
            if (line.GetLowestX() - area.BRX >= g_C2DTolerance)
                continue;
            if (line.GetHighestY() - area.TLY <= -g_C2DTolerance)
                continue;
            if (line.GetLowestY() - area.BRY >= g_C2DTolerance)
                continue;
            if (area.ContainsPoint(line.Point1) || area.ContainsPoint(line.Point2) || line.IntersectsWith(area))
                return true;
        }
    }
    return false;
}

static int g_fail = 0;
static void check(bool c, const char* w) { if (!c) { std::printf("FAIL %s\n", w); g_fail = 1; } }
float g_C2DTolerance = 0.01f;
static int g_contains, g_intersects; static bool g_intersectResult; static const C2DLineF* g_lastIntersect;
bool C2DBoxF::ContainsPoint(const C2DVector& p) const { ++g_contains; return p.X >= TLX && p.X <= BRX && p.Y >= TLY && p.Y <= BRY; }
bool C2DLineF::IntersectsWith(const C2DBoxF&) const { ++g_intersects; g_lastIntersect = this; return g_intersectResult; }
typedef std::_List_node<C2DLineF> Node;
static void MakeList(std::list<C2DLineF>& l, Node& head, Node* nodes, int n)
{
    l._M_node = &head;
    Node* prev = &head;
    for (int i = 0; i < n; ++i) { prev->_M_next = &nodes[i]; nodes[i]._M_prev = prev; prev = &nodes[i]; }
    prev->_M_next = &head; head._M_prev = prev;
}
static void SetLine(Node& n, float x1, float y1, float x2, float y2) { n._M_data.Point1.X = x1; n._M_data.Point1.Y = y1; n._M_data.Point2.X = x2; n._M_data.Point2.Y = y2; }
static bool Run(const std::list<C2DLineF>* a, const std::list<C2DLineF>* b, const C2DBoxF& box)
{
    static CNavQuadTree_Methods tree;
    const std::list<C2DLineF>* arr[2] = { a, b };
    std::vector<const std::list<C2DLineF>*> v; v._M_start = arr; v._M_finish = arr + (b ? 2 : (a ? 1 : 0)); v._M_end_of_storage = v._M_finish;
    g_contains = g_intersects = 0;
    return tree.IsAreaBlockedByLines(box, v);
}
int main()
{
    C2DBoxF box; box.TLX = 0; box.TLY = 0; box.BRX = 10; box.BRY = 10;
    Node h1, h2, n1[4], n2[2]; std::list<C2DLineF> l1, l2;
    check(!Run(0, 0, box), "no lists");
    MakeList(l1, h1, n1, 0);
    check(!Run(&l1, 0, box) && g_contains == 0, "empty list");
    // the four range rejects (each line also lies wholly outside, so a reject is the only way to skip the box tests)
    SetLine(n1[0], -5, 1, -1, 2);          // highest X left of the box
    SetLine(n1[1], 11, 1, 15, 2);          // lowest X right of the box
    SetLine(n1[2], 1, -5, 2, -1);          // highest Y above
    SetLine(n1[3], 1, 11, 2, 15);          // lowest Y below
    MakeList(l1, h1, n1, 4); g_intersectResult = true;
    check(!Run(&l1, 0, box) && g_contains == 0 && g_intersects == 0, "range rejects");
    // reversed end points reject the same way (Max/Min pick either end)
    SetLine(n1[0], -1, 2, -5, 1); SetLine(n1[1], 15, 2, 11, 1); SetLine(n1[2], 2, -1, 1, -5); SetLine(n1[3], 2, 15, 1, 11);
    check(!Run(&l1, 0, box) && g_contains == 0, "reversed rejects");
    // within tolerance of the edge is not rejected: the touching line reaches the box tests
    SetLine(n1[0], -5, 1, -0.005f, 2); MakeList(l1, h1, n1, 1); g_intersectResult = false;
    check(!Run(&l1, 0, box) && g_contains == 2 && g_intersects == 1 && g_lastIntersect == &n1[0]._M_data, "tolerance edge");
    // point 1 inside
    SetLine(n1[0], 5, 5, 20, 20); check(Run(&l1, 0, box) && g_contains == 1 && g_intersects == 0, "point1 inside");
    // point 2 inside
    SetLine(n1[0], 20, 20, 5, 5); check(Run(&l1, 0, box) && g_contains == 2 && g_intersects == 0, "point2 inside");
    // crossing segment, found in the second list after a miss in the first
    SetLine(n1[0], -5, 5, -2, 5);          // rejected
    SetLine(n2[0], -5, 20, 20, 20);        // spans in X, rejected in Y
    SetLine(n2[1], -5, 5, 15, 5);          // crosses
    MakeList(l2, h2, n2, 2); g_intersectResult = true;
    check(Run(&l1, &l2, box) && g_intersects == 1 && g_lastIntersect == &n2[1]._M_data && g_contains == 2, "crossing in second list");
    g_intersectResult = false;
    check(!Run(&l1, &l2, box) && g_intersects == 1, "no crossing");
    if (!g_fail) std::printf("00a76f30_TEST PASS\n");
    return g_fail;
}