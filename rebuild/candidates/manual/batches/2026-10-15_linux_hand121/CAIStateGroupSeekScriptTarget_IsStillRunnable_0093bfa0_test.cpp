#include <cstdio>
// SeekScriptTarget::IsStillRunnable: enemy alive, still the targeting component's target, component pointer set
struct Thing { unsigned char flags91; };
struct Ptr { Thing* item; Thing* GetPItem() const { return item; } };
struct Targeting { Thing* target; Ptr ptr; Thing* GetTarget() { return target; } };
struct Group {
    Targeting* targeting; Ptr enemy;
    bool IsStillRunnable()
    {
        if (enemy.GetPItem() != 0 && !(enemy.GetPItem()->flags91 & 1)) { if (enemy.GetPItem() == targeting->GetTarget()) { if (targeting->ptr.GetPItem() != 0) return true; } }
        return false;
    }
};
int main()
{
    Thing e; e.flags91 = 0; Thing other; other.flags91 = 0; Targeting t; t.target = &e; t.ptr.item = &other; Group g; g.targeting = &t; g.enemy.item = &e;
    if (!g.IsStillRunnable()) { std::printf("0093bfa0_TEST FAIL\n"); return 1; }
    t.ptr.item = 0; if (g.IsStillRunnable()) { std::printf("0093bfa0_TEST FAIL\n"); return 1; }
    t.ptr.item = &other; t.target = &other; if (g.IsStillRunnable()) { std::printf("0093bfa0_TEST FAIL\n"); return 1; }
    t.target = &e; e.flags91 = 1; if (g.IsStillRunnable()) { std::printf("0093bfa0_TEST FAIL\n"); return 1; }
    e.flags91 = 0; g.enemy.item = 0; if (g.IsStillRunnable()) { std::printf("0093bfa0_TEST FAIL\n"); return 1; }
    std::printf("0093bfa0_TEST PASS\n"); return 0;
}
