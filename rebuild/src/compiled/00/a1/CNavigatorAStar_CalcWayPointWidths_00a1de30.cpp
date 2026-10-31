// CNavigatorAStar::CalcWayPointWidths(const C3DVector& position) -- measure the navigable widths of the first two route legs (position -> first way point, first -> second) into the steering agent's four best-distance slots, then set its ProximityForArrival to the narrowest less the navigator radius, raised to CurrentMaxSpeed (CurrentMaxSpeed alone when the route has one point)
// Leaf: ?CalcWayPointWidths@CNavigatorAStar@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A1DE30, 298 bytes, `ret 4`.
// Identity: bound ?CalcWayPointWidths@CNavigatorAStar@@QAEXABVC3DVector@@@Z by the landed caller InitialiseFromPreCalculatedRoute
//   0x00A1E370 (passes its `const C3DVector& from`); six call sites (xref). Not in the PDB name sets (inlined in Ego_r).
// Callees: CNavigatorAStar::CalculateNavigableWidthOfLine(const CNavWayPoint&, const CNavWayPoint&, float&, float&, bool) 0x00A1D6E0
//   (Ego_r.pdb ?CalculateNavigableWidthOfLine@CNavigatorAStar@@QAEXABVCNavWayPoint@@0AAM1_N@Z, bound the same);
//   CNavigatorExternalInterface virtual slot 6 (+0x18) PeekNavigatorRadius(bool) const (CGroundNavigatorExternalInterface vtable
//   0x0125EC9C slot 6 = 0x006CDA10, landed; slots 0..5 named from the same vtable's landed bodies).
// Layout: CNavigatorBase from engine/CNavigatorBase.h (SteeringAgent +0x20; the header flattens the vptr, so it is included
//   under the layout alias and CNavigatorAStar models only Route +0x8c, STLport list<CNavWayPoint>, as the landed caller).
//   CSteeringAgent (no engine header) is Ego_r.pdb with retail's one-word STLport lists, as the landed
//   IsCurrentNavigabilityBetter 0x00A1A1A0: CurrentMaxSpeed +0x18, BestDistLeft..BestNextDistRight +0x40..+0x4c,
//   ProximityForArrival +0x50.
// Shape (what matched): the Ego_d.pdb inline accessors of CSteeringAgent -- GetBestDistLeft/Right, GetBestNextDistLeft/Right
//   (float&), GetRadius(bool) const (forwards to NavigatorInterface->PeekNavigatorRadius), GetCurrentMaxSpeed() const,
//   SetProximityForArrival(float). Through GetRadius the agent is loaded into eax and the interface into ecx (`mov eax,[esi+20h]
//   ; mov ecx,[eax]`); a direct SteeringAgent->NavigatorInterface->PeekNavigatorRadius() loads both into ecx. With the PDB's
//   reference parameters of CalculateNavigableWidthOfLine (respelled from pointers) the `pop edi` lands before the final fstp.
//   The four width references are bound before the first call (retail keeps the +0x48/+0x4c addresses in frame words); the
//   narrowest is a float local copied as integers between the compares.
#define CNavigatorBase CNavigatorBaseLayout
#include "engine/CNavigatorBase.h"
#undef CNavigatorBase
class C3DVector { public: float X, Y, Z; };
class C2DVector { public:
    float X, Y;
    C2DVector(const C2DVector& o) : X(o.X), Y(o.Y) { }
    C2DVector(const C3DVector& v) : X(v.X), Y(v.Y) { }
};
class CNavWayPoint { public:                       // 10 bytes
    C2DVector     Position;
    unsigned char NavLayer;
    bool          Removable;
    CNavWayPoint(const C2DVector& position, unsigned char layer, bool removable) : Position(position), NavLayer(layer), Removable(removable) { }
};
struct SWayPointNode {                             // _List_node<CNavWayPoint>
    SWayPointNode* _M_next;
    SWayPointNode* _M_prev;
    CNavWayPoint   _M_data;                        // +0x08
};
inline void __distance(SWayPointNode* first, SWayPointNode* last, unsigned long& n)
{
    while (first != last) {
        first = first->_M_next;
        ++n;
    }
}
struct CWayPointList {                             // STLport list<CNavWayPoint>
    SWayPointNode* _M_node;
    CNavWayPoint& front() { return _M_node->_M_next->_M_data; }
    unsigned long size() const
    {
        unsigned long n = 0;
        __distance(_M_node->_M_next, _M_node, n);
        return n;
    }
};
class CNavigatorExternalInterface { public:
    virtual bool IsMovementBlockedAt(const C3DVector&, const C3DVector&, bool&, bool&) const = 0;   // slot 0
    virtual float GetTerrainDesirabilityAt(const C3DVector&) const = 0;                            // slot 1
    virtual float GetTerrainHeightAt(const C3DVector&) const = 0;                                  // slot 2
    virtual float GetThemeHeightAt(const C3DVector&) const = 0;                                    // slot 3
    virtual float FindPosZAt(const C3DVector&, bool) const = 0;                                    // slot 4
    virtual float FindPosZAtIncludingThemes(const C3DVector&, bool) const = 0;                     // slot 5
    virtual float PeekNavigatorRadius(bool physical) const = 0;                                    // slot 6 (+0x18)
};
struct CRetailList { void* _M_node; };             // STLport list: one word
class CSteeringAgent { public:
    CNavigatorExternalInterface* NavigatorInterface;          // +0x00
    bool                         DummyObstacle;               // +0x04
    float                        RepulsionMultiplier;         // +0x08
    CRetailList                  SteeringBehaviours;          // +0x0C
    CRetailList                  SteeringBehaviourInputs;     // +0x10
    CRetailList                  SteeringBehaviourConstraints; // +0x14
    float                        CurrentMaxSpeed;             // +0x18
    float                        CurrentSpeed;                // +0x1C
    C2DVector                    Goal;                        // +0x20
    C2DVector                    NextGoal;                    // +0x28
    int                          TimeToReacquireGoal;         // +0x30
    float                        DistCutoff;                  // +0x34
    C2DVector                    Steer;                       // +0x38
    float                        BestDistLeft;                // +0x40
    float                        BestDistRight;               // +0x44
    float                        BestNextDistLeft;            // +0x48
    float                        BestNextDistRight;           // +0x4C
    float                        ProximityForArrival;         // +0x50
    // Ego_d.pdb inline accessors
    float GetRadius(bool physical) const { return NavigatorInterface->PeekNavigatorRadius(physical); }
    float& GetBestDistLeft() { return BestDistLeft; }
    float& GetBestDistRight() { return BestDistRight; }
    float& GetBestNextDistLeft() { return BestNextDistLeft; }
    float& GetBestNextDistRight() { return BestNextDistRight; }
    float GetCurrentMaxSpeed() const { return CurrentMaxSpeed; }
    void SetProximityForArrival(float proximity) { ProximityForArrival = proximity; }
};

class CNavigatorAStar : public CNavigatorBaseLayout { public:
    unsigned char _pad24[0x8c - 0x24];
    CWayPointList Route;                           // +0x8c
    void CalculateNavigableWidthOfLine(const CNavWayPoint& from, const CNavWayPoint& to, float& widthLeft, float& widthRight, bool flag);   // 0x00A1D6E0
    void CalcWayPointWidths(const C3DVector& position);
};

void CNavigatorAStar::CalcWayPointWidths(const C3DVector& position)
{
    CNavWayPoint start(position, Route.front().NavLayer, true);
    CNavWayPoint* first = &Route.front();
    CNavWayPoint* second = Route.size() > 1 ? &Route._M_node->_M_next->_M_next->_M_data : 0;
    float& bestDistLeft = SteeringAgent->GetBestDistLeft();
    float& bestDistRight = SteeringAgent->GetBestDistRight();
    float& bestNextDistLeft = SteeringAgent->GetBestNextDistLeft();
    float& bestNextDistRight = SteeringAgent->GetBestNextDistRight();
    CalculateNavigableWidthOfLine(start, *first, bestDistLeft, bestDistRight, false);
    if (second) {
        CalculateNavigableWidthOfLine(*first, *second, bestNextDistLeft, bestNextDistRight, false);
        float narrowest = bestDistLeft;
        if (bestDistRight < narrowest)
            narrowest = bestDistRight;
        if (bestNextDistLeft < narrowest)
            narrowest = bestNextDistLeft;
        if (bestNextDistRight < narrowest)
            narrowest = bestNextDistRight;
        float width = narrowest - SteeringAgent->GetRadius(false);
        SteeringAgent->SetProximityForArrival(width < SteeringAgent->GetCurrentMaxSpeed() ? SteeringAgent->GetCurrentMaxSpeed() : width);
    } else
        SteeringAgent->SetProximityForArrival(SteeringAgent->GetCurrentMaxSpeed());
}