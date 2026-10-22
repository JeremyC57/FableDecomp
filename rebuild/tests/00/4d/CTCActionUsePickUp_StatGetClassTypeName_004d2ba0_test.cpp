#include <cstdio>
struct GAN004d2ba0 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2ba0::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCActionUsePickUp_StatGetClassTypeName(GAN004d2ba0* result){ result->m((void*)0x123a0c0, -1); return result; }
int main(){ GAN004d2ba0 x; void* r=CTCActionUsePickUp_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a0c0||gN!=-1){ printf("004d2ba0_TEST FAIL\n"); return 1; }
 printf("004d2ba0_TEST PASS\n"); return 0; }