// Behaviour test for 0x0093D270 (parked row): the row's logic over a flat model.
#include <cstdio>

struct V { float X, Y; };
static bool Navigable(const V& a, const V& b, float prox) { float dx=a.X-b.X, dy=a.Y-b.Y; return dx*dx+dy*dy <= prox*prox*100.0f; }
struct Enemy { bool Dead; V Pos; };
struct State {
    Enemy* Best; Enemy* PEnemy; bool StillRunnable; V SelfPos; float Proximity;
    bool IsRunnable() {
        Enemy* enemy = Best;
        if (enemy != 0 && !enemy->Dead) {
            PEnemy = enemy;
            if (StillRunnable && Navigable(SelfPos, PEnemy->Pos, Proximity)) return true;
        }
        return false;
    }
};
int main() {
    Enemy near = { false, { 1, 1 } }, far = { false, { 100, 0 } }, dead = { true, { 0, 0 } };
    State s = { &near, 0, true, { 0, 0 }, 1.0f };
    bool ok = s.IsRunnable() && s.PEnemy == &near;
    s.Best = &far; ok = ok && !s.IsRunnable() && s.PEnemy == &far;
    s.Best = &dead; s.PEnemy = 0; ok = ok && !s.IsRunnable() && s.PEnemy == 0;
    s.Best = 0; ok = ok && !s.IsRunnable();
    s.Best = &near; s.StillRunnable = false; ok = ok && !s.IsRunnable();
    std::printf(ok ? "0093d270_TEST PASS\n" : "0093d270_TEST FAIL\n");
    return ok ? 0 : 1;
}
