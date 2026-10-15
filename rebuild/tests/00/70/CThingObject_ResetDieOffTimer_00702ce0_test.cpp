#include <cstdio>
// Behaviour test for CThingObject::ResetDieOffTimer (0x00702CE0).
struct CThingObjectDef { long DieOffTimer; };
struct CThingObject {
    const CThingObjectDef* PDef; long DieOffTimer; bool DiesOff;
    void ResetDieOffTimer() { DieOffTimer = PDef->DieOffTimer; DiesOff = DieOffTimer > 0; }
};
int main()
{
    CThingObjectDef d; CThingObject o; o.PDef = &d; o.DieOffTimer = 99; o.DiesOff = true;
    int fails = 0;
    d.DieOffTimer = 0; o.ResetDieOffTimer(); if (o.DieOffTimer != 0 || o.DiesOff) ++fails;
    d.DieOffTimer = 250; o.ResetDieOffTimer(); if (o.DieOffTimer != 250 || !o.DiesOff) ++fails;
    d.DieOffTimer = -4; o.ResetDieOffTimer(); if (o.DieOffTimer != -4 || o.DiesOff) ++fails;
    if (fails) { std::printf("00702ce0_TEST FAIL\n"); return 1; }
    std::printf("00702ce0_TEST PASS\n");
    return 0;
}