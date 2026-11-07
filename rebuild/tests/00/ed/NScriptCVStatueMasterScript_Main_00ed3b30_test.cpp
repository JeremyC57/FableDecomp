// Behaviour test for NScript::CV_StatueMasterScript::Main (0x00ED3B30): a copy of the Main logic over a mock game; checks the three
// bindings and the four centre-to-marker XY angles (atan2(dx, dy) / 2 pi wrapped into [0, 1)), including a null marker (NullCoord).
#include <cstdio>
#include <cstring>
#include <math.h>

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetAngleXY() const;
};
static float GFArcTan(double opposite, double adjacent) { return (float)atan2(opposite, adjacent) / (2.0f * 3.14159265358979323846f); }
static float GET_ROUNDED_ANGLE(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0f);
        if (angle < 0.0f) angle += 1.0f;
    }
    return angle;
}
float C3DVector::GetAngleXY() const { return GET_ROUNDED_ANGLE(GFArcTan(X, Y)); }

static C3DVector NullCoord(0.0f, 0.0f, 0.0f);
struct Thing { const C3DVector* pos; const C3DVector& GetPos() const { return pos ? *pos : NullCoord; } };

struct MockGame {
    int bindings;
    const char* names[5]; C3DVector* positions[5];
    Thing GetThingWithScriptName(const char* name) {
        Thing t; t.pos = 0;
        for (int i = 0; i < 5; ++i) if (std::strcmp(names[i], name) == 0) t.pos = positions[i];
        return t;
    }
};

struct Script {
    MockGame* Game;
    float AnglesToFaceList[4];
    void AddEntityScriptBinding(const char*) { ++Game->bindings; }
    void Main()
    {
        AddEntityScriptBinding("StatueMasterStatue");
        AddEntityScriptBinding("StatueMasterCellarDoors");
        AddEntityScriptBinding("StatueMasterChest");
        Thing Nothing = Game->GetThingWithScriptName("SM_Nothing");
        Thing Bowerstone = Game->GetThingWithScriptName("SM_Bowerstone");
        Thing Guild = Game->GetThingWithScriptName("SM_Guild");
        Thing Greatwood = Game->GetThingWithScriptName("SM_Greatwood");
        Thing Center = Game->GetThingWithScriptName("SM_Center");
        AnglesToFaceList[0] = (Nothing.GetPos() - Center.GetPos()).GetAngleXY();
        AnglesToFaceList[1] = (Bowerstone.GetPos() - Center.GetPos()).GetAngleXY();
        AnglesToFaceList[2] = (Guild.GetPos() - Center.GetPos()).GetAngleXY();
        AnglesToFaceList[3] = (Greatwood.GetPos() - Center.GetPos()).GetAngleXY();
    }
};

static bool Near(float a, float b) { return a - b < 1e-4f && b - a < 1e-4f; }

int main()
{
    C3DVector center(10.0f, 20.0f, 5.0f), north(10.0f, 25.0f, 0.0f), east(13.0f, 20.0f, 0.0f), south(10.0f, 18.0f, 0.0f), west(4.0f, 20.0f, 9.0f);
    MockGame g; g.bindings = 0;
    g.names[0] = "SM_Nothing"; g.names[1] = "SM_Bowerstone"; g.names[2] = "SM_Guild"; g.names[3] = "SM_Greatwood"; g.names[4] = "SM_Center";
    g.positions[0] = &north; g.positions[1] = &east; g.positions[2] = &south; g.positions[3] = &west; g.positions[4] = &center;
    Script s; s.Game = &g; s.Main();
    if (g.bindings != 3 || !Near(s.AnglesToFaceList[0], 0.0f) || !Near(s.AnglesToFaceList[1], 0.25f) ||
        !Near(s.AnglesToFaceList[2], 0.5f) || !Near(s.AnglesToFaceList[3], 0.75f)) {
        std::printf("00ed3b30_TEST FAIL\n"); return 1;
    }
    g.positions[4] = 0;    // no centre marker: positions measured from NullCoord (retail has no null check)
    s.Main();
    if (!Near(s.AnglesToFaceList[1], (float)(atan2(13.0, 20.0) / 6.283185307179586))) { std::printf("00ed3b30_TEST FAIL\n"); return 1; }
    std::printf("00ed3b30_TEST PASS\n");
    return 0;
}