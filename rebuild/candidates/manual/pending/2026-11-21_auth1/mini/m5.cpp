#include "pch.h"
struct A { T* t; T* u; N* n; bool f1(); };
bool A::f1() { if (n->Nav(t->GetPos(), u->GetPos(), t->GetCM().GetP())) return true; return false; }
