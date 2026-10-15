// Behaviour test for 0x0068d700 (CInputProcessFirstPersonLookAround::ProcessInput): the reconstructed logic run over a host model of its callees.
#include <cstdio>
#include <cstring>
#include <cmath>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct CInputEvent { long kind; long type; float x, y; };
struct CGameEvent { long Type; signed char Player; unsigned char Data[32]; unsigned char EndPos; bool Valid; bool Replacement;
    unsigned char used;
    CGameEvent(long t) : Type(t), Player(-1), EndPos(0), Valid(true), Replacement(false), used(0) {}
    void Copy(unsigned int n, const void* p) { std::memcpy(Data + used, p, n); used = (unsigned char)(used + n); } };
struct CProcessedInput { int handled; int events; long lastType; signed char lastPlayer; unsigned char lastData[32];
    CProcessedInput() : handled(0), events(0), lastType(0), lastPlayer(0) {}
    void SetHandled() { ++handled; }
    void AddGameEvent(const CGameEvent& e) { ++events; lastType = e.Type; lastPlayer = e.Player; std::memcpy(lastData, e.Data, 32); } };
struct CPlayer { long removed; long mode; CPlayer() : removed(0), mode(0) {} void RemoveMode(long m) { removed = m; } bool HasMode(long m) { return mode == m; } };
// predicate stubs: event kinds 0x13 pressed, 0x15 held; type = the game action
struct CInputProcessGameBase {
    CPlayer* MyPlayer;
    bool IsEventButtonPressed(const CInputEvent& e) { return e.kind == 0x13; }
    bool IsEventButtonHeld(const CInputEvent& e) { return e.kind == 0x15; }
    bool IsEventGameAction(long a, const CInputEvent& e) { return e.type == a; }
};

struct C2DVector { float x, y; };
static void PreProcessStickVector(C2DVector* v) { v->x *= 0.5f; v->y *= 0.5f; }
struct CGameCameraManager { C2DVector look; int sets; void SetFirstPersonLookAroundVector(const C2DVector& v) { look = v; ++sets; } };
struct CCamera { CGameCameraManager* mgr; bool GetCameraManager(CGameCameraManager** out) { *out = mgr; return true; } };
struct CWorld { CCamera* cam; };
struct CInputProcessFirstPersonLookAround : CInputProcessGameBase {
    CWorld* World;
    bool GetEventGameActionVector(long a, const CInputEvent& e, C2DVector* v) { if (e.type != a) return false; v->x = e.x; v->y = e.y; return true; }
    void ProcessInput(const CInputEvent& event, CProcessedInput&)
    {
        C2DVector look;
        if (!GetEventGameActionVector(0x4b, event, &look))
            return;
        if (event.kind == 0xd) { look.x += look.x; look.y += look.y; }
        else if (event.kind == 0x11) PreProcessStickVector(&look);
        CCamera* source = World->cam;
        CGameCameraManager* camera = 0;
        source->GetCameraManager(&camera);
        camera->SetFirstPersonLookAroundVector(look);
    }
};
static void Run()
{
    CGameCameraManager m; m.sets = 0; CCamera c; c.mgr = &m; CWorld w; w.cam = &c;
    CInputProcessFirstPersonLookAround p; p.World = &w; CProcessedInput out;
    CInputEvent other = { 0xd, 0x4a, 1, 1 }; p.ProcessInput(other, out); CHECK(m.sets == 0);
    CInputEvent mouse = { 0xd, 0x4b, 1.5f, -2 }; p.ProcessInput(mouse, out); CHECK(m.sets == 1 && m.look.x == 3.0f && m.look.y == -4.0f);
    CInputEvent stick = { 0x11, 0x4b, 1, 2 }; p.ProcessInput(stick, out); CHECK(m.sets == 2 && m.look.x == 0.5f && m.look.y == 1.0f);
    CInputEvent plain = { 0x5, 0x4b, 7, 8 }; p.ProcessInput(plain, out); CHECK(m.sets == 3 && m.look.x == 7.0f && m.look.y == 8.0f);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0068d700_TEST FAIL\n"); return 1; }
    std::printf("0068d700_TEST PASS\n");
    return 0;
}
