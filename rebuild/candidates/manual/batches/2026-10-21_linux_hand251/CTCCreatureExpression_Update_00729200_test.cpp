#include <cstdio>
// behaviour test: Update advances every state by the frame time, removes non-primary states with no combination anim or with
// both weights zero (removing the anim), then -- if states remain -- clears the type-1 anims and, outside movie sequences,
// re-adds one type-0x12 anim per state with alpha = weight, last alpha = old weight and frame (1, 1)
struct SState { int AnimIndex; float Weight, OldWeight; bool Primary; bool HasAnim; };
struct SAdded { int anim; float alpha, last, f0, f1; };
static int g_updates, g_removedAnims, g_cleared; static SAdded g_added[8]; static int g_nAdded;
static int Update(SState* states, int n, float dt, bool movie)
{
    for (int i = 0; i < n; ++i) ++g_updates;
    (void)dt;
    int out = 0;
    for (int i = 0; i < n; ++i) {
        bool remove = !states[i].Primary && (!states[i].HasAnim || (states[i].Weight == 0.0f && states[i].OldWeight == 0.0f));
        if (remove) ++g_removedAnims; else states[out++] = states[i];
    }
    if (out) {
        ++g_cleared;
        if (!movie)
            for (int i = 0; i < out; ++i) {
                SAdded a = { states[i].AnimIndex, states[i].Weight, states[i].OldWeight, 1.0f, 1.0f };
                g_added[g_nAdded++] = a;
            }
    }
    return out;
}
int main()
{
    SState s[4] = { { 1, 0.0f, 0.0f, true, false }, { 2, 0.0f, 0.0f, false, true }, { 3, 0.5f, 0.0f, false, true },
                    { 4, 0.5f, 0.5f, false, false } };
    int n = Update(s, 4, 0.1f, false);
    bool ok = n == 2 && g_updates == 4 && g_removedAnims == 2 && g_cleared == 1 && g_nAdded == 2 && g_added[0].anim == 1 &&
              g_added[1].anim == 3 && g_added[1].alpha == 0.5f && g_added[1].f0 == 1.0f && g_added[1].f1 == 1.0f;
    n = Update(s, n, 0.1f, true);
    ok = ok && n == 2 && g_cleared == 2 && g_nAdded == 2;
    SState t[1] = { { 5, 0.0f, 0.0f, false, true } };
    n = Update(t, 1, 0.1f, false);
    ok = ok && n == 0 && g_cleared == 2;
    if (!ok) { std::printf("00729200_TEST FAIL\n"); return 1; }
    std::printf("00729200_TEST PASS\n");
    return 0;
}
