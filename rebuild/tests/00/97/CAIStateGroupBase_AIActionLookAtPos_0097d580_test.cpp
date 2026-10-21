#include <cstdio>
// Behaviour copy of CAIStateGroupBase::AIAction_LookAtPos (0x0097D580).
struct V { float X, Y, Z; };
struct Output { unsigned long Types; long LookFlags; V LookAtPos; };
struct Creature { int lookedAfter; int calls; bool lastBody, lastHead;
    bool IsLookingAtPos(const V&, float, bool body, bool head) { lastBody = body; lastHead = head; return ++calls > lookedAfter; } };
struct Brain { Creature* creature; int yields; int outputs; Output last; int stopAfter;
    void Yield() { ++yields; } void SetBrainOutput(const Output& o) { ++outputs; last = o; } };
struct Group {
    Brain* PBrain;
    bool IsStopping() { return PBrain->stopAfter >= 0 && PBrain->yields >= PBrain->stopAfter; }
    int LookAtPos(int repeat, V pos, long flags, float tol) {
        if (IsStopping()) return 1;
        int result;
        do {
            Creature* c = PBrain->creature;
            if (c->IsLookingAtPos(pos, tol, (flags & 2) > 0, (flags & 4) > 0)) result = 2;
            else { Output o; o.Types = 0; o.LookFlags = 0; o.Types |= 2; o.LookAtPos = pos; o.LookFlags = flags; PBrain->SetBrainOutput(o); result = 4; }
            PBrain->Yield();
            if (IsStopping()) return 1;
        } while (repeat == 1 && result == 4);
        return result;
    }
};
int main() {
    Creature c = { 2, 0, false, false }; Brain b = { &c, 0, 0, {0, 0, {0, 0, 0}}, -1 }; Group g = { &b };
    V p = { 1, 2, 3 };
    int r = g.LookAtPos(1, p, 6, 0.1f);
    if (r != 2 || b.outputs != 2 || b.yields != 3 || b.last.Types != 2 || b.last.LookFlags != 6 || b.last.LookAtPos.Y != 2 || !c.lastBody || !c.lastHead) {
        std::printf("0097d580_TEST FAIL\n"); return 1; }
    Creature c2 = { 5, 0, false, false }; Brain b2 = { &c2, 0, 0, {0, 0, {0, 0, 0}}, -1 }; Group g2 = { &b2 };
    if (g2.LookAtPos(0, p, 2, 0.1f) != 4 || b2.outputs != 1 || c2.lastHead) { std::printf("0097d580_TEST FAIL\n"); return 1; }
    Brain b3 = { &c2, 0, 0, {0, 0, {0, 0, 0}}, 1 }; Group g3 = { &b3 };
    if (g3.LookAtPos(1, p, 0, 0.1f) != 1) { std::printf("0097d580_TEST FAIL\n"); return 1; }
    std::printf("0097d580_TEST PASS\n"); return 0;
}