#include <cstdio>
// Behaviour test for 00a1b4e0 CNavigatorAStar::GetSpeedReducedForStartAndEnd: one way point left -> 35% inside speed+dist,
// 65% inside 3*speed+dist, else the speed with SlowedDownForLastPoint cleared; any other route length keeps the speed too.
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

int main()
{
    static CNavigatorAStar nav;
    _List_node<CNavWayPoint> header, wp, wp2;
    wp._M_data.Position.X = 10.0f; wp._M_data.Position.Y = 0.0f;
    header._M_next = &wp; wp._M_next = &header; header._M_prev = &wp; wp._M_prev = &header;
    nav.Route._M_node = &header;
    C2DVector near(8.0f, 0.0f), mid(4.0f, 0.0f), far(-20.0f, 0.0f);
    bool ok = true;
    nav.SlowedDownForLastPoint = true;
    float r = nav.GetSpeedReducedForStartAndEnd(2.0f, 1.0f, near);   // dist 2 < 3 -> 35%
    float want = 0.35f * 2.0f; ok = ok && r == want && nav.SlowedDownForLastPoint;
    r = nav.GetSpeedReducedForStartAndEnd(2.0f, 1.0f, mid);           // dist 6: 3 < 6 < 7 -> 65%
    want = 0.65f * 2.0f; ok = ok && r == want && nav.SlowedDownForLastPoint;
    r = nav.GetSpeedReducedForStartAndEnd(2.0f, 1.0f, far);           // dist 30 -> unchanged, flag cleared
    ok = ok && r == 2.0f && !nav.SlowedDownForLastPoint;
    // two way points: unchanged
    wp._M_next = &wp2; wp2._M_prev = &wp; wp2._M_next = &header; header._M_prev = &wp2;
    nav.SlowedDownForLastPoint = true;
    r = nav.GetSpeedReducedForStartAndEnd(2.0f, 1.0f, near);
    ok = ok && r == 2.0f && !nav.SlowedDownForLastPoint;
    if (!ok) { std::printf("00a1b4e0_TEST FAIL\n"); return 1; }
    std::printf("00a1b4e0_TEST PASS\n");
    return 0;
}
