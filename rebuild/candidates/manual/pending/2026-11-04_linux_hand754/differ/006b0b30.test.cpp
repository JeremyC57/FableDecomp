// Behaviour test for CTCPhysicsBase::InitialActivate (0x006B0B30): a model of the row.
#include <stdio.h>

struct Mapwho { bool accept; int placed; float at; };
static void InitialActivate(bool toBePlaced, Mapwho* mapwho, float offset, float position)
{
    if (toBePlaced && mapwho->accept == true && (offset + position) >= 0) { mapwho->placed++; mapwho->at = offset + position; }
}
static int run()
{
    Mapwho m = { true, 0, 0 }, no = { false, 0, 0 };
    InitialActivate(true, &m, 1.5f, 2.0f); InitialActivate(false, &m, 1.5f, 2.0f); InitialActivate(true, &no, 1, 1);
    return m.placed == 1 && m.at == 3.5f && no.placed == 0;
}
int main()
{
    int ok = run();
    printf(ok ? "006b0b30_TEST PASS\n" : "006b0b30_TEST FAIL\n");
    return ok ? 0 : 1;
}
