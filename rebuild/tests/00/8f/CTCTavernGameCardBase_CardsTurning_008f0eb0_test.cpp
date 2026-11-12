#include <cstdio>
#include <vector>
#include <cmath>
// Behaviour model of CTCTavernGameCardBase::CardsTurning (0x008F0EB0).
struct V3 { float X, Y, Z; V3() {} V3(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Card { float MoveFraction; bool Turning; bool Moving; V3 LocalPos, NewLocalPos, CurrentPos;
    void Move(const V3& p, bool imm) {
        if (imm) { LocalPos = p; NewLocalPos = p; return; }
        if (Moving) LocalPos = CurrentPos; else { CurrentPos = LocalPos; Moving = true; }
        NewLocalPos = p; MoveFraction = 0.0f; } };
struct Def { float CardSeparation, TableSeparation; };
struct Game { int State; int sounds; std::vector<Card*> Cards; Def def;
    void SetState(int s) { State = s; }
    void CardsTurning() {
        for (size_t k = 0; k < Cards.size(); ++k) if (Cards[k]->Turning == true) return;
        if (State == 0x16) SetState(0x17);
        else if (State == 0x19) {
            float x = 0.0f, y = 0.0f;
            for (size_t k = 0; k < Cards.size(); ++k) { x += Cards[k]->CurrentPos.X; y += Cards[k]->CurrentPos.Y; }
            x /= (float)Cards.size(); y /= (float)Cards.size();
            int i = 0;
            for (size_t k = 0; k < Cards.size(); ++k, ++i) { ++sounds;
                Cards[k]->Move(V3(x, y, (float)(53 - i) * def.CardSeparation + def.TableSeparation), false); }
            SetState(0x1a); return;
        } else SetState(0);
    } };
static bool eq(float a, float b) { return std::fabs(a - b) < 1e-4f; }
int main()
{
    Card a = {}, b = {};
    a.CurrentPos = V3(1, 2, 0); a.LocalPos = V3(9, 9, 9); a.Moving = true;
    b.CurrentPos = V3(3, 6, 0); b.LocalPos = V3(5, 5, 5); b.Moving = false;
    Game g; g.State = 0x19; g.sounds = 0; g.Cards.push_back(&a); g.Cards.push_back(&b); g.def.CardSeparation = 0.5f; g.def.TableSeparation = 1.0f;
    a.Turning = true; g.CardsTurning();
    if (g.State != 0x19 || g.sounds != 0) { std::printf("008f0eb0_TEST FAIL\n"); return 1; }
    a.Turning = false; g.CardsTurning();
    bool ok = g.State == 0x1a && g.sounds == 2 && eq(a.NewLocalPos.X, 2) && eq(a.NewLocalPos.Y, 4) && eq(a.NewLocalPos.Z, 27.5f)
        && eq(b.NewLocalPos.Z, 27.0f) && eq(a.LocalPos.X, 1) && b.Moving && eq(b.CurrentPos.X, 5) && a.MoveFraction == 0.0f;
    g.State = 0x16; g.CardsTurning(); ok = ok && g.State == 0x17;
    g.State = 3; g.CardsTurning(); ok = ok && g.State == 0;
    if (!ok) { std::printf("008f0eb0_TEST FAIL\n"); return 1; }
    std::printf("008f0eb0_TEST PASS\n");
    return 0;
}