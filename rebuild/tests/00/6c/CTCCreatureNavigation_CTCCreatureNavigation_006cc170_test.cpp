#include <cstdio>
// Behaviour model of CTCCreatureNavigation::CTCCreatureNavigation (0x006CC170).
struct Nav {
    void* PNavigator; void* pnFirst; void* pnLast; void* pnEnd; float Proximity; int MovementType; float TargetSpeed; float Speed;
    void* avoid; void* agFirst; void* agLast; void* agEnd; int NextUpdate; int FrameLastNavigated; bool CalculatingNewRoute;
    bool CanMove; bool GotTarget; float ProxDirect; float CurrentSpeed;
    Nav() : PNavigator(0), pnFirst(0), pnLast(0), pnEnd(0), Proximity(0), MovementType(0), TargetSpeed(0), Speed(0), avoid(0),
            agFirst(0), agLast(0), agEnd(0), NextUpdate(0), CanMove(false), GotTarget(false), ProxDirect(0), CurrentSpeed(0)
    { FrameLastNavigated = -1; CalculatingNewRoute = true; }
};
int main()
{
    Nav n;
    if (n.PNavigator || n.pnFirst || n.agEnd || n.FrameLastNavigated != -1 || !n.CalculatingNewRoute || n.CanMove || n.GotTarget || n.CurrentSpeed != 0) { std::printf("006cc170_TEST FAIL\n"); return 1; }
    std::printf("006cc170_TEST PASS\n");
    return 0;
}