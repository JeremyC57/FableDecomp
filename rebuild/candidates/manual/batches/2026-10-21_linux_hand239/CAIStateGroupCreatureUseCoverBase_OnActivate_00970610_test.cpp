#include <cstdio>
#include <cstring>
#include <set>
#include <string>
// Behaviour copy of CAIStateGroup_CreatureUseCoverBase::OnActivate (0x00970610).
struct Creature { std::set<std::string> anims; int queried; };
struct Def { int refs; unsigned long speech; int released; };
struct Group {
    Creature* creature; Def* def; bool hasDive, hasJump; long count; unsigned long said; Creature* talker;
    bool Has(const char* n) { ++creature->queried; return creature->anims.count(n) != 0; }
    void OnActivate() {
        hasDive = Has("DIVE_FOR_COVER");
        hasJump = Has("JUMP_OUT_LEFT") && Has("JUMP_OUT_RIGHT");
        unsigned long g; ++def->refs; g = def->speech; if (--def->refs == 0) ++def->released;
        talker = creature; said = g; count = 0;
    }
};
int main() {
    Creature c; c.queried = 0; c.anims.insert("JUMP_OUT_RIGHT"); c.anims.insert("DIVE_FOR_COVER");
    Def d = { 1, 77, 0 };
    Group g = { &c, &d, false, true, 9, 0, 0 };
    g.OnActivate();
    if (!g.hasDive || g.hasJump || c.queried != 2 || g.said != 77 || g.talker != &c || g.count != 0 || d.refs != 1 || d.released) { std::printf("00970610_TEST FAIL\n"); return 1; }
    c.anims.insert("JUMP_OUT_LEFT"); c.queried = 0; g.OnActivate();
    if (!g.hasJump || c.queried != 3) { std::printf("00970610_TEST FAIL\n"); return 1; }
    std::printf("00970610_TEST PASS\n"); return 0;
}
