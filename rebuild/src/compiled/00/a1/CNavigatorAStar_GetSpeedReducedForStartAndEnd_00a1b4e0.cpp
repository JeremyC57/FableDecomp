// CNavigatorAStar::GetSpeedReducedForStartAndEnd(float, float, const C2DVector&) -- on the last leg of a route (one way point left) slow down near the end: inside speed+distance of it run at 35% speed, inside 3*speed+distance at 65%; otherwise clear SlowedDownForLastPoint and keep the speed
// Leaf: GetSpeedReducedForStartAndEnd
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1B4E0, 188 bytes (+ int3 padding; `ret 0Ch`), float result on the x87 stack.
// Callees: none (the STLport list size() walk is inline). Float pool: 3.0f 0x0122DED4, 0.35f 0x0129CBB0, 0.65f 0x0129CBB4.
// Layout: Ego_r.pdb CNavigatorAStar with STLport containers: Route (list<CNavWayPoint>, a 4-byte header pointer; node data +8,
//   way point Position first) at retail +0x8c (PDB +0x94), SlowedDownForLastPoint at retail +0xbf (PDB +0xcf).
// Shape: size() is the node-counting walk compared with 1; the end offset is a C2DVector local (front().Position - pos)
//   and each test squares the radius after the distance sum (`dist2 < r * r`, test ah,41h).
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetLengthSquared() const { return X * X + Y * Y; }
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
    T& front() { return _M_node->_M_next->_M_data; }
};
class CNavigatorAStar {
public:
    unsigned char      _pad00[0x8c];
    list<CNavWayPoint> Route;                      // +0x8c
    unsigned char      _pad90[0x2f];
    bool               SlowedDownForLastPoint;     // +0xbf
    float GetSpeedReducedForStartAndEnd(float speed, float slowDownDistance, const C2DVector& pos);
};

float CNavigatorAStar::GetSpeedReducedForStartAndEnd(float speed, float slowDownDistance, const C2DVector& pos)
{
    if (Route.size() == 1) {
        C2DVector toEnd = Route.front().Position - pos;
        float outer = 3.0f * speed + slowDownDistance;
        if (toEnd.GetLengthSquared() < outer * outer) {
            C2DVector offset = Route.front().Position - pos;
            float inner = slowDownDistance + speed;
            if (offset.GetLengthSquared() < inner * inner)
                return 0.35f * speed;
            return 0.65f * speed;
        }
    }
    SlowedDownForLastPoint = false;
    return speed;
}