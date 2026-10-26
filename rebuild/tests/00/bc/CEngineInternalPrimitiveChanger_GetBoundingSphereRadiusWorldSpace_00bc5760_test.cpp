// behaviour test for 0x00BC5760 CEngineInternalPrimitiveChanger::GetBoundingSphereRadiusWorldSpace(float& radius): with no active child (index -1), an index past the list, or an empty handle the answer is false; otherwise the call (and its arguments) goes to the active child, whose answer is returned
#include <cstdio>
#include <vector>
struct Arg { int v; };
struct Prim {
    int calls; bool answer; const Arg* seen; bool seenFlag;
    Prim(bool a) : calls(0), answer(a), seen(0), seenFlag(false) { }
    virtual ~Prim() { }
    virtual bool GetBoundingSphereRadiusWorldSpace(const Arg& a) { ++calls; seen = &a; seenFlag = true; return answer; }
};
struct Handle { Prim* Data; };
struct Changer {
    std::vector<Handle> Primitives;
    int ActiveIndex;
    bool HasActivePrimitive() const { return ActiveIndex != -1 && (unsigned int)ActiveIndex < Primitives.size() && Primitives[ActiveIndex].Data != 0; }
    Prim& GetActivePrimitive() { return *Primitives[ActiveIndex].Data; }
    bool GetBoundingSphereRadiusWorldSpace(const Arg& a)                    // the function under test
    {
        if (HasActivePrimitive())
            return GetActivePrimitive().GetBoundingSphereRadiusWorldSpace(a);
        return false;
    }
};
int main()
{
    int fail = 0;
    Prim yes(true), no(false);
    Arg arg; arg.v = 5;
    Changer c;
    Handle h0; h0.Data = &no; Handle h1; h1.Data = 0; Handle h2; h2.Data = &yes;
    c.Primitives.push_back(h0); c.Primitives.push_back(h1); c.Primitives.push_back(h2);
    c.ActiveIndex = -1;
    if (c.GetBoundingSphereRadiusWorldSpace(arg) != false) fail |= 1;
    c.ActiveIndex = 3;
    if (c.GetBoundingSphereRadiusWorldSpace(arg) != false) fail |= 2;
    c.ActiveIndex = 1;
    if (c.GetBoundingSphereRadiusWorldSpace(arg) != false) fail |= 4;
    if (yes.calls != 0 || no.calls != 0) fail |= 8;
    c.ActiveIndex = 2;
    if (c.GetBoundingSphereRadiusWorldSpace(arg) != true || yes.calls != 1 || yes.seen != &arg || yes.seenFlag != true) fail |= 16;
    c.ActiveIndex = 0;
    if (c.GetBoundingSphereRadiusWorldSpace(arg) != false || no.calls != 1 || no.seen != &arg) fail |= 32;
    if (fail) { std::printf("00bc5760_TEST FAIL %d\n", fail); return 1; }
    std::printf("00bc5760_TEST PASS\n");
    return 0;
}