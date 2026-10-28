#include "engine/CCombatSequence_GenericRangedAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
static const char kName[] = "GenericRangedAttack";
struct CCombatSequence_GenericRangedAttack_Methods : CCombatSequence_GenericRangedAttack { CCharString GetName() const; };
CCharString CCombatSequence_GenericRangedAttack_Methods::GetName() const { return CCharString(kName, -1); }
