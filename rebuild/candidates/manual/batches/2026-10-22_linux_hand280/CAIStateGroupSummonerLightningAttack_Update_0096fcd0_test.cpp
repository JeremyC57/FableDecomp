// Behaviour test for CAIStateGroup_SummonerLightningAttack::Update (0x0096FCD0): a live target is marked in danger, live weapons are sheathed (dead ones skipped), the summoner faces the target, strikes, waits out running actions, smashes out and schedules the next preemptive use; a dead target or busy summoner only terminates.
#include <cstdio>
struct CWeapon { bool dead; };
struct CState {
    bool targetAlive, busy, inDanger, terminated, struck, smashed;
    CWeapon* slots[4]; int slotCount, slotIndex, sheathed, waits, performing;
    long nextFrame, frame, fps; float delay;
    CWeapon* GetThingInPrimarySlot() { return slotIndex < slotCount ? slots[slotIndex++] : 0; }
    long Sheathe() { ++sheathed; return 0; }
    void Update()
    {
        if (targetAlive && !busy) {
            inDanger = true;
            for (;;) {
                CWeapon* weapon = GetThingInPrimarySlot();
                if (!weapon)
                    break;
                if (weapon->dead)
                    continue;
                long result = Sheathe();
                if (result != 1)
                    continue;
                terminated = true;
                return;
            }
            if (targetAlive) {
                struck = true;
                while (performing > 0) { --performing; ++waits; }
                smashed = true;
                nextFrame = (long)((float)frame + (float)fps * delay);
            }
        }
        terminated = true;
    }
};
int main()
{
    CWeapon alive = { false }, dead = { true };
    CState s = { true, false, false, false, false, false, { &alive, &dead, &alive, 0 }, 3, 0, 0, 0, 2, 0, 100, 30, 1.5f };
    s.Update();
    bool ok = s.inDanger && s.sheathed == 2 && s.struck && s.waits == 2 && s.smashed && s.nextFrame == 145 && s.terminated;
    CState b = { true, true, false, false, false, false, { 0, 0, 0, 0 }, 0, 0, 0, 0, 0, 7, 100, 30, 1.5f };
    b.Update();
    ok = ok && !b.inDanger && !b.struck && b.nextFrame == 7 && b.terminated;
    if (!ok) { std::printf("0096fcd0_TEST FAIL\n"); return 1; }
    std::printf("0096fcd0_TEST PASS\n");
    return 0;
}
