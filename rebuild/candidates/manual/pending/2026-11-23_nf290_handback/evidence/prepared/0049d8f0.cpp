#include "rebuild_abi.h"
// CWorld::SetAsPaused @ 0x0049d8f0
class CWorld { public: char pad[0xa4]; bool Paused; protected: void SetAsPaused(bool paused); public: };
void CWorld::SetAsPaused(bool paused){ if(paused != Paused) Paused = paused; }