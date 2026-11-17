// CTCStealth::IsAvailableForAttack() -- the creature can be attacked when no attacker limit is exceeded
// NF48 (round 7, follower of 0x00747890): the callee 0x00747890 is CTCStealth::GetNumberOfAttackersOverMaximum(vector<const CThing*>*)
//   const (Ego_r.pdb, private; the landed row), called with a null list (`push 0`); the made-up stand-in GetAttackAvailability(bool) is
//   gone. The list type is STLport 4.6.2 std::vector<const CThing*> from the shared stlport/fable_containers.h.
#include "stlport/fable_containers.h"
class CThing;
class CTCStealth { public:
    bool IsAvailableForAttack();
private:
    long GetNumberOfAttackersOverMaximum(std::vector<const CThing*>* attackers) const;   // 0x00747890
};
bool CTCStealth::IsAvailableForAttack() {
    return GetNumberOfAttackersOverMaximum(0) == -1;
}
