#include <cstdio>
struct GAN004d2bec { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2bec::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCActionUseSearch_StatGetClassTypeName(GAN004d2bec* result){ result->m((void*)0x123a128, -1); return result; }
int main(){ GAN004d2bec x; void* r=CTCActionUseSearch_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a128||gN!=-1){ printf("004d2bec_TEST FAIL\n"); return 1; }
 printf("004d2bec_TEST PASS\n"); return 0; }