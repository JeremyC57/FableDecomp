// Behaviour test for 0x007FD650 CTCDPhysicalObstruction::FrameUpdate: a standalone copy of the model. With the debug flag on the
// function must set three blue persistent lines (post at the start, post at the end, bar between the lifted points, width 0.05,
// z-buffered) on line1 / line2 / line3 in that order, each with flags 0; it always runs the impulse update once, and the life
// timer counts down only while positive and kills the parent exactly when it reaches zero.
#include <cstdio>
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    const C3DVector operator+(const C3DVector& other) const { return C3DVector(X + other.X, Y + other.Y, Z + other.Z); }
};
class CRGBColour { public:
    unsigned char B, G, R, A;
    CRGBColour() { }
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class CEnginePrimitiveHandle { public: int id; };
class CEnginePrimitiveLine { public:
    unsigned __int64 Id;
    C3DVector Start, End;
    CRGBColour Colour;
    float Width;
    unsigned long BankIndex;
    int BlendMode, BlendOp, InterpolationSet;
    bool UseZBuffer;
    CEnginePrimitiveLine() { }
    CEnginePrimitiveLine(unsigned __int64 id, const C3DVector& start, const C3DVector& end, const CRGBColour& colour, float width,
                         int blend_mode, unsigned long bank_index, bool use_z, int interpolation_set, int blend_op)
        : Id(id), Start(start), End(end), Colour(colour), Width(width), BankIndex(bank_index), BlendMode(blend_mode),
          BlendOp(blend_op), InterpolationSet(interpolation_set), UseZBuffer(use_z) { }
};
class CIEngine { public:
    int count;
    int handleIds[8];
    unsigned long flags[8];
    CEnginePrimitiveLine lines[8];
    CIEngine() : count(0) { }
    void SetPersistentPrimitive(CEnginePrimitiveHandle& handle, const CEnginePrimitiveLine& line, unsigned long f)
    {
        handleIds[count] = handle.id; flags[count] = f; lines[count] = line; ++count;
    }
};
class CDisplayEngine { public: CIEngine* Engine3D; CIEngine& Get3DEngine() { return *Engine3D; } };
class CMainGameComponent { public:
    CDisplayEngine* PDisplayEngine;
    static CMainGameComponent* PMainGameComponent;
    static CMainGameComponent* Get() { return PMainGameComponent; }
    CDisplayEngine& GetDisplayEngine() { return *PDisplayEngine; }
};
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;
class CThingManager { public:
    bool DrawCombatCollisionDebug;
    bool IsDrawCombatCollisionDebug() const { return DrawCombatCollisionDebug; }
};
class CThing { public:
    int kills; bool lastArg;
    CThing() : kills(0), lastArg(false) { }
    void Kill(bool b) { ++kills; lastArg = b; }
};
class CTCDPhysicalObstruction { public:
    CThing* ParentThing;
    CThingManager* manager;
    C3DVector StartPos, EndPos;
    long LifeTimer;
    CEnginePrimitiveHandle line1, line2, line3;
    int impulses;
    CThingManager& GetThingManager() { return *manager; }
    void UpdateAdditionalImpulse() { ++impulses; }
    void FrameUpdate()
    {
        if (GetThingManager().IsDrawCombatCollisionDebug()) {
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line1,
                CEnginePrimitiveLine(0, StartPos, StartPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0), 0);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line2,
                CEnginePrimitiveLine(0, EndPos, EndPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0), 0);
            CMainGameComponent::Get()->GetDisplayEngine().Get3DEngine().SetPersistentPrimitive(line3,
                CEnginePrimitiveLine(0, StartPos + C3DVector(0.0f, 0.0f, 2.0f), EndPos + C3DVector(0.0f, 0.0f, 2.0f), CRGBColour(0, 0, 255, 255), 0.05f, 0, 0, true, 0, 0), 0);
        }
        UpdateAdditionalImpulse();
        if (LifeTimer > 0) {
            if (--LifeTimer == 0)
                ParentThing->Kill(true);
        }
    }
};
static bool Same(const C3DVector& v, float x, float y, float z) { return v.X == x && v.Y == y && v.Z == z; }
static bool Blue(const CEnginePrimitiveLine& l)
{
    return l.Colour.R == 0 && l.Colour.G == 0 && l.Colour.B == 255 && l.Colour.A == 255 && l.Width == 0.05f && l.UseZBuffer &&
           l.Id == 0 && l.BankIndex == 0 && l.BlendMode == 0 && l.BlendOp == 0 && l.InterpolationSet == 0;
}
int main()
{
    CIEngine engine; CDisplayEngine display; display.Engine3D = &engine;
    CMainGameComponent game; game.PDisplayEngine = &display; CMainGameComponent::PMainGameComponent = &game;
    CThingManager manager; CThing parent;
    CTCDPhysicalObstruction o;
    o.ParentThing = &parent; o.manager = &manager; o.impulses = 0;
    o.StartPos = C3DVector(1.0f, 2.0f, 3.0f); o.EndPos = C3DVector(10.0f, 20.0f, 30.0f);
    o.line1.id = 11; o.line2.id = 22; o.line3.id = 33;
    bool ok = true;
    // Debug draw off: no lines, impulse still runs, timer 0 stays 0 and nothing is killed.
    manager.DrawCombatCollisionDebug = false; o.LifeTimer = 0;
    o.FrameUpdate();
    ok = ok && engine.count == 0 && o.impulses == 1 && o.LifeTimer == 0 && parent.kills == 0;
    // A negative timer is left alone too.
    o.LifeTimer = -5; o.FrameUpdate();
    ok = ok && o.LifeTimer == -5 && parent.kills == 0 && o.impulses == 2;
    // Debug draw on, timer 2: three lines, timer 1, no kill.
    manager.DrawCombatCollisionDebug = true; o.LifeTimer = 2;
    o.FrameUpdate();
    ok = ok && engine.count == 3 && o.impulses == 3 && o.LifeTimer == 1 && parent.kills == 0;
    if (engine.count == 3) {
        ok = ok && engine.handleIds[0] == 11 && engine.handleIds[1] == 22 && engine.handleIds[2] == 33;
        ok = ok && engine.flags[0] == 0 && engine.flags[1] == 0 && engine.flags[2] == 0;
        ok = ok && Same(engine.lines[0].Start, 1.0f, 2.0f, 3.0f) && Same(engine.lines[0].End, 1.0f, 2.0f, 5.0f);
        ok = ok && Same(engine.lines[1].Start, 10.0f, 20.0f, 30.0f) && Same(engine.lines[1].End, 10.0f, 20.0f, 32.0f);
        ok = ok && Same(engine.lines[2].Start, 1.0f, 2.0f, 5.0f) && Same(engine.lines[2].End, 10.0f, 20.0f, 32.0f);
        ok = ok && Blue(engine.lines[0]) && Blue(engine.lines[1]) && Blue(engine.lines[2]);
    }
    // Timer 1 -> 0: the parent is killed once, with `true`.
    manager.DrawCombatCollisionDebug = false;
    o.FrameUpdate();
    ok = ok && engine.count == 3 && o.LifeTimer == 0 && parent.kills == 1 && parent.lastArg && o.impulses == 4;
    // And not again on the next frame.
    o.FrameUpdate();
    ok = ok && parent.kills == 1 && o.LifeTimer == 0 && o.impulses == 5;
    if (!ok) { std::printf("007fd650_TEST FAIL\n"); return 1; }
    std::printf("007fd650_TEST PASS\n");
    return 0;
}