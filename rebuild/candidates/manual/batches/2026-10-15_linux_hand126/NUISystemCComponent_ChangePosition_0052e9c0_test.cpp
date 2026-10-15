// Behaviour test for 0052e9c0 NUISystem::CComponent::ChangePosition: a positive time keeps the current
// position as the interpolation start and restarts the timer; zero time snaps every position.
#include <cstdio>

struct C2DVector { float X, Y; };
struct CComponent {
    C2DVector Position, TargetPosition, InitialPosition; float PositionTimeElapsed, PositionTime;
    void ChangePosition(const C2DVector& position, float time, long)
    {
        PositionTime = time;
        if (time > 0.0f) { TargetPosition = position; InitialPosition = Position; PositionTimeElapsed = 0.0f; }
        else { TargetPosition = position; InitialPosition = position; Position = position; }
    }
};

int main()
{
    CComponent c; c.Position.X = 1; c.Position.Y = 2; c.InitialPosition.X = c.InitialPosition.Y = 0;
    c.PositionTimeElapsed = 5.0f;
    C2DVector p; p.X = 10; p.Y = 20;
    c.ChangePosition(p, 0.5f, 0);
    bool ok = c.PositionTime == 0.5f && c.TargetPosition.X == 10 && c.TargetPosition.Y == 20
        && c.InitialPosition.X == 1 && c.InitialPosition.Y == 2 && c.PositionTimeElapsed == 0.0f && c.Position.X == 1;
    c.PositionTimeElapsed = 3.0f;
    p.X = 7; p.Y = 8;
    c.ChangePosition(p, 0.0f, 0);
    ok = ok && c.Position.X == 7 && c.Position.Y == 8 && c.InitialPosition.X == 7 && c.InitialPosition.Y == 8
        && c.TargetPosition.X == 7 && c.PositionTimeElapsed == 3.0f && c.PositionTime == 0.0f;
    if (!ok) { std::printf("0052e9c0_TEST FAIL\n"); return 1; }
    std::printf("0052e9c0_TEST PASS\n");
    return 0;
}
