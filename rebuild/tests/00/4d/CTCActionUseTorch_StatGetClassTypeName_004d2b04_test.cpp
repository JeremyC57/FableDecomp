#include <cstdio>
struct GAN004d2b04 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2b04::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCActionUseTorch_StatGetClassTypeName(GAN004d2b04* result){ result->m((void*)0x123a000, -1); return result; }
int main(){ GAN004d2b04 x; void* r=CTCActionUseTorch_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a000||gN!=-1){ printf("004d2b04_TEST FAIL\n"); return 1; }
 printf("004d2b04_TEST PASS\n"); return 0; }