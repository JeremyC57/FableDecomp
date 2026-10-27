// Behaviour test for 0x0085BD60 CCombatAction_ControlledStrafeJump::IsOverrideableByActionOfSamePriority() const:
// overrideable exactly when the (virtual) IsActionReadyForInterruption says so.
#include <cstdio>

class ActionBase {
public:
    virtual ~ActionBase() { }
    virtual bool IsOverrideableByActionOfSamePriority() const { return false; }
    virtual bool IsActionReadyForInterruption() const { return false; }
};
class StrafeJump : public ActionBase {
public:
    bool ready;
    virtual bool IsActionReadyForInterruption() const { return ready; }
    virtual bool IsOverrideableByActionOfSamePriority() const
    {
        if (!IsActionReadyForInterruption())
            return false;
        return true;
    }
};

int main()
{
    StrafeJump j;
    const ActionBase& a = j;
    j.ready = false;
    if (a.IsOverrideableByActionOfSamePriority()) { std::printf("0085bd60_TEST FAIL\n"); return 1; }
    j.ready = true;
    if (!a.IsOverrideableByActionOfSamePriority()) { std::printf("0085bd60_TEST FAIL\n"); return 1; }
    std::printf("0085bd60_TEST PASS\n");
    return 0;
}