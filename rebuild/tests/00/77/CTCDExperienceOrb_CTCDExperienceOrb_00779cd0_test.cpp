// Behaviour test for CTCDExperienceOrb::~CTCDExperienceOrb (0x00779CD0): Reset(0) releases the def reference and clears
// the pointer, so the member destructor has nothing left to release.
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
struct Def { int refs; };
struct DefPointer { Def* Object; void Release() { if (Object) --Object->refs; }
    void Reset(Def* d) { Release(); Object = d; if (Object) ++Object->refs; } ~DefPointer() { Release(); } };
struct Orb { DefPointer PDef; ~Orb() { PDef.Reset(0); } };
int main()
{
    Def d = { 1 };
    { Orb o; o.PDef.Object = &d; }
    bool ok = d.refs == 0;
    printf(ok ? "00779cd0_TEST PASS\n" : "00779cd0_TEST FAIL\n");
    return ok ? 0 : 1;
}