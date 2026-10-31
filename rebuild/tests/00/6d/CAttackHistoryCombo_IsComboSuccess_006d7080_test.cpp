// behaviour test for 0x006D7080 CAttackHistoryCombo::IsComboSuccess
#include <cstdio>
enum { SAME = 0, DIFF = 1, SPEC = 2, NONE_ATTR = -1 };
struct Step { int type; int attr; };
// history[0..n) oldest..newest; combo[0..m) checked newest step first (combo walked backwards)
static bool IsComboSuccess(const int* history, int n, const Step* combo, int m) {
    if (n == 0) return false;
    int idx = n - 1;
    int attack = history[idx--];
    for (int s = m; s != 0; --s) {
        if (attack == NONE_ATTR) return false;
        int type = combo[s - 1].type;
        if (type == SPEC && attack != combo[s - 1].attr) return false;
        if (idx >= 0) {
            int previous = history[idx];
            if (previous != NONE_ATTR) {
                if (type == DIFF) { if (attack == previous) return false; }
                else if (type == SAME) { if (attack != previous) return false; }
                attack = history[idx--];
                continue;
            }
        }
        attack = NONE_ATTR;
    }
    return true;
}
int main() {
    int h1[] = {1, 2, 3};
    Step c1[] = {{SPEC, 1}, {DIFF, 0}, {SPEC, 3}};
    Step c2[] = {{SAME, 0}, {SPEC, 3}};
    Step c3[] = {{SPEC, 0}, {SPEC, 0}, {SPEC, 0}, {SPEC, 0}};
    bool ok = IsComboSuccess(h1, 3, c1, 3) && !IsComboSuccess(h1, 3, c2, 2) && !IsComboSuccess(h1, 0, c1, 3)
        && IsComboSuccess(h1, 3, c1, 0) && !IsComboSuccess(h1, 3, c3, 4);
    if (!ok) { std::printf("006d7080_TEST FAIL\n"); return 1; }
    std::printf("006d7080_TEST PASS\n");
    return 0;
}