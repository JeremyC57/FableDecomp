#include "rebuild_abi.h"
// Leaf: DrawGetEnvironment
struct CWorld { char pad[0x18]; void *m_env; void *DrawGetEnvironment(); };
void *CWorld::DrawGetEnvironment(){ return m_env; }