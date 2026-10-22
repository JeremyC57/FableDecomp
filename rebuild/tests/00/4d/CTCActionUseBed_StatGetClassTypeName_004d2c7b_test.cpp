#include <cstdio>
struct GAN004d2c7b { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2c7b::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCActionUseBed_StatGetClassTypeName(GAN004d2c7b* result){ result->m((void*)0x123a1bc, -1); return result; }
int main(){ GAN004d2c7b x; void* r=CTCActionUseBed_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a1bc||gN!=-1){ printf("004d2c7b_TEST FAIL\n"); return 1; }
 printf("004d2c7b_TEST PASS\n"); return 0; }