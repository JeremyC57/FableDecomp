#include <cstdio>
struct Base00baa410 { void m(void* matrix); };
static void* gBase; static void* gArg;
void Base00baa410::m(void* matrix){ gBase=this; gArg=matrix; }
char __fastcall fn_00baa410(char* sprite, int _edx, void* matrix){ ((Base00baa410*)(sprite+0x74))->m(matrix); return 1; }
int main(){ char blk[0x100]; int a; char r=fn_00baa410(blk,0,&a);
 if(r!=1||gBase!=blk+0x74||gArg!=&a){ printf("00baa410_TEST FAIL\n"); return 1; }
 printf("00baa410_TEST PASS\n"); return 0; }