// 00722590 behaviour test: CTCSmokeGenerator::CreatePositions (final.cpp body over mock appearance and defs)
#include <stdio.h>
#include <string>
#include <vector>
#include <set>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct C3DVector { float X, Y, Z; };
struct CGenerationPos { C3DVector Position; void* Thing; CGenerationPos(const C3DVector& p, void* t) : Position(p), Thing(t) {} };
struct CTCGraphicAppearance {
    bool Loaded; std::set<std::string> Helpers;
    bool IsResourcesLoaded(bool) { return Loaded; }
    bool GetHelperPoint(const std::string& name, long, unsigned long, C3DVector* pos, float)
    {
        if (!Helpers.count(name)) return false;
        pos->X = (float)name[name.size() - 1]; pos->Y = 1.0f; pos->Z = 2.0f; return true;
    }
};
struct CSmokeGeneratorDef { int RefCount; float GenerationDistance; };
struct CThing { CTCGraphicAppearance* Appearance; CSmokeGeneratorDef* Def; };
struct CTCSmokeGenerator {
    CThing* ParentThing; float GenerationDistance; std::vector<CGenerationPos> GenerationPositions; bool PositionsCreated;
    void CreatePositions();
};
void CTCSmokeGenerator::CreatePositions()
{
    CTCGraphicAppearance* appearance = ParentThing->Appearance;
    if (!appearance)
        return;
    if (!appearance->IsResourcesLoaded(true))
        return;
    for (long i = 1; i < 10; ++i)
    {
        std::string prefix = std::string("") + "focus_chimney_";
        char number[16];
        if (i < 10)
            sprintf(number, "0%ld", i);
        else
            sprintf(number, "%ld", i);
        std::string helperName = prefix + number;
        C3DVector position;
        if (!appearance->GetHelperPoint(helperName, 0, 0, &position, 1.0f))
            break;
        GenerationPositions.push_back(CGenerationPos(position, 0));
    }
    CSmokeGeneratorDef* def = ParentThing->Def;
    ++def->RefCount;
    GenerationDistance = def->GenerationDistance;
    PositionsCreated = true;
    --def->RefCount;
}
int main()
{
    CTCGraphicAppearance app; app.Loaded = true;
    app.Helpers.insert("focus_chimney_01"); app.Helpers.insert("focus_chimney_02"); app.Helpers.insert("focus_chimney_04");
    CSmokeGeneratorDef def = { 1, 12.5f };
    CThing thing = { &app, &def };
    CTCSmokeGenerator g; g.ParentThing = &thing; g.GenerationDistance = 0; g.PositionsCreated = false;
    g.CreatePositions();
    CHECK(g.GenerationPositions.size() == 2 && g.GenerationPositions[1].Position.X == (float)'2');
    CHECK(g.GenerationDistance == 12.5f && g.PositionsCreated && def.RefCount == 1);
    CTCSmokeGenerator h; h.ParentThing = &thing; h.GenerationDistance = 0; h.PositionsCreated = false; app.Loaded = false;
    h.CreatePositions();
    CHECK(h.GenerationPositions.empty() && !h.PositionsCreated);
    thing.Appearance = 0; h.CreatePositions();
    CHECK(!h.PositionsCreated);
    if (!g_fail) printf("00722590_TEST PASS\n");
    return g_fail;
}