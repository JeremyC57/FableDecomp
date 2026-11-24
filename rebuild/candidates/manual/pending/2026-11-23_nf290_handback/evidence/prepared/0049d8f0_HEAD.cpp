#include "rebuild_abi.h"
// CWorld::SetAsPaused @ 0x0049d8f0
class CWorld { public: char pad[0xa4]; char m_paused; void SetAsPaused(char v); };
void CWorld::SetAsPaused(char v){ if(v != m_paused) m_paused = v; }