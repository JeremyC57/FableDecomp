#include "engine/CCombatSequenceWhisperStrikeStrafeBack.h"

class CCharString { public: char* m_data; CCharString(); CCharString(const char*, long); CCharString(const CCharString&); ~CCharString(); };
struct CCombatSequenceWhisperStrikeStrafeBack_Methods : CCombatSequenceWhisperStrikeStrafeBack { CCharString GetName(); };
CCharString CCombatSequenceWhisperStrikeStrafeBack_Methods::GetName() { return CCharString((const char*)0x1278d6c, -1); }
