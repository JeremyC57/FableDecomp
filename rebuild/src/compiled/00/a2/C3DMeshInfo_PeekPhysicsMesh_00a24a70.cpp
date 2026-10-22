struct A { char pad[0x380]; void** arr; };
struct S { char pad[4]; A* Bank; char pad2[0x3c-8]; int idx; };
void* __fastcall PeekPhysicsMesh(S* info){ A* a=info->Bank; return a->arr[info->idx]; }