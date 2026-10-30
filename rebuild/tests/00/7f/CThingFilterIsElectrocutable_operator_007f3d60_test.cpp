// Behaviour test for 0x007F3D60 CThingFilter_IsElectrocutable::operator() (see src/007f3d60.cpp): a thing with no
// definition is not electrocutable; otherwise the def's Electrocutable bit decides. Every counted copy is released again.
#include <cstdio>
struct Def { int RefCount; int Deleted; bool IsFragile : 1; bool IsClothing : 1; bool IsSelfIlluminating : 1; bool UseHighDetailQuadTree : 1; bool Electrocutable : 1; };
struct DefPointer {
    Def* Object;
    DefPointer(Def* d) : Object(d) { if (Object) ++Object->RefCount; }
    DefPointer(const DefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    ~DefPointer() { if (Object && --Object->RefCount == 0) ++Object->Deleted; }
};
struct Thing { Def* PBaseDef; DefPointer GetPBaseDef() const { return DefPointer(PBaseDef); } };
static bool IsElectrocutable(const Thing& t)
{
    if (t.GetPBaseDef().Object != 0) return t.GetPBaseDef().Object->Electrocutable;
    return false;
}
int main()
{
    Thing none = { 0 };
    Def d = Def(); d.RefCount = 1; d.Electrocutable = true; Thing yes = { &d };
    Def e = Def(); e.RefCount = 1; e.Electrocutable = false; e.IsFragile = true; Thing no = { &e };
    if (IsElectrocutable(none) || !IsElectrocutable(yes) || IsElectrocutable(no)) { std::printf("007f3d60_TEST FAIL\n"); return 1; }
    if (d.RefCount != 1 || e.RefCount != 1 || d.Deleted || e.Deleted) { std::printf("007f3d60_TEST FAIL\n"); return 1; }
    std::printf("007f3d60_TEST PASS\n");
    return 0;
}