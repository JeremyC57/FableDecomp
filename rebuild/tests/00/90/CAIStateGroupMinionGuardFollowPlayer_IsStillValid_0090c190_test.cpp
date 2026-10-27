// Behaviour test for 0x0090C190 CAIStateGroup_MinionGuardFollowPlayer::IsStillValid() const: the full check,
// IsStillValidInternal(true), through the virtual.
#include <cstdio>

class StateGroup {
public:
    virtual ~StateGroup() { }
    virtual bool IsStillValid() const { return false; }
protected:
    virtual bool IsStillValidInternal(bool) const { return false; }
};
class GuardFollow : public StateGroup {
public:
    mutable int calls; mutable bool lastFull; bool validWhenFull;
    virtual bool IsStillValid() const { return IsStillValidInternal(true); }
protected:
    virtual bool IsStillValidInternal(bool full) const { ++calls; lastFull = full; return full && validWhenFull; }
};

int main()
{
    GuardFollow g; g.calls = 0; g.lastFull = false; g.validWhenFull = true;
    const StateGroup& s = g;
    if (!s.IsStillValid() || g.calls != 1 || !g.lastFull) { std::printf("0090c190_TEST FAIL\n"); return 1; }
    g.validWhenFull = false;
    if (s.IsStillValid() || g.calls != 2) { std::printf("0090c190_TEST FAIL\n"); return 1; }
    std::printf("0090c190_TEST PASS\n");
    return 0;
}