// behaviour test for 0x00751DA0 CTCLook::IsAbleToTurnDaBody() const
#include <cstdio>
struct Creature { bool dead; bool acting; bool locked; };
static bool IsAbleToTurnDaBody(const Creature* c) {
    if (c && !c->dead && c->acting) {
        if (c->locked) return false;
    }
    return true;
}
int main() {
    Creature a = {false, true, true}, b = {true, true, true}, c = {false, false, true}, d = {false, true, false};
    if (IsAbleToTurnDaBody(&a) || !IsAbleToTurnDaBody(&b) || !IsAbleToTurnDaBody(&c) || !IsAbleToTurnDaBody(&d) || !IsAbleToTurnDaBody(0)) {
        std::printf("00751da0_TEST FAIL\n"); return 1;
    }
    std::printf("00751da0_TEST PASS\n");
    return 0;
}