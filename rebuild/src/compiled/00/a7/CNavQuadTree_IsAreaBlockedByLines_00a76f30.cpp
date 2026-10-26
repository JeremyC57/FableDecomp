// CNavQuadTree::IsAreaBlockedByLines(const C2DBoxF& area, const std::vector<const std::list<C2DLineF>*>& lineLists) const -- true when any navigation line of any list touches the area: a fuzzy bounding-range reject on X and Y, then either end point inside the box or the segment crossing it
// Leaf: ?IsAreaBlockedByLines@ (Ego_r.pdb ?IsAreaBlockedByLines@CNavQuadTree@@ABE_NABVC2DBoxF@@ABV?$vector@PBV?$list@VC2DLineF@@...@std@@@Z; modelled through CNavQuadTree_Methods)
// Retail: Fable.exe @ 0x00A76F30, 371 bytes, `ret 8`, 8-aligned frame for the double temporaries.
// Body: for each list, for each line: skip when GetHighestX() - area.TLX <= -eps, GetLowestX() - area.BRX >= eps,
// GetHighestY() - area.TLY <= -eps or GetLowestY() - area.BRY >= eps (eps = the float at 0x0129BA3C); else
// area.ContainsPoint(Point1) / ContainsPoint(Point2) (0x00535CD0) or IntersectsWith(area) (0x00A57BD0) -> true.
// Layout: engine/CNavQuadTree.h for `this` (unused); C2DBoxF {TLX, TLY, BRX, BRY} and C2DLineF {Point1, Point2} from
// struct_layouts_egor.tsv; STLport list (node {_M_next, _M_prev, data}, list = {_M_node}) and vector {start, finish}.
// Shape: GetHighestX/GetLowestX/... are the landed C2DLineF bodies (0x00A56B80..0x00A56C10: (float)Max<double> /
// Min<double> over const-reference templates on double temporaries), inlined here; a `const C2DLineF& line = *it`
// reference local (retail forms &Point2 from &line, `lea eax,[esi+8]`).
// Opt: /O2 (16-aligned entry).
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