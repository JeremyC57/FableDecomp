// CNavigatorAStar::SmoothCorners() -- for a route of three or more way points, walk every corner (three consecutive points) and take the unit directions of its two legs; the turn itself is not applied in this build, so only the normalisation work remains
// Leaf: SmoothCorners
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1BE10, 208 bytes. __fastcall, `ret`; an 8-aligned frame (`and esp,-8`).
// Callees: C2DVector::Normalise() 0x00A14540 (landed) on each leg.
// Layout: Ego_r.pdb CNavigatorAStar with STLport containers: Route (list<CNavWayPoint>; node next +0, prev +4, way point +8 with
//   its Position first) at retail +0x8c.
// Shape: size() is the inline node-counting walk (`jb` on 3); four node pointers ride the list (the lead one advanced, the
//   other three re-derived through prev links); the legs come from the 2-float C2DVector constructor (Y computed first, as
//   arguments are evaluated right to left); the final corner is handled after the loop with its own pair of locals. VC
//   aligns the frame to 8 for the four 8-byte C2DVector locals.
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    C2DVector& Normalise();                        // 0x00A14540
};
struct CNavWayPoint {                              // 10 bytes
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