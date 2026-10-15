#include <cstdio>
struct CThing;
static CThing* g_baseArg = 0;
struct CTCPhysicsBase {
    virtual ~CTCPhysicsBase() { }
    unsigned char _base[0x3c];
    CTCPhysicsBase(CThing* thing) { g_baseArg = thing; }
};
struct CTCPhysicsLight : CTCPhysicsBase {
    bool AffectedByGravity : 1;
    bool Other : 1;
    CTCPhysicsLight(CThing* thing) : CTCPhysicsBase(thing) { AffectedByGravity = false; }
};
int main()
{
    CThing* t = (CThing*)0x9abc;
    CTCPhysicsLight l(t);
    if (g_baseArg != t || l.AffectedByGravity) { std::printf("00723400_TEST FAIL\n"); return 1; }
    std::printf("00723400_TEST PASS\n");
    return 0;
}