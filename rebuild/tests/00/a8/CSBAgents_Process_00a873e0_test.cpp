// Behaviour test for CSBAgents::Process (0x00A873E0): inverse-square repulsion summed over live, relevant agents.
#include <cstdio>
#include <cmath>
#include <vector>

class C2DVector { public:
    float X, Y;
    C2DVector() { }
    C2DVector(float x, float y) : X(x), Y(y) { }
    C2DVector operator-(const C2DVector& v) const { return C2DVector(X - v.X, Y - v.Y); }
    const C2DVector& operator*=(float s) { X *= s; Y *= s; return *this; }
    const C2DVector& operator+=(const C2DVector& v) { X += v.X; Y += v.Y; return *this; }
    C2DVector& Normalise() { float l = std::sqrt(X * X + Y * Y); if (l > 0) { X /= l; Y /= l; } return *this; }
};
class CKinematic { public: C2DVector Vector; CKinematic() { } CKinematic(const C2DVector& v) : Vector(v) { } };
class CSteeringAgent;
struct CNav {
    C2DVector Pos; float Radius; bool Player; bool Dead; const CSteeringAgent* NotAvoid;
    float PeekNavigatorRadius(bool) const { return Radius; }
    C2DVector PeekNavigatorPosition2D() const { return Pos; }
    const CSteeringAgent* GetPThingToNotAvoid() const { return NotAvoid; }
    bool IsPlayer() const { return Player; }
    bool IsDead() const { return Dead; }
};
class CSteeringAgent { public:
    CNav* NavigatorInterface; float DistCutoff; bool Wandering;
    float GetRadius(bool p) const { return NavigatorInterface->PeekNavigatorRadius(p); }
    C2DVector GetPosition() const { return NavigatorInterface->PeekNavigatorPosition2D(); }
    const CSteeringAgent* GetTarget() const { return NavigatorInterface->GetPThingToNotAvoid(); }
    bool IsDead() const { return NavigatorInterface->IsDead(); }
    bool IsPlayer() const { return NavigatorInterface->IsPlayer(); }
    bool IsWandering() const { return Wandering; }
    float GetDistCutoff() const { return DistCutoff; }
};
static bool g_SBAgentsDisabled_013d2d54 = false;
struct CSBAgents {
    CKinematic Output;
    std::vector<const CSteeringAgent*>* Agents;
    bool ShouldIgnoreAgent(const CSteeringAgent* a, const CSteeringAgent* o, const CSteeringAgent* ignore) { return a == o || o == ignore; }
    float GetDistanceBetweenAgents(const CSteeringAgent* a, const CSteeringAgent* o, bool)
    {
        C2DVector d = a->GetPosition() - o->GetPosition();
        return (std::sqrt(d.X * d.X + d.Y * d.Y) - (a->GetRadius(false) + o->GetRadius(false))) / a->GetRadius(false);
    }
    bool Process(const CSteeringAgent* agent)
    {
        if (g_SBAgentsDisabled_013d2d54) {
            Output = CKinematic(C2DVector(0.0f, 0.0f));
            return true;
        }
        C2DVector force;
        force.X = force.Y = 0.0f;
        const CSteeringAgent* target = agent->GetTarget();
        for (std::vector<const CSteeringAgent*>::iterator it = Agents->begin(); it != Agents->end(); ++it) {
            if ((*it)->IsDead())
                continue;
            if (ShouldIgnoreAgent(agent, *it, target))
                continue;
            C2DVector away = agent->GetPosition() - (*it)->GetPosition();
            float dist = GetDistanceBetweenAgents(agent, *it, true);
            if (dist < 0.0f && !(*it)->IsPlayer() && !agent->IsWandering())
                continue;
            float cutoff = agent->GetDistCutoff();
            if (dist < cutoff / agent->GetRadius(false)) {
                away.Normalise();
                if (dist < 1.0f)
                    dist = 1.0f;
                away *= 1.0f / (dist * dist);
                force += away;
            }
        }
        Output = CKinematic(force);
        return true;
    }
};

static bool near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    CNav n0 = { C2DVector(0, 0), 1.0f, false, false, 0 };
    CNav n1 = { C2DVector(4, 0), 1.0f, false, false, 0 };    // dist (4-2)/1 = 2 -> repel -x by 1/4
    CNav n2 = { C2DVector(0, 3), 1.0f, false, true, 0 };     // dead: skipped
    CNav n3 = { C2DVector(0, -100), 1.0f, false, false, 0 }; // beyond cutoff
    CSteeringAgent a0 = { &n0, 10.0f, false }, a1 = { &n1, 10.0f, false }, a2 = { &n2, 10.0f, false }, a3 = { &n3, 10.0f, false };
    std::vector<const CSteeringAgent*> list;
    list.push_back(&a0); list.push_back(&a1); list.push_back(&a2); list.push_back(&a3);
    CSBAgents sb; sb.Agents = &list; sb.Output = CKinematic(C2DVector(9, 9));
    bool ok = sb.Process(&a0) && near(sb.Output.Vector.X, -0.25f) && near(sb.Output.Vector.Y, 0.0f);
    n1.Pos = C2DVector(2.5f, 0);                              // dist 0.5 -> clamped to 1 -> full unit push
    ok = ok && sb.Process(&a0) && near(sb.Output.Vector.X, -1.0f);
    n0.NotAvoid = &a1;                                        // the ignored agent contributes nothing
    ok = ok && sb.Process(&a0) && near(sb.Output.Vector.X, 0.0f);
    g_SBAgentsDisabled_013d2d54 = true; sb.Output = CKinematic(C2DVector(5, 5));
    ok = ok && sb.Process(&a0) && sb.Output.Vector.X == 0.0f && sb.Output.Vector.Y == 0.0f;
    if (!ok) { std::printf("00a873e0_TEST FAIL\n"); return 1; }
    std::printf("00a873e0_TEST PASS\n");
    return 0;
}