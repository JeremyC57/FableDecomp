struct A { char pad[0x380]; void** arr; };
struct S { char pad[4]; A* Bank; char pad2[0x3c-8]; int idx; };
void* __fastcall PeekPhysicsMesh(S* self){ A* a=self->Bank; return a->arr[self->idx]; }