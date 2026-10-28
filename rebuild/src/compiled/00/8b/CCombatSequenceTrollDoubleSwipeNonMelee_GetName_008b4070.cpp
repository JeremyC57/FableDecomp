#include "engine/CCombatSequence_TrollDoubleSwipeNonMelee.h"

class CCharString { public: char* p; CCharString(const char*, long); };
struct CCombatSequence_TrollDoubleSwipeNonMelee_Methods : CCombatSequence_TrollDoubleSwipeNonMelee { CCharString GetName() const; };
CCharString CCombatSequence_TrollDoubleSwipeNonMelee_Methods::GetName() const { return CCharString((const char*)0x1278a18, -1); }
