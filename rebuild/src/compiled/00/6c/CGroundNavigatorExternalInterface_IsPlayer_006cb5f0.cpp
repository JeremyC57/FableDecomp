#include "rebuild_abi.h"
struct S { char p[0x10]; unsigned char f10; };
struct C { char p[4]; S* Creature; bool IsPlayer(); };
bool C::IsPlayer(){ return this->Creature->f10==2; }