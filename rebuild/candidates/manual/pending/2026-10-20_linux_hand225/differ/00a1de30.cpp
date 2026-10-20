// CNavigatorAStar::CalcWayPointWidths(const C2DVector& position) -- measure the navigable widths of the first two route legs (position -> first way point, first -> second) into the owner's four width slots, then set the owner's route width to the narrowest less the owner shape's radius, clamped to the owner's minimum (the minimum alone when the route has one point)
// Grade: DIFFER(298v298) -- owner reloaded into ecx (retail eax) for the shape's virtual call; retail schedules `pop edi` before the final `fstp [esi+50h]`
// Leaf: ?CalcWayPointWidths@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1DE30, 298 bytes (+ int3 padding), `ret 4`.
// Callees: CNavigatorAStar::CalculateNavigableWidthOfLine(CNavWayPoint*, CNavWayPoint*, float*, float*, bool) 0x00A1D6E0,
//   the owner shape's virtual slot 6 (+0x18, float(bool)).
// Layout (retail, as 0x00A1D5D0): Route +0x8c (STLport list, way point at node +8, NavLayer node +0x10); the navigator's
//   owner record at +0x20: shape pointer +0, minimum width +0x18, leg widths +0x40/+0x44/+0x48/+0x4c, route width +0x50.
// Shape: the way point takes its position by value (the copy goes through x87: fld/fld/fxch/fstp/fstp); the four width slots bound as references before the calls (retail re-reads them through the saved pointers);
//   the owner re-read for the shape call and the clamp; the narrowest kept in a stack float (copied as integers).
struct C2DVector {
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
};
struct CNavWayPoint {                              // 10 bytes
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
    CNavWayPoint(C2DVector position, unsigned char layer, bool removable) : Position(position), NavLayer(layer), Removable(removable) { }
};
struct SWayPointNode {                             // _List_node<CNavWayPoint>
    SWayPointNode* _M_next;
    SWayPointNode* _M_prev;
    CNavWayPoint   Data;                           // +0x08
};
inline void __distance(SWayPointNode* first, SWayPointNode* last, unsigned long& n)
{
    while (first != last) {
        first = first->_M_next;
        ++n;
    }
}
struct CWayPointList {                             // STLport list<CNavWayPoint> (retail: the sentinel pointer)
    SWayPointNode* _M_node;
    CNavWayPoint& front() { return _M_node->_M_next->Data; }
    unsigned long size() const
    {
        unsigned long n = 0;
        __distance(_M_node->_M_next, _M_node, n);
        return n;
    }
};
struct CNavShape {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual float GetRadius(bool flag);            // slot 6 (+0x18)
};
struct SNavOwner {
    CNavShape*    Shape;                           // +0x00
    unsigned char _pad04[0x18 - 0x04];
    float         MinWidth;                        // +0x18
    unsigned char _pad1c[0x40 - 0x1c];
    float         StartWidthA;                     // +0x40
    float         StartWidthB;                     // +0x44
    float         NextWidthA;                      // +0x48
    float         NextWidthB;                      // +0x4c
    float         RouteWidth;                      // +0x50
};

struct CNavigatorAStar {
    virtual void v00();
    unsigned char _pad04[0x20 - 0x04];
    SNavOwner*    Owner;                           // +0x20
    unsigned char _pad24[0x8c - 0x24];
    CWayPointList Route;                           // +0x8c
    void CalculateNavigableWidthOfLine(CNavWayPoint* from, CNavWayPoint* to, float* widthA, float* widthB, bool flag);   // 0x00A1D6E0
    void CalcWayPointWidths(const C2DVector& position);
};

void CNavigatorAStar::CalcWayPointWidths(const C2DVector& position)
{
    CNavWayPoint start(position, Route.front().NavLayer, true);
    CNavWayPoint* first = &Route.front();
    CNavWayPoint* second = Route.size() > 1 ? &Route._M_node->_M_next->_M_next->Data : 0;
    float& startA = Owner->StartWidthA;
    float& startB = Owner->StartWidthB;
    float& nextA = Owner->NextWidthA;
    float& nextB = Owner->NextWidthB;
    CalculateNavigableWidthOfLine(&start, first, &startA, &startB, false);
    if (second) {
        CalculateNavigableWidthOfLine(first, second, &nextA, &nextB, false);
        float narrowest = startA;
        if (startB < narrowest)
            narrowest = startB;
        if (nextA < narrowest)
            narrowest = nextA;
        if (nextB < narrowest)
            narrowest = nextB;
        float width = narrowest - Owner->Shape->GetRadius(false);
        Owner->RouteWidth = width < Owner->MinWidth ? Owner->MinWidth : width;
    } else
        Owner->RouteWidth = Owner->MinWidth;
}
