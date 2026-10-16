// Behaviour test for CTCTavernGameCardBase::TurnAllCards (0x008F09E0): every card plays the flip sound, starts turning from
// 0.5 to 0 (face down) or 0 to 0.5 (face up) and flips its face; state 22 at the end.
#include <cstdio>
struct Card { float Initial, Final, Fraction; bool Turning, FaceDown; };
static int g_sounds, g_state;
static void TurnAll(Card** b, Card** e)
{
    for (Card** it = b; it != e; ++it) {
        ++g_sounds;
        Card* c = *it;
        c->Turning = true; c->Fraction = 0.0f;
        if (c->FaceDown == true) { c->Initial = 0.5f; c->Final = 0.0f; } else { c->Initial = 0.0f; c->Final = 0.5f; }
        c->FaceDown = !c->FaceDown;
    }
    g_state = 22;
}
int main()
{
    Card a = { 9, 9, 9, false, true }, b = { 9, 9, 9, false, false };
    Card* v[2] = { &a, &b };
    TurnAll(v, v + 2);
    if (g_sounds != 2 || g_state != 22 || a.Initial != 0.5f || a.Final != 0 || a.FaceDown || !a.Turning ||
        b.Initial != 0 || b.Final != 0.5f || !b.FaceDown || b.Fraction != 0) { std::printf("008f09e0_TEST FAIL\n"); return 1; }
    std::printf("008f09e0_TEST PASS\n");
    return 0;
}
