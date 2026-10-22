#include "rebuild_abi.h"
struct D { char p[0x1c9]; unsigned char f1c9; };
struct A { char p[0x18]; D* f18; };
struct C { char p[4]; A* PBrain; char pac[0xac-8]; unsigned char WasAllowedToFollowPlayer; void OnActivate(); };
void C::OnActivate(){ this->WasAllowedToFollowPlayer = (unsigned char)(this->PBrain->f18->f1c9 & 1); }