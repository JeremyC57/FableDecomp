#include <cstdio>
// behaviour test: the CTCDiceGame constructor links the parent, points each rule holder at its own inline storage, seeds the
// pair with (0, seed), zeroes the quads and the two trailing words, then allocates the game state from the setup value and
// the controller, both pointing back at the game
struct Rules { int N; Rules(int n) : N(n) { } };
struct Holder { Rules* P; Rules Storage; Holder() : P(&Storage), Storage(0) { } };
struct Quad { short V[4]; Quad() { V[0] = V[1] = V[2] = V[3] = 0; } };
struct Game;
struct State { Game* G; int Value; State(Game* g, int v) : G(g), Value(v) { } };
struct Controller { Game* G; Controller(Game* g) : G(g) { } };
struct Game {
    void* Parent; State* S; Controller* C; Holder A, B; int SeedA, SeedB; Quad Q[3]; int F9C, FA0;
    Game(void* parent, const int* setup, int seed) : Parent(parent), S(0), C(0), SeedA(0), SeedB(seed), F9C(0), FA0(0)
    {
        S = new State(this, *setup);
        C = new Controller(this);
    }
};
int main()
{
    int parent = 0, setup = 42;
    Game g(&parent, &setup, 7);
    bool ok = g.Parent == &parent && g.A.P == &g.A.Storage && g.B.P == &g.B.Storage && g.A.Storage.N == 0 && g.SeedA == 0
              && g.SeedB == 7 && g.Q[2].V[3] == 0 && g.F9C == 0 && g.FA0 == 0 && g.S->G == &g && g.S->Value == 42 && g.C->G == &g;
    delete g.S; delete g.C;
    if (!ok) { std::printf("00c14e40_TEST FAIL\n"); return 1; }
    std::printf("00c14e40_TEST PASS\n");
    return 0;
}