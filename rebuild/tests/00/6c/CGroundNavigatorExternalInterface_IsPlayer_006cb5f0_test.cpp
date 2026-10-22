#include "rebuild_abi.h"
#include <cstdio>
struct S { char p[0x10]; unsigned char f10; };
struct C { char p[4]; S* Creature; bool IsPlayer(); };
bool C::IsPlayer(){ return this->Creature->f10==2; }
int main(){ S s; s.f10=2; C o; o.Creature=&s; if(!o.IsPlayer()){std::printf("B1");return 1;} s.f10=1; if(o.IsPlayer()){std::printf("B2");return 1;} std::printf("RB_6cb5f0_OK\n"); return 0; }