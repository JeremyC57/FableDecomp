// behaviour test for 0x00717710 NTCScriptedControl::CActionTalkBase::OnActivate(): the conversation-mode decision
#include <cstdio>
struct Thing { int type; bool knockedDown; bool performing; bool allowsMove; bool canConverse; };
struct Result { bool gettingUp; bool addedMode; int removed; };
static Result OnActivate(const Thing& parent, const Thing* target, bool hasModes) {
    Result r = {false, false, 0};
    if (parent.knockedDown || (parent.performing && parent.allowsMove)) r.gettingUp = true;
    if (target && (target->knockedDown || (target->performing && target->allowsMove))) r.gettingUp = true;
    if (hasModes) r.removed++;
    if (parent.type != 0 /*hero*/ && hasModes && parent.canConverse) r.addedMode = true;
    return r;
}
int main() {
    Thing p = {1, false, true, true, true}; Thing t = {0, false, false, false, false};
    Result a = OnActivate(p, &t, true);
    if (!a.gettingUp || !a.addedMode || a.removed != 1) { std::printf("00717710_TEST FAIL\n"); return 1; }
    Thing h = {0, false, false, false, true};
    Result b = OnActivate(h, 0, true);
    if (b.gettingUp || b.addedMode) { std::printf("00717710_TEST FAIL\n"); return 1; }
    Result c = OnActivate(p, &t, false);
    if (c.addedMode || c.removed) { std::printf("00717710_TEST FAIL\n"); return 1; }
    std::printf("00717710_TEST PASS\n"); return 0;
}