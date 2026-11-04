// Behaviour test for CMessageEventFilter_IsMyDeadLeader::operator() (0x006AF3F0, parked DIFFER row): a model of the row -- true only for a death message (type 9) whose thing has my leader definition.
#include <stdio.h>
struct Thing { long DefIndex; };
struct Msg { long Type; Thing* About; };
static bool IsMyDeadLeader(const Msg* m, long leaderDef) { return m->Type == 9 && m->About && m->About->DefIndex == leaderDef; }
static int Check() { Thing t = { 7 }; Msg a = { 9, &t }, b = { 3, &t }, c = { 9, 0 }; return IsMyDeadLeader(&a, 7) && !IsMyDeadLeader(&a, 8) && !IsMyDeadLeader(&b, 7) && !IsMyDeadLeader(&c, 7); }
int main()
{
    int ok = Check();
    printf(ok ? "006af3f0_TEST PASS\n" : "006af3f0_TEST FAIL\n");
    return ok ? 0 : 1;
}
