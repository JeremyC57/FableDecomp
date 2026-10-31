// Behaviour test for CSB::GetDistanceBetweenAgents (0x00A872F0): |pos(agent) - pos(other)| - (r(agent) + r(other)), the radii
// asked with "either agent is the player"; when relative the gap is divided by the agent's radius.
#include <cstdio>
#include <cmath>

class C2DVector { public: float X, Y; };
class CNavigator {
public:
    C2DVector Pos; float Radius, PlayerRadius; bool Player;
    virtual ~CNavigator() { }
    virtual float PeekNavigatorRadius(bool isPlayerRelated) const { return isPlayerRelated ? PlayerRadius : Radius; }
    virtual C2DVector PeekNavigatorPosition2D() const { return Pos; }
    virtual bool IsPlayer() const { return Player; }
};
class CSteeringAgent { public: CNavigator* NavigatorInterface; };

static float GetDistanceBetweenAgents(const CSteeringAgent* agent, const CSteeringAgent* other, bool relative)
{
    C2DVector otherPosition = other->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector position = agent->NavigatorInterface->PeekNavigatorPosition2D();
    C2DVector delta; delta.X = position.X - otherPosition.X; delta.Y = position.Y - otherPosition.Y;
    bool player = agent->NavigatorInterface->IsPlayer() || other->NavigatorInterface->IsPlayer();
    float agentRadius = agent->NavigatorInterface->PeekNavigatorRadius(player);
    float length = (float)std::sqrt(delta.X * delta.X + delta.Y * delta.Y);
    float otherRadius = other->NavigatorInterface->PeekNavigatorRadius(player);
    float distance = length - (agentRadius + otherRadius);
    if (relative)
        return distance / agent->NavigatorInterface->PeekNavigatorRadius(player);
    return distance;
}

int main()
{
    CNavigator a, b;
    a.Pos.X = 0; a.Pos.Y = 0; a.Radius = 1.0f; a.PlayerRadius = 2.0f; a.Player = false;
    b.Pos.X = 3; b.Pos.Y = 4; b.Radius = 0.5f; b.PlayerRadius = 1.5f; b.Player = false;
    CSteeringAgent sa = { &a }, sb = { &b };
    if (std::fabs(GetDistanceBetweenAgents(&sa, &sb, false) - 3.5f) > 1e-5f) { std::printf("00a872f0_TEST FAIL\n"); return 1; }
    if (std::fabs(GetDistanceBetweenAgents(&sa, &sb, true) - 3.5f) > 1e-5f) { std::printf("00a872f0_TEST FAIL\n"); return 1; }
    b.Player = true;                      // player radii: 5 - (2 + 1.5) = 1.5, relative 1.5 / 2
    if (std::fabs(GetDistanceBetweenAgents(&sa, &sb, false) - 1.5f) > 1e-5f) { std::printf("00a872f0_TEST FAIL\n"); return 1; }
    if (std::fabs(GetDistanceBetweenAgents(&sa, &sb, true) - 0.75f) > 1e-5f) { std::printf("00a872f0_TEST FAIL\n"); return 1; }
    std::printf("00a872f0_TEST PASS\n");
    return 0;
}