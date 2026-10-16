// behaviour test for CThingAICreature::RequiresAutoPlacement (0x00833710): def ref taken and released around the read
#include <cstdio>
static int g_destroyed = 0;
struct CTCVillageDef { int RefCount; bool RequiresAutoPlacement; void Destroy() { ++g_destroyed; } };
struct CDefPointer { CTCVillageDef* Object;
    CDefPointer(const CDefPointer& o) : Object(o.Object) { if (Object) ++Object->RefCount; }
    CDefPointer(CTCVillageDef* d) : Object(d) {}
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    CTCVillageDef* operator->() const { return Object; } };
struct CTCVillage { CDefPointer PDef; CTCVillage(CTCVillageDef* d) : PDef(d) {} CDefPointer GetDef() const { return PDef; } };
struct CThingAICreature {
    bool hasMember; CTCVillage* village;
    bool RequiresAutoPlacement()
    {
        if (hasMember) { CTCVillage* v = village; bool requires = v && v->GetDef()->RequiresAutoPlacement; if (requires) return true; }
        return false;
    }
};
int main()
{
    CTCVillageDef def; def.RefCount = 1; def.RequiresAutoPlacement = true;
    CTCVillage village(&def);
    CThingAICreature c; c.hasMember = true; c.village = &village;
    int bad = !c.RequiresAutoPlacement() || def.RefCount != 1;
    def.RequiresAutoPlacement = false; if (c.RequiresAutoPlacement() || def.RefCount != 1) bad = 1;
    c.village = 0; if (c.RequiresAutoPlacement()) bad = 1;
    c.hasMember = false; c.village = &village; def.RequiresAutoPlacement = true; if (c.RequiresAutoPlacement()) bad = 1;
    if (g_destroyed != 0) bad = 1;
    village.PDef.Object = 0;
    if (bad) { std::printf("00833710_TEST FAIL\n"); return 1; }
    std::printf("00833710_TEST PASS\n");
    return 0;
}
