// Grade: DIFFER(596v601) -- esi/edi swapped (retail esi=pos, edi=this) with the push esi/push edi prologue interleave; delta-block scheduling (push ebx; mov bl after the three flds); CalculateNavigableWidthOfLine temporary loads pos.Y before pos.X and stores its bytes before the pushes; the SetGoal(front, front) references CSE'd into one register (retail computes two). Advisor C1 best (p8: delta copy + -=, delta *= 0.5f in place, s = Y*Y; s += X*X).
// CNavigatorAStar::MoveTowardNextWayPoint(const C3DVector& pos, float speed, float endDistance, C3DVector& move, bool noSlowDown, bool avoidDynamicObstacles, bool, bool slowForEnd) -- run the steering agent one step (speed limited near the route end), turn its steer into a move with the ground-height correction, and when the agent asks for an extra way point insert the midpoint to the next way point if the line to it is wide enough
// Leaf: ?MoveTowardNextWayPoint@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1E110, 608 bytes (`ret 20h`).
// Callees: GetSpeedReducedForStartAndEnd 0x00A1B4E0, CSteeringAgent::Process 0x00A1AFF0, the interface's slot 4
//   GetGroundHeight (+0x10), CalcWayPointWidths 0x00A1DE30, the agent interface's slot 6 GetRadius (+0x18),
//   list<CNavWayPoint>::push_front 0x00A21FC0, CalculateNavigableWidthOfLine 0x00A1D6E0, list::size 0x006CD240 (out of
//   line here), CSteeringAgent::SetGoal 0x00A20970, free 0x00BFEA14; the height-test offset 0x013D27D0, 8.0f 0x0125916C,
//   0.5f 0x0122F59C.
// Layout (retail): CNavigatorAStar as 0x00A1E370 (Route +0x8c); CSteeringAgent (Ego_r.pdb less 8 bytes per STLport list):
//   NavigatorInterface +0, CurrentMaxSpeed +0x18, Steer +0x38, BestDistLeft +0x40, BestDistRight +0x44, RecalcWidth +0x5e,
//   AddWaypoint +0x5f, AvoidDynamicObstacles +0x60.
#include <math.h>
#include <stdlib.h>
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
struct C3DVector;
struct C2DVector {
    float X, Y;
    C2DVector() {}
    C2DVector(float x, float y) : X(x), Y(y) {}
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) {}
    C2DVector(const C3DVector& v);
    C2DVector& operator-=(const C2DVector& o) { X -= o.X; Y -= o.Y; return *this; }
    C2DVector& operator+=(const C2DVector& o) { X += o.X; Y += o.Y; return *this; }
    C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
    float GetLength() const { float s = Y * Y; s += X * X; return (float)sqrt(s); }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
};
inline C2DVector::C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) {}
#pragma pack(push, 1)
struct CNavWayPoint {                              // 10 bytes
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
    CNavWayPoint(const C2DVector& p, unsigned char layer) : Position(p), NavLayer(layer), Removable(true) {}
};
#pragma pack(pop)
struct SWayPointNode {                             // _List_node<CNavWayPoint>
    SWayPointNode* _M_next;
    SWayPointNode* _M_prev;
    CNavWayPoint   _M_data;
};
struct CWayPointList {                             // STLport list<CNavWayPoint>
    SWayPointNode* _M_node;
    CNavWayPoint& front() { return _M_node->_M_next->_M_data; }
    unsigned long size() const;                    // 0x006CD240 (not inlined here)
    void push_front(const CNavWayPoint& x);        // 0x00A21FC0
    void pop_front()
    {
        SWayPointNode* node = _M_node->_M_next;
        SWayPointNode* next = node->_M_next;
        SWayPointNode* prev = node->_M_prev;
        prev->_M_next = next;
        next->_M_prev = prev;
        free(node);
    }
};
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual float GetGroundHeight(const C3DVector& position, bool exact);   // slot 4 (+0x10)
    virtual void v05();
    virtual float GetRadius(bool b);                                         // slot 6 (+0x18)
};
struct CSteeringAgent {
    CNavigatorExternalInterface* NavigatorInterface;   // +0x00
    unsigned char _pad04[0x14];
    float         CurrentMaxSpeed;                     // +0x18
    unsigned char _pad1c[0x1c];
    C2DVector     Steer;                               // +0x38
    float         BestDistLeft;                        // +0x40
    float         BestDistRight;                       // +0x44
    unsigned char _pad48[0x16];
    bool          RecalcWidth;                         // +0x5e
    bool          AddWaypoint;                         // +0x5f
    bool          AvoidDynamicObstacles;               // +0x60
    void Process();                                    // 0x00A1AFF0
    void SetGoal(const CNavWayPoint& goal, const CNavWayPoint& next);   // 0x00A20970
};
extern C3DVector g_NavHeightTestOffset;                // 0x013D27D0
struct CNavigatorAStar {
    // +0x00 vptr
    unsigned char                _pad04[0x10];
    CNavigatorExternalInterface* PInterface;           // +0x14
    unsigned char                _pad18[0x8];
    CSteeringAgent*              SteeringAgent;        // +0x20
    unsigned char                _pad24[0x68];
    CWayPointList                Route;                // +0x8c
    virtual void v00();
    float GetSpeedReducedForStartAndEnd(float speed, float endDistance, const C2DVector& pos);   // 0x00A1B4E0
    void CalcWayPointWidths(const C3DVector& pos);     // 0x00A1DE30
    void CalculateNavigableWidthOfLine(const CNavWayPoint& from, const CNavWayPoint& to, float& left, float& right, bool b);   // 0x00A1D6E0
    void MoveTowardNextWayPoint(const C3DVector& pos, float speed, float endDistance, C3DVector& move, bool noSlowDown,
                                bool avoidDynamicObstacles, bool, bool slowForEnd);
};

void CNavigatorAStar::MoveTowardNextWayPoint(const C3DVector& pos, float speed, float endDistance, C3DVector& move, bool noSlowDown,
                                             bool avoidDynamicObstacles, bool, bool slowForEnd)
{
    SteeringAgent->CurrentMaxSpeed = (!noSlowDown && slowForEnd) ? GetSpeedReducedForStartAndEnd(speed, endDistance, C2DVector(pos)) : speed;
    SteeringAgent->AvoidDynamicObstacles = avoidDynamicObstacles;
    SteeringAgent->Process();
    C2DVector steer(SteeringAgent->Steer);
    move.X = steer.X;
    move.Y = steer.Y;
    move.Z = PInterface->GetGroundHeight(pos + move + g_NavHeightTestOffset, true) - pos.Z;
    if (SteeringAgent->RecalcWidth)
        CalcWayPointWidths(pos);
    if (!SteeringAgent->AddWaypoint)
        return;
    CNavWayPoint& next = Route.front();
    unsigned char layer = next.NavLayer;
    C2DVector delta(next.Position);
    delta -= C2DVector(pos);
    if (delta.GetLength() > SteeringAgent->NavigatorInterface->GetRadius(false) * 8.0f) {
        delta *= 0.5f;
        Route.push_front(CNavWayPoint(C2DVector(delta.X + pos.X, delta.Y + pos.Y), layer));
        float left;
        float right;
        CalculateNavigableWidthOfLine(CNavWayPoint(C2DVector(pos), layer), Route.front(), left, right, false);
        if (left > SteeringAgent->BestDistLeft && right > SteeringAgent->BestDistRight) {
            CalcWayPointWidths(pos);
            if (Route.size() > 1)
                SteeringAgent->SetGoal(Route.front(), Route._M_node->_M_next->_M_next->_M_data);
            else
                SteeringAgent->SetGoal(Route.front(), Route.front());
            return;
        }
        Route.pop_front();
    }
}
