// CSBCCorridor::Process(const CSteeringAgent* agent) -- corridor steering constraint: with steering globally disabled pass the input through; otherwise mark MoveBeforeOrientation and, when the wall list is empty or the agent's goal / BestDistLeft / BestDistRight changed, rebuild the four corridor walls (two side walls at the clamped left/right widths, a back wall at the agent and a front wall past the goal, each with its inward normal); then clip the input velocity against every wall the agent is within two radii of (slide along the wall, push away by at most half the gap) and abort with a zero output and Reset() when the agent is outside a wall
// Grade: DIFFER(1743v1768) 355/525 -- the control flow, the four CWall pushes, the function-local static (guard bit 0x013D2D5C,
//   value 0x013D2D58) and the clip loop match in order; the residue is mostly x87 stack-slot homes (retail puts the
//   position at F+0x18, direction F+0x20, this build at other slots) plus the copy of the nearest-point result into a
//   local (retail copies the hidden-return temp, 25 bytes) and two compare polarities
// Leaf: ?Process@CSBCCorridor@@UAE_NPBVCSteeringAgent@@@Z
// Flags: /O2 /Oy /W3
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A880C0, 1768 bytes (oracle), `ret 4`, 8-byte aligned frame.
// Name: Ego_r.pdb CSBCCorridor {Walls, CurrentEndPoint, BestLeft, BestRight} (retail +0x18..+0x28 per engine/CSBCCorridor.h),
//   CWall {Point1, Point2, ToInternal}, CSB::Output (CKinematic) +0x04, CSBConstraint::MoveBeforeOrientation +0x14 (Ego_r
//   offset 20); CSteeringAgent {NavigatorInterface +0, Goal +0x20, BestDistLeft +0x40, BestDistRight +0x44} from the Ego_r
//   layout with 4-byte retail lists. Unproven: CSBConstraint +0x0c `Input`, the CNavigatorExternalInterface slot names
//   (6 GetRadius(long), 7 GetVelocity(C2DVector&), 8 GetPosition(C2DVector&), 22 the end-scale getter), 0x00A57140
//   C2DLineF::GetNearestPoint, g_SteeringDisabled 0x013D2D60.
// Callees: C2DVector::Normalise 0x00A14540, list<CWall>::clear 0x00A20E10 (landed as CGuiControlTreePane_SortTreeRecursively,
//   a propagated label), list<CWall>::push_back 0x00A88B10, 0x00A57140; literals 0.1f/1.1f/0.0f/0.5f and 1e-4f pooled.
// Levers: navigator getters as out-parameter virtuals (retail reads the output slot, not eax: 406 -> 362); float abs
//   helper for the epsilon test; `!(abs <= eps)` polarity (360); one outer-scope position local (356); `!(dot > 0)`
//   (355). Inert/worse: assigning the nearest point into a pre-declared local (+25 bytes, 408), `!(a < eps && b < eps)`.
extern "C" double __cdecl fabs(double);
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(fabs, sqrt)
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator-() const { return C2DVector(-X, -Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    float operator*(const C2DVector& v) const { return X * v.X + Y * v.Y; }
    float GetLength() const { return (float)sqrt(X * X + Y * Y); }
    void Normalise();                                       // 0x00A14540
};
class C2DLineF { public:
    C2DVector Start;
    C2DVector End;
    C2DLineF(const C2DVector& start, const C2DVector& end) : Start(start), End(end) { }
    C2DVector GetNearestPoint(const C2DVector& point) const;   // 0x00A57140 (name unproven)
};
class CNavigatorExternalInterface { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual float GetRadius(long index) const;              // slot 6 (+0x18)
    virtual void GetVelocity(C2DVector& velocity) const;    // slot 7 (+0x1c)
    virtual void GetPosition(C2DVector& position) const;    // slot 8 (+0x20)
    virtual void v9(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20();
    virtual void v21();
    virtual float GetCorridorEndScale() const;              // slot 22 (+0x58)
};
class CSteeringAgent { public:
    CNavigatorExternalInterface* NavigatorInterface;        // +0x00
    bool DummyObstacle;                                     // +0x04
    float RepulsionMultiplier;                              // +0x08
    void* SteeringBehaviours;                               // +0x0c
    void* SteeringBehaviourInputs;                          // +0x10
    void* SteeringBehaviourConstraints;                     // +0x14
    float CurrentMaxSpeed;                                  // +0x18
    float CurrentSpeed;                                     // +0x1c
    C2DVector Goal;                                         // +0x20
    C2DVector NextGoal;                                     // +0x28
    long TimeToReacquireGoal;                               // +0x30
    float DistCutoff;                                       // +0x34
    C2DVector Steer;                                        // +0x38
    float BestDistLeft;                                     // +0x40
    float BestDistRight;                                    // +0x44
};
class CWall { public:
    C2DVector Point1;
    C2DVector Point2;
    C2DVector ToInternal;
    CWall(const C2DVector& p1, const C2DVector& p2, const C2DVector& toInternal) : Point1(p1), Point2(p2), ToInternal(toInternal) { }
};
struct CWallNode { CWallNode* _M_next; CWallNode* _M_prev; CWall _M_data; };
class CWallList { public:
    CWallNode* _M_node;
    unsigned int size() const
    {
        unsigned int n = 0;
        for (CWallNode* p = _M_node->_M_next; p != _M_node; p = p->_M_next)
            ++n;
        return n;
    }
    void clear();                                           // 0x00A20E10
    void push_back(const CWall& wall);                      // 0x00A88B10
};
__forceinline float FloatAbs(float v) { return (float)fabs(v); }
extern bool g_SteeringDisabled;                             // 0x013D2D60 (name unproven)
class CSB { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3();
    virtual void Reset();                                   // slot 4 (+0x10)
    C2DVector Output;                                       // +0x04 CKinematic
};
class CSBConstraint : public CSB { public:
    C2DVector Input;                                        // +0x0c
    bool MoveBeforeOrientation;                             // +0x14
};
class CSBCCorridor : public CSBConstraint { public:
    CWallList Walls;                                        // +0x18
    C2DVector CurrentEndPoint;                              // +0x1c
    float BestLeft;                                         // +0x24
    float BestRight;                                        // +0x28
    virtual bool Process(const CSteeringAgent* agent);
};

bool CSBCCorridor::Process(const CSteeringAgent* agent)
{
    if (g_SteeringDisabled) {
        Output = Input;
        return true;
    }
    MoveBeforeOrientation = true;
    C2DVector pos;
    if (Walls.size() == 0 || !(FloatAbs(CurrentEndPoint.X - agent->Goal.X) <= 0.0001f) || !(FloatAbs(CurrentEndPoint.Y - agent->Goal.Y) <= 0.0001f)
        || BestLeft != agent->BestDistLeft || BestRight != agent->BestDistRight) {
        CurrentEndPoint = agent->Goal;
        BestLeft = agent->BestDistLeft;
        BestRight = agent->BestDistRight;
        agent->NavigatorInterface->GetPosition(pos);
        C2DVector dir = CurrentEndPoint - pos;
        dir.Normalise();
        C2DVector perp(dir.Y, -dir.X);
        float radius = agent->NavigatorInterface->GetRadius(0);
        C2DVector offset = dir * radius * 0.1f;
        agent->NavigatorInterface->GetPosition(pos);
        C2DVector start = pos - offset;
        static float s_EndScale = agent->NavigatorInterface->GetCorridorEndScale();
        float right = agent->BestDistRight;
        float left = agent->BestDistLeft;
        C2DVector end = CurrentEndPoint + dir * radius * s_EndScale;
        float minWidth = radius * 1.1f;
        if (right < minWidth)
            right = minWidth;
        if (left < minWidth)
            left = minWidth;
        Walls.clear();
        Walls.push_back(CWall(start + perp * right, end + perp * right, -perp));
        Walls.push_back(CWall(start - perp * left, end - perp * left, perp));
        Walls.push_back(CWall(start - perp * left, start + perp * right, dir));
        Walls.push_back(CWall(end - perp * left, end + perp * right, -dir));
    }
    C2DVector result = Input;
    for (CWallNode* node = Walls._M_node->_M_next; node != Walls._M_node; node = node->_M_next) {
        CWall& wall = node->_M_data;
        C2DLineF line(wall.Point1, wall.Point2);
        C2DVector agentPos;
        agent->NavigatorInterface->GetPosition(agentPos);
        C2DVector nearest = line.GetNearestPoint(agentPos);
        agent->NavigatorInterface->GetPosition(pos);
        C2DVector toWall = nearest - pos;
        float gap = toWall.GetLength() - agent->NavigatorInterface->GetRadius(0);
        if (toWall * wall.ToInternal > 0.0f) {
            Output = C2DVector(0.0f, 0.0f);
            Reset();
            return false;
        }
        if (agent->NavigatorInterface->GetRadius(0) * 2.0f > gap) {
            C2DVector velocity;
            agent->NavigatorInterface->GetVelocity(velocity);
            if (velocity * toWall > 0.0f)
                MoveBeforeOrientation = false;
            if (!(result * wall.ToInternal > 0.0f)) {
                MoveBeforeOrientation = false;
                C2DVector along = wall.Point2 - wall.Point1;
                along.Normalise();
                C2DVector push(0.0f, 0.0f);
                C2DVector slide = along * (along * result);
                if (gap > 0.0f) {
                    push = toWall;
                    push.Normalise();
                    push = push * (push * result);
                    float limit = gap * 0.5f;
                    if (push.GetLength() > limit) {
                        push.Normalise();
                        push = push * limit;
                    }
                }
                result = push + slide;
            }
        }
    }
    Output = result;
    return true;
}
