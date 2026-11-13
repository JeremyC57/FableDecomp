// Behaviour test for 0x007D6870 CTCTavernGameCardPairs::ProcessInput (pairs card game input).
// A plain model of the row's logic (compiled apart from the row source): the TU switch hands everything to the card base;
// otherwise MAIN_GAME handles select / quit, TURNING_UP / MAIN_GAME / TURNING_BACK run the movement block, other states go to
// the base game. The movement block projects the pointer offset on the set's right / forward, picks the direction, clamps,
// finds the nearest other card and moves there unless the cards are aligned across the stick or the move points backwards.
#include <cstdio>

struct Vec3 { float X, Y, Z; };
static Vec3 V(float x, float y, float z) { Vec3 v; v.X = x; v.Y = y; v.Z = z; return v; }
static float Dot(const Vec3& a, const Vec3& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
static Vec3 Sub(const Vec3& a, const Vec3& b) { return V(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static Vec3 Cross(const Vec3& a, const Vec3& b) { return V(a.Y * b.Z - a.Z * b.Y, a.Z * b.X - a.X * b.Z, a.X * b.Y - a.Y * b.X); }
static float Abs(float f) { unsigned long b = *(unsigned long*)&f; b &= 0x7fffffff; return *(float*)&b; }
static const float EPS = 1e-4f;

struct Card { Vec3 LocalPos; float Width, Height; bool FaceDown, Turning; int turned; };

enum { TURNING_UP = 22, MAIN_GAME = 23, TURNING_BACK = 25 };
enum { CALL_NONE, CALL_CARD_BASE, CALL_GAME_BASE };

struct Game {
    bool use_card_base; int state; bool select_result; bool game_won; Card* selected;
    Vec3 up, forward, game_pos, pointer_pos; int control_type; float impulse, height_offset;
    float min_x, max_x, min_y, max_y;
    Card* cards[8]; int ncards; Card* highlighted;
    long prev_dir, frames_repeated, last_moved, frame;
    int base_call; int sounds; int ok_state; int yes_no_state; int performed; Card* moved_to;
};

static void ProcessInput(Game& g, bool pressed, bool yes, bool quit, bool moving, float mx, float my)
{
    if (g.use_card_base) { g.base_call = CALL_CARD_BASE; return; }
    switch (g.state) {
    case MAIN_GAME:
        if (pressed) {
            if (yes) {
                if (!g.select_result) { g.sounds += g.game_won ? 1 : 10; g.ok_state = 24; }
                else if (g.selected && !g.selected->FaceDown && !g.selected->Turning) { g.sounds += 100; g.selected->turned++; }
                g.performed++;
            }
            if (quit) { g.yes_no_state = 11; g.performed++; }
        }
    case TURNING_UP:
    case TURNING_BACK:
        if (moving) {
            if (mx == 0.0f && my == 0.0f) { g.frames_repeated = 0; return; }
            Vec3 offset = V(0, 0, 0);
            if (g.control_type == 0) offset = Sub(g.pointer_pos, g.game_pos);
            else if (g.control_type == 1) offset = V(0.0f, 0.0f, g.height_offset);
            Vec3 pos = V(Dot(offset, Cross(g.forward, g.up)) + mx * g.impulse, Dot(offset, g.forward) - my * g.impulse, 0.0f);
            float dx = (g.highlighted->LocalPos.X - pos.X) / g.highlighted->Width;
            float dy = (g.highlighted->LocalPos.Y - pos.Y) / g.highlighted->Height;
            long dir;
            if (Abs(dx) > Abs(dy)) dir = dx > 0.0f ? 0 : 1; else dir = dy > 0.0f ? 2 : 3;
            if (g.prev_dir != dir) g.frames_repeated = 0;
            g.prev_dir = dir; g.last_moved = g.frame;
            if (pos.X < g.min_x) pos.X = g.min_x; else if (pos.X > g.max_x) pos.X = g.max_x;
            if (pos.Y < g.min_y) pos.Y = g.min_y; else if (pos.Y > g.max_y) pos.Y = g.max_y;
            float best_d = 0.0f; Card* best = 0;
            for (int i = 0; i < g.ncards; ++i) {
                Card* c = g.cards[i];
                if (c == g.highlighted) continue;
                float cx = (c->LocalPos.X - pos.X) / c->Width, cy = (c->LocalPos.Y - pos.Y) / c->Height;
                float d = cx * cx + cy * cy;
                if (best_d == 0.0f || best_d > d) { best_d = d; best = c; }
            }
            if ((Abs(g.highlighted->LocalPos.X - best->LocalPos.X) < EPS && Abs(my) < EPS)
                || (Abs(g.highlighted->LocalPos.Y - best->LocalPos.Y) < EPS && Abs(mx) < EPS)) break;
            if (Dot(Sub(pos, g.highlighted->LocalPos), Sub(pos, best->LocalPos)) > 0.0f) return;
            g.moved_to = best; g.performed++;
        }
        break;
    default:
        g.base_call = CALL_GAME_BASE;
        break;
    }
}

static Card cards[4];
static void Setup(Game& g)
{
    Game z = Game(); g = z;
    g.state = MAIN_GAME; g.select_result = true;
    g.up = V(0, 0, 1); g.forward = V(0, 1, 0); g.game_pos = V(0, 0, 0); g.pointer_pos = V(0, 0, 0);
    g.impulse = 1.0f; g.min_x = -10; g.max_x = 10; g.min_y = -10; g.max_y = 10; g.frame = 77; g.prev_dir = 5; g.frames_repeated = 9;
    for (int i = 0; i < 4; ++i) { Card c = Card(); cards[i] = c; cards[i].Width = 1.0f; cards[i].Height = 1.0f; g.cards[i] = &cards[i]; }
    cards[0].LocalPos = V(0, 0, 0); cards[1].LocalPos = V(2, 0, 0); cards[2].LocalPos = V(0, 2, 0); cards[3].LocalPos = V(-2, 0, 0);
    g.ncards = 4; g.highlighted = &cards[0];
}

int main()
{
    int fail = 0; Game g;
    Setup(g); g.use_card_base = true; ProcessInput(g, true, true, true, true, 1, 0);
    if (g.base_call != CALL_CARD_BASE || g.performed) fail |= 1;
    Setup(g); g.state = 5; ProcessInput(g, true, true, true, true, 1, 0);
    if (g.base_call != CALL_GAME_BASE) fail |= 2;
    Setup(g); g.select_result = false; g.game_won = true; ProcessInput(g, true, true, false, false, 0, 0);
    if (g.sounds != 1 || g.ok_state != 24 || g.performed != 1) fail |= 4;
    Setup(g); g.selected = &cards[1]; ProcessInput(g, true, true, true, false, 0, 0);
    if (cards[1].turned != 1 || g.sounds != 100 || g.yes_no_state != 11 || g.performed != 2) fail |= 8;
    Setup(g); g.selected = &cards[1]; cards[1].FaceDown = true; ProcessInput(g, true, true, false, false, 0, 0);
    if (cards[1].turned != 0) fail |= 16;
    Setup(g); ProcessInput(g, true, false, false, true, 0, 0);                    // stick centred: reset repeat count
    if (g.frames_repeated != 0 || g.moved_to) fail |= 32;
    Setup(g); g.state = TURNING_UP; ProcessInput(g, false, false, false, true, 1.5f, 0.1f);   // right: nearest card 1
    if (g.moved_to != &cards[1] || g.prev_dir != 1 || g.last_moved != 77 || g.frames_repeated != 0) fail |= 64;
    Setup(g); g.state = TURNING_BACK; ProcessInput(g, false, false, false, true, -1.5f, 0.1f); // left: card 3
    if (g.moved_to != &cards[3] || g.prev_dir != 0) fail |= 128;
    Setup(g); ProcessInput(g, false, false, false, true, 0.1f, -1.5f);           // forward is +Y: stick -Y moves up
    if (g.moved_to != &cards[2] || g.prev_dir != 3) fail |= 256;
    Setup(g); ProcessInput(g, false, false, false, true, 0.0f, 1.5f);            // aligned column, no X stick: no move
    if (g.moved_to) fail |= 512;
    Setup(g); g.max_x = 0.5f; ProcessInput(g, false, false, false, true, 5.0f, 0.2f);   // clamped, still nearest card 1
    if (g.moved_to != &cards[1]) fail |= 1024;
    Setup(g); cards[1].LocalPos = V(-1, -1, 0); cards[2].LocalPos = V(5, 5, 0); cards[3].LocalPos = V(6, 6, 0);
    ProcessInput(g, false, false, false, true, 0.6f, 0.3f);                       // nearest lies behind: dot > 0, no move
    if (g.moved_to || g.performed) fail |= 2048;
    // mutation checks: the model must notice a wrong direction choice and a wrong nearest card
    Setup(g); ProcessInput(g, false, false, false, true, 1.5f, 0.1f);
    if (g.prev_dir == 0 || g.moved_to == &cards[3]) fail |= 4096;
    if (fail) { std::printf("007d6870_TEST FAIL %d\n", fail); return 1; }
    std::printf("007d6870_TEST PASS\n");
    return 0;
}