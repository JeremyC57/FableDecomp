// Behaviour test for 0x00865B50 CCombatAction_GenericStrikeResponseBase::IsAlwaysPhysicsPushableBySelfAvoidance() const:
// true exactly when the action is using action movement.
#include <cstdio>

class ActionBase {
public:
    bool usingActionMovement;
    virtual ~ActionBase() { }
    bool IsUsingActionMovement() const { return usingActionMovement; }
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const { return false; }
};
class StrikeResponse : public ActionBase {
public:
    virtual bool IsAlwaysPhysicsPushableBySelfAvoidance() const
    {
        if (!IsUsingActionMovement())
            return false;
        return true;
    }
};

int main()
{
    StrikeResponse s;
    const ActionBase& a = s;
    s.usingActionMovement = true;
    if (!a.IsAlwaysPhysicsPushableBySelfAvoidance()) { std::printf("00865b50_TEST FAIL\n"); return 1; }
    s.usingActionMovement = false;
    if (a.IsAlwaysPhysicsPushableBySelfAvoidance()) { std::printf("00865b50_TEST FAIL\n"); return 1; }
    std::printf("00865b50_TEST PASS\n");
    return 0;
}