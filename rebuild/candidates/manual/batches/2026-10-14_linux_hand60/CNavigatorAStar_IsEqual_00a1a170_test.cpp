#include <cstdio>
#include <cstdlib>
#include <cstring>
static long g_eq = 0; struct CNode { float x, y; bool operator==(const CNode& o) const { ++g_eq; float dx = x - o.x, dy = y - o.y; return dx < 1e-4f && dx > -1e-4f && dy < 1e-4f && dy > -1e-4f; } };
struct CAStar { bool IsEqual(const CNode* a, const CNode* b) { if (a == b) return true; if (!a || !b) return false; return *a == *b; } };

int main() {
    CNode a, b; a.x = 1; a.y = 2; b.x = 1; b.y = 2; CAStar s; if (!s.IsEqual(&a, &a) || g_eq != 0 || !s.IsEqual(&a, &b) || g_eq != 1 || s.IsEqual(&a, 0) || s.IsEqual(0, &b) || !s.IsEqual(0, 0)) { std::printf("00a1a170_TEST FAIL\n"); return 1; } b.y = 3; if (s.IsEqual(&a, &b)) { std::printf("00a1a170_TEST FAIL\n"); return 1; }
    std::printf("00a1a170_TEST PASS\n"); return 0;
}
