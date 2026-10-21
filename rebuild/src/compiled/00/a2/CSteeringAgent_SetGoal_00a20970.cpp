// CSteeringAgent::SetGoal(const C2DVector& goal, const C2DVector& nextGoal) -- when the goal moves by more than 1e-4 on either axis, take the new goal and next goal, restart the best-approach tracking from the current distance and clear the per-section flags
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A20970, 144 bytes; `ret 8`. Callees: CNavigatorExternalInterface virtual slot 8 (+0x20),
//          the navigator's current 2D position returned by value. Constant: 1e-4f @ 0x0129BA3C.
// Layout: Ego_r.pdb CSteeringAgent with retail's one-word lists (everything after them 0x18 lower, as the landed
//         IsCurrentNavigabilityBetter 0x00A1A1A0): Goal +0x20, NextGoal +0x28, TimeSinceBestApproach +0x54, BestApproach +0x58,
//         AvoidDynamicObstaclesUntilNextWaypoint +0x61, HasWanderedDuringThisSection +0x62, FailPath +0x63.
// Shape: `fcomp ; test ah,41h ; jp` = `!(fabs(d) <= eps)`; the vectors copy as dwords; the distance is an inline length.
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(fabs, sqrt)
struct C2DVector {
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& o) const { return C2DVector(X - o.X, Y - o.Y); }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
};
struct CNavigatorExternalInterface {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual C2DVector GetPosition2D() const;       // slot 8 (+0x20)
};
struct CRetailList { void* m_pHead; };            // retail list: one word
struct CSteeringAgent {
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
    int                          TimeSinceBestApproach;       // +0x54
    float                        BestApproach;                // +0x58
    bool                         Wandering;                   // +0x5C
    bool                         MovingForwards;              // +0x5D
    bool                         RecalcWidth;                 // +0x5E
    bool                         AddWaypoint;                 // +0x5F
    bool                         AvoidDynamicObstacles;       // +0x60
    bool                         AvoidDynamicObstaclesUntilNextWaypoint; // +0x61
    bool                         HasWanderedDuringThisSection; // +0x62
    bool                         FailPath;                    // +0x63
    bool                         OnDirectApproach;            // +0x64
    void SetGoal(const C2DVector& goal, const C2DVector& nextGoal);
};

void CSteeringAgent::SetGoal(const C2DVector& goal, const C2DVector& nextGoal)
{
    if (!((float)fabs(Goal.X - goal.X) <= 0.0001f) || !((float)fabs(Goal.Y - goal.Y) <= 0.0001f)) {
        Goal = goal;
        NextGoal = nextGoal;
        C2DVector position = NavigatorInterface->GetPosition2D();
        C2DVector delta = position - Goal;
        TimeSinceBestApproach = 0;
        HasWanderedDuringThisSection = false;
        AvoidDynamicObstaclesUntilNextWaypoint = true;
        FailPath = false;
        double length = delta.GetLength();
        BestApproach = (float)length;
    }
}