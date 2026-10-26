#include "engine/CCombatSequenceUnblockableFacing.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, int); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceUnblockableFacing_Methods : CCombatSequenceUnblockableFacing { CCharString GetName(); };
CCharString CCombatSequenceUnblockableFacing_Methods::GetName() { return CCharString((const char*)0x1278e90, -1); }
