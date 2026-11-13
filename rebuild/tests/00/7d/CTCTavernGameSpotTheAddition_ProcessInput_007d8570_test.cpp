// Behaviour test for 0x007D8570 CTCTavernGameSpotTheAddition::ProcessInput: a host copy of the row's logic over plain models
// (the tavern game, its two defs, the input process and the processed-input flag), checked state by state.
#include <cstdio>
#include <cstring>
#include <cmath>

enum { SETUP = 39, ADD = 40, WATCH = 41, SELECT = 42, END_ROUND = 6, QUIT_MID_GAME = 11, GREETING = 1 };
enum { ACT_YES = 22, ACT_THIRD = 24, ACT_MOVE = 79, ACT_QUIT = 83 };
enum { WIN_PHRASE = 1, LOSE_PHRASE = 2, CONTINUE_TEXT = 3, WIN_ROUND_PHRASE = 4, QUIT_TEXT = 5, YES_TEXT = 6, NO_TEXT = 7 };

struct Vec { float X, Y, Z; };
static Vec Sub(const Vec& a, const Vec& b) { Vec r; r.X = a.X - b.X; r.Y = a.Y - b.Y; r.Z = a.Z - b.Z; return r; }
static Vec Cross(const Vec& a, const Vec& b) { Vec r; r.X = a.Y * b.Z - a.Z * b.Y; r.Y = a.Z * b.X - a.X * b.Z; r.Z = a.X * b.Y - a.Y * b.X; return r; }
static float Dot(const Vec& a, const Vec& b) { float d = a.Y * b.Y; d += a.Z * b.Z; d += a.X * b.X; return d; }

struct Thing { Vec Pos; bool HasAppearance; int HighlightKind; };     // 0 none, 1 error colour, 2 normal colour

struct Input {                       // the event as the input process reports it
    int Action;                      // game action of the event (0 = none)
    bool Pressed;
    float MoveX, MoveY;
};

struct Game {
    int State, CurrentRound, ItemsPerRound;
    bool GameWon;
    Thing* Pointer; Thing* NewKnickKnack; Thing* Target;
    Vec Up, Forward, GamePos;
    float ImpulseScale, MinX, MaxX, MinY, MaxY;
    // effects
    int SetStateTo, OkLeft, OkRight, OkState, YesNoText, YesNoYesState, YesNoNoState;
    bool Performed, BaseCalled, SubtitlesCleared;
    const char* Sound;
    Vec MeshOffset; bool MeshPlaced;
};

static void Reset(Game& g)
{
    std::memset(&g, 0, sizeof(g));
    g.Up.Z = 1.0f; g.Forward.Y = 1.0f;
    g.ImpulseScale = 0.5f; g.MinX = -2.0f; g.MaxX = 2.0f; g.MinY = -1.0f; g.MaxY = 1.0f;
    g.ItemsPerRound = 3;
    g.SetStateTo = g.OkState = g.YesNoYesState = g.YesNoNoState = -1;
}

static void Highlight(Thing* t, int kind) { if (t && t->HasAppearance) t->HighlightKind = kind; }

// The row's logic (same order of tests and effects as 0x007D8570).
static void ProcessInput(Game& g, const Input& in)
{
    switch (g.State) {
    case SETUP:
    case ADD:
        break;
    case WATCH:
        g.SetStateTo = ADD;
        g.Performed = true;
        break;
    case SELECT:
        if (in.Action == ACT_MOVE) {
            Vec offset = Sub(g.Pointer->Pos, g.GamePos);
            Vec position;
            position.X = Dot(offset, Cross(g.Forward, g.Up)) + in.MoveX * g.ImpulseScale;
            position.Y = Dot(offset, g.Forward) + in.MoveY * g.ImpulseScale;
            position.Z = 0.0f;
            if (position.X < g.MinX) position.X = g.MinX; else if (position.X > g.MaxX) position.X = g.MaxX;
            if (position.Y < g.MinY) position.Y = g.MinY; else if (position.Y > g.MaxY) position.Y = g.MaxY;
            g.MeshOffset = position; g.MeshPlaced = true;
            g.Performed = true;
        }
        if (g.Target && in.Pressed && in.Action == ACT_YES) {
            if (g.Target != g.NewKnickKnack) {
                g.Sound = "TAVERN_GAME_LOSE_GAME";
                Highlight(g.Target, 1);
                g.OkLeft = LOSE_PHRASE; g.OkRight = CONTINUE_TEXT; g.OkState = END_ROUND;
            } else if (g.CurrentRound + 1 == g.ItemsPerRound) {
                g.GameWon = true;
                g.Sound = "TAVERN_GAME_WIN_GAME";
                g.OkLeft = WIN_PHRASE; g.OkRight = CONTINUE_TEXT; g.OkState = END_ROUND;
            } else {
                g.Sound = "TAVERN_GAME_CORRECT";
                g.OkLeft = WIN_ROUND_PHRASE; g.OkRight = CONTINUE_TEXT; g.OkState = SETUP;
            }
            if (g.NewKnickKnack) Highlight(g.NewKnickKnack, 2);
            g.Performed = true;
        }
        break;
    default:
        g.BaseCalled = true;
        break;
    }
    if (g.State >= SETUP && g.State <= SELECT) {
        if (in.Action == ACT_THIRD || in.Action == ACT_QUIT) {
            g.SubtitlesCleared = true;
            g.YesNoText = QUIT_TEXT; g.YesNoYesState = QUIT_MID_GAME; g.YesNoNoState = g.State;
            g.Performed = true;
        }
    }
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-5f; }

int main()
{
    int fails = 0;
    Game g; Input in; Thing pointer, a, b;
    // watch state: skip to the add state
    Reset(g); std::memset(&in, 0, sizeof(in)); g.State = WATCH;
    ProcessInput(g, in);
    if (g.SetStateTo != ADD || !g.Performed || g.BaseCalled) ++fails;
    // setup state with no button: nothing at all
    Reset(g); g.State = SETUP; ProcessInput(g, in);
    if (g.Performed || g.BaseCalled || g.SetStateTo != -1) ++fails;
    // other state: base game, and no quit prompt outside the addition states
    Reset(g); g.State = GREETING; in.Action = ACT_QUIT; ProcessInput(g, in);
    if (!g.BaseCalled || g.SubtitlesCleared || g.Performed) ++fails;
    // select + movement: right = Forward x Up = (1,0,0); offset (3,0.4,0) + stick (2,-4)*0.5 -> (4,-1.6) clamped to (2,-1)
    Reset(g); std::memset(&pointer, 0, sizeof(pointer)); g.Pointer = &pointer; pointer.Pos.X = 3.0f; pointer.Pos.Y = 0.4f;
    g.State = SELECT; in.Action = ACT_MOVE; in.MoveX = 2.0f; in.MoveY = -4.0f; in.Pressed = false;
    ProcessInput(g, in);
    if (!g.MeshPlaced || !Near(g.MeshOffset.X, 2.0f) || !Near(g.MeshOffset.Y, -1.0f) || g.MeshOffset.Z != 0.0f || !g.Performed) ++fails;
    // inside the bounds: unclamped
    Reset(g); g.Pointer = &pointer; pointer.Pos.X = 0.5f; pointer.Pos.Y = 0.25f; g.State = SELECT; in.MoveX = 0.2f; in.MoveY = 0.2f;
    ProcessInput(g, in);
    if (!Near(g.MeshOffset.X, 0.6f) || !Near(g.MeshOffset.Y, 0.35f)) ++fails;
    // wrong pick: lose sound, error highlight on the pick, normal highlight on the new piece, end of round
    Reset(g); std::memset(&a, 0, sizeof(a)); std::memset(&b, 0, sizeof(b)); a.HasAppearance = b.HasAppearance = true;
    g.State = SELECT; g.Target = &a; g.NewKnickKnack = &b; in.Action = ACT_YES; in.Pressed = true;
    ProcessInput(g, in);
    if (std::strcmp(g.Sound, "TAVERN_GAME_LOSE_GAME") || a.HighlightKind != 1 || b.HighlightKind != 2 || g.OkLeft != LOSE_PHRASE
        || g.OkState != END_ROUND || g.GameWon) ++fails;
    // right pick, not the last round: correct sound, back to setup
    Reset(g); b.HighlightKind = 0; g.State = SELECT; g.Target = &b; g.NewKnickKnack = &b; g.CurrentRound = 0;
    ProcessInput(g, in);
    if (std::strcmp(g.Sound, "TAVERN_GAME_CORRECT") || g.OkLeft != WIN_ROUND_PHRASE || g.OkState != SETUP || g.GameWon
        || b.HighlightKind != 2) ++fails;
    // right pick in the last round: game won
    Reset(g); g.State = SELECT; g.Target = &b; g.NewKnickKnack = &b; g.CurrentRound = 2;
    ProcessInput(g, in);
    if (std::strcmp(g.Sound, "TAVERN_GAME_WIN_GAME") || !g.GameWon || g.OkLeft != WIN_PHRASE || g.OkState != END_ROUND) ++fails;
    // released button: no judgement
    Reset(g); g.State = SELECT; g.Target = &b; g.NewKnickKnack = &b; in.Pressed = false;
    ProcessInput(g, in);
    if (g.Sound || g.Performed) ++fails;
    // quit while a round runs: yes -> quit mid game, no -> back to the current state
    Reset(g); g.State = ADD; in.Action = ACT_THIRD; ProcessInput(g, in);
    if (!g.SubtitlesCleared || g.YesNoText != QUIT_TEXT || g.YesNoYesState != QUIT_MID_GAME || g.YesNoNoState != ADD || !g.Performed) ++fails;
    if (fails) { std::printf("007d8570_TEST FAIL\n"); return 1; }
    std::printf("007d8570_TEST PASS\n");
    return 0;
}