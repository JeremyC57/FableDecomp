// Behaviour test for CCreatureAction_ActivateZTarget::PerformAction (0x008424A0): the player gets mode 2, not exclusive.
#include <cstdio>
enum EPlayerMode { };
class CPlayer {
public:
    long lastMode; bool lastExclusive; int calls;
    CPlayer() : lastMode(-1), lastExclusive(true), calls(0) { }
    void AddMode(EPlayerMode mode, bool exclusive) { lastMode = (long)mode; lastExclusive = exclusive; ++calls; }
};
class CCreatureActionBase {
public:
    CPlayer* player;
    virtual ~CCreatureActionBase() { }
    virtual void PerformAction() { }
protected:
    CPlayer& GetPlayer() { return *player; }
};
class CCreatureAction_ActivateZTarget : public CCreatureActionBase {
public:
    virtual void PerformAction();
};
void CCreatureAction_ActivateZTarget::PerformAction()
{
    GetPlayer().AddMode((EPlayerMode)2, false);
}
int main()
{
    CPlayer p;
    CCreatureAction_ActivateZTarget a;
    a.player = &p;
    CCreatureActionBase* base = &a;
    base->PerformAction();
    if (p.calls != 1 || p.lastMode != 2 || p.lastExclusive) { std::printf("008424a0_TEST FAIL\n"); return 1; }
    std::printf("008424a0_TEST PASS\n");
    return 0;
}