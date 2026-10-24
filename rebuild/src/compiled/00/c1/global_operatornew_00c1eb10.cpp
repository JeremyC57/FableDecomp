#include "rebuild_abi.h"
// Leaf: operator_new
extern int g_vt_c1eb10;
struct Sub { void M_00c1ebf0(void* p); };
struct C { void* vt; Sub* f4; void Op(); };
void C::Op(){ this->vt=&g_vt_c1eb10; Sub* s=this->f4; ((Sub*)((char*)s+0x2c))->M_00c1ebf0(this); }