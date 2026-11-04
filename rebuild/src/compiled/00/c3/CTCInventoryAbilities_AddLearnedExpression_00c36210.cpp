#include "rebuild_abi.h"
// Leaf: ?Learn@C@@QAEHXZ
struct Sub { int Add(void* p); };
struct C { char p44[0x44]; int f44; char p48[0x4c-0x48]; Sub s4c; int Learn(); };
int C::Learn(){ return this->s4c.Add(&this->f44); }