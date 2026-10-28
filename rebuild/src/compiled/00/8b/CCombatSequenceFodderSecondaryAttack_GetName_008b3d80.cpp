#include "engine/CCombatSequence_FodderSecondaryAttack.h"

class CCharString { public: char* m_buf; CCharString(const char*, long); };
static const char kName[] = "FodderSecondaryAttack";
struct CCombatSequence_FodderSecondaryAttack_Methods : CCombatSequence_FodderSecondaryAttack { CCharString GetName() const; };
CCharString CCombatSequence_FodderSecondaryAttack_Methods::GetName() const { return CCharString(kName, -1); }
