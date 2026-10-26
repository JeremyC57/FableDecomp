#include "rebuild_abi.h"
// Leaf: DrawGetEnvironment
// CWorld::DrawGetEnvironment @ 0x00f14250
class CWorld { public: char pad[0x44]; void *m_env; void *DrawGetEnvironment(); };
void *CWorld::DrawGetEnvironment(){ return m_env; }