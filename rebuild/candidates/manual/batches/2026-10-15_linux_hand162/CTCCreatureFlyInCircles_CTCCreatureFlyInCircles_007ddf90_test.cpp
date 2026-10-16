#include <cstdio>
// Behaviour model of CTCCreatureFlyInCircles::CTCCreatureFlyInCircles (0x007DDF90).
struct Fly {
    void* parent; void* Creature; float Speed, DeltaSpeed; int SpeedChangeTicks; float CurrentRadius, DeltaRadius; int RadiusChangeTicks;
    int Mode; bool Transitioning, ModeChangeQueued, Active, HasBeenHit; float Tension; bool EndPointsConnected; int points; int LastExplodeTick;
    Fly(void* p) : parent(p), Creature(parent)
    {
        EndPointsConnected = false; Tension = 0.5f; points = 0;
        Active = true; Mode = 1; Transitioning = false; ModeChangeQueued = false; HasBeenHit = false;
        CurrentRadius = 0; DeltaRadius = 0; RadiusChangeTicks = 0; Speed = 3.0f; DeltaSpeed = 0; SpeedChangeTicks = 0;
        LastExplodeTick = 0; Tension = 0.5f; points = 3;
    }
};
int main()
{
    int owner = 0;
    Fly f(&owner);
    if (f.Creature != &owner || f.Speed != 3.0f || f.Mode != 1 || !f.Active || f.Transitioning || f.Tension != 0.5f || f.points != 3 || f.EndPointsConnected) { std::printf("007ddf90_TEST FAIL\n"); return 1; }
    std::printf("007ddf90_TEST PASS\n");
    return 0;
}
