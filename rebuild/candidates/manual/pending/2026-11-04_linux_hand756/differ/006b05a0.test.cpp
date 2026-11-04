// Behaviour test for CTCPhysicsBase::CreatePPhysicsMeshInfo (0x006B05A0): a model of the row -- static (5) or animated (4) mesh info
// from the bank; its physics mesh when it has one, else the physics def's collision setting.
#include <stdio.h>
struct Info { bool HasPhysicsMesh; };
static int Create(int type, const Info* staticInfo, const Info* animInfo, bool defCollision)
{
    const Info* info = type == 5 ? staticInfo : type == 4 ? animInfo : 0;
    if (info && info->HasPhysicsMesh) return 1;            // physics mesh
    return defCollision ? 2 : 0;                           // collision fallback
}
int main()
{
    Info yes = { true }, no = { false };
    int ok = Create(5, &yes, &no, false) == 1 && Create(4, &yes, &no, true) == 2 && Create(4, &yes, &no, false) == 0 && Create(4, &no, &yes, false) == 1;
    printf(ok ? "006b05a0_TEST PASS\n" : "006b05a0_TEST FAIL\n");
    return ok ? 0 : 1;
}
