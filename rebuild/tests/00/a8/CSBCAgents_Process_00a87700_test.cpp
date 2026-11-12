// Behaviour test for 0x00A87700 CSBCAgents::Process(const CSteeringAgent*): the row's logic over stub classes (C++03).
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
static void check(bool v, int line) { if (!v) { printf("00a87700_TEST FAIL line %d\n", line); exit(1); } }
class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    C2DVector operator+(const C2DVector& v) const { return C2DVector(X + v.X, Y + v.Y); }
    C2DVector operator*(float s) const { return C2DVector(X * s, Y * s); }
    C2DVector& Normalise() { float l = (float)sqrt(X * X + Y * Y); if (l > 0.0f) { X /= l; Y /= l; } return *this; }
    void Rotate(float turns) { float a = (float)atan2(Y, X) + turns * 6.2831853f; float l = (float)sqrt(X * X + Y * Y);
        X = l * (float)cos(a); Y = l * (float)sin(a); }
};
class CKinematic { public: C2DVector Vector; CKinematic() { } CKinematic(const C2DVector& v) : Vector(v) { }
    const C2DVector& GetVector() const { return Vector; } void SetVector(const C2DVector& v) { Vector = v; } };
class CSteeringAgent { public:
    C2DVector Pos; float Radius; bool Player, Wandering, AvoidDynamicObstacles; const CSteeringAgent* Target;
    const CSteeringAgent* GetTarget() const { return Target; } bool IsPlayer() const { return Player; }
    C2DVector GetPosition() const { return Pos; } float GetRadius(bool) const { return Radius; }
    bool IsWandering() const { return Wandering; } bool GetAvoidDynamicObstacles() const { return AvoidDynamicObstacles; } };
static bool g_SBCAgentsDisabled;
class CSBCAgents { public:
    CKinematic Output; CKinematic Input; bool MoveBeforeOrientation; std::vector<const CSteeringAgent*>* Agents;
    bool ShouldIgnoreAgent(const CSteeringAgent*, const CSteeringAgent* other, const CSteeringAgent* target) { return other == target; }
    float GetDistanceBetweenAgents(const CSteeringAgent* a, const CSteeringAgent* b, bool)
    { C2DVector d = a->Pos - b->Pos; return (float)sqrt(d.X * d.X + d.Y * d.Y) - (a->Radius + b->Radius); }
    bool Process(const CSteeringAgent* agent);
};
bool CSBCAgents::Process(const CSteeringAgent* agent)
{
    if (g_SBCAgentsDisabled) {
        Output = Input;
        return true;
    }
    MoveBeforeOrientation = true;
    const CSteeringAgent* target = agent->GetTarget();
    for (std::vector<const CSteeringAgent*>::iterator it = Agents->begin(); it != Agents->end(); ++it) {
        if (ShouldIgnoreAgent(agent, *it, target) && (!agent->GetAvoidDynamicObstacles() || !(*it)->IsPlayer()))
            continue;
        C2DVector away = agent->GetPosition() - (*it)->GetPosition();
        if (GetDistanceBetweenAgents(agent, *it, false) <= 0.0f && !(*it)->IsPlayer() && !agent->IsWandering())
            continue;
        C2DVector input = Input.GetVector();
        away = (*it)->GetPosition() - agent->GetPosition();
        float dist = GetDistanceBetweenAgents(agent, *it, false);
        away.Normalise();
        float ahead = dist - (away.X * input.X + away.Y * input.Y);
        if (ahead < agent->GetRadius(false) * 0.1f && ahead < dist) {
            dist -= 0.1f;
            if (dist < 0.0f)
                dist = 0.0f;
            C2DVector side = away;
            side.Rotate(0.25f);
            C2DVector push = away * dist;
            float along = input.X * side.X + input.Y * side.Y;
            Output.SetVector(side * along + push);
            return true;
        }
    }
    Output = Input;
    return true;
}

static CSteeringAgent make(float x, float y) { CSteeringAgent a; a.Pos = C2DVector(x, y); a.Radius = 1.0f; a.Player = false;
    a.Wandering = false; a.AvoidDynamicObstacles = false; a.Target = 0; return a; }
int main()
{
    CSteeringAgent me = make(0.0f, 0.0f), far = make(10.0f, 0.0f), near = make(2.5f, 0.0f);
    std::vector<const CSteeringAgent*> list; list.push_back(&far);
    CSBCAgents c; c.Agents = &list; c.Input = CKinematic(C2DVector(1.0f, 0.0f)); c.MoveBeforeOrientation = false;
    g_SBCAgentsDisabled = true;
    check(c.Process(&me) && c.Output.Vector.X == 1.0f && !c.MoveBeforeOrientation, __LINE__);   // disabled: pass through
    g_SBCAgentsDisabled = false; c.Output = CKinematic(C2DVector(9.0f, 9.0f));
    check(c.Process(&me) && c.MoveBeforeOrientation && c.Output.Vector.X == 1.0f && c.Output.Vector.Y == 0.0f, __LINE__);   // far: pass through
    list.push_back(&near);                                                  // overlapping agent straight ahead: blocked
    check(c.Process(&me), __LINE__);
    check(fabs(c.Output.Vector.X - 0.4f) < 1e-3f && fabs(c.Output.Vector.Y) < 1e-3f, __LINE__);   // gap 0.5 - 0.1 pushed along the away axis
    me.Target = &near; c.Output = CKinematic(C2DVector(9.0f, 9.0f));       // the target is ignored
    check(c.Process(&me) && c.Output.Vector.X == 1.0f && c.Output.Vector.Y == 0.0f, __LINE__);
    me.AvoidDynamicObstacles = true; near.Player = true;                     // ...unless it is the player and we avoid dynamic obstacles
    c.Output = CKinematic(C2DVector(9.0f, 9.0f));
    check(c.Process(&me) && !(c.Output.Vector.X == 1.0f && c.Output.Vector.Y == 0.0f), __LINE__);
    printf("00a87700_TEST PASS\n");
    return 0;
}