#include <cstdio>
struct GAN004d2d00 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2d00::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCCarriedActionUseGoldBag_StatGetClassTypeName(GAN004d2d00* result){ result->m((void*)0x123a26c, -1); return result; }
int main(){ GAN004d2d00 x; void* r=CTCCarriedActionUseGoldBag_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a26c||gN!=-1){ printf("004d2d00_TEST FAIL\n"); return 1; }
 printf("004d2d00_TEST PASS\n"); return 0; }