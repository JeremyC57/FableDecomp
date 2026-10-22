#include <cstdio>
struct A { char pad[0x380]; void** arr; };
struct S { char pad[4]; A* Bank; char pad2[0x3c-8]; int idx; };
void* __fastcall PeekPhysicsMesh(S* info){ A* a=info->Bank; return a->arr[info->idx]; }
int main(){ void* arr[4]={0,0,(void*)7,0}; A a; a.arr=arr; S o; o.Bank=&a; o.idx=2;
 if(PeekPhysicsMesh(&o)!=(void*)7){ std::printf("00a24a70_TEST FAIL\n"); return 1;}
 std::printf("00a24a70_TEST PASS\n"); return 0;}