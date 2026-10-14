#include <cstdio>
// Model of InitStateGroup @ 0x0096f6f0: timer = -1, GetSubDef(&PDef) through a counted temporary of the creature's def.
struct Def { long refs; int deleted; int subDefCalls; void* outArg; void Delete() { ++deleted; } bool GetSubDef(void* out) { ++subDefCalls; outArg = out; return true; } };
struct DefPointer {
    Def* Object;
    DefPointer(Def* o) : Object(o) { if (Object) ++Object->refs; }
    ~DefPointer() { if (Object) { if (--Object->refs == 0) Object->Delete(); Object = 0; } }
    Def* operator->() const { return Object; }
};
struct Group {
    Def* creatureDef; void* PDef; long NextFrame;
    void InitStateGroup() { NextFrame = -1; DefPointer def(creatureDef); def->GetSubDef(&PDef); }
};
int main()
{
    Def d = { 1, 0, 0, 0 }; Group g; g.creatureDef = &d; g.NextFrame = 0;
    g.InitStateGroup();
    if (g.NextFrame != -1 || d.subDefCalls != 1 || d.outArg != &g.PDef || d.refs != 1 || d.deleted != 0) { std::printf("0096f6f0_TEST FAIL\n"); return 1; }
    d.refs = 0; g.InitStateGroup();
    if (d.deleted != 1 || d.refs != 0) { std::printf("0096f6f0_TEST FAIL\n"); return 1; }
    std::printf("0096f6f0_TEST PASS\n"); return 0;
}