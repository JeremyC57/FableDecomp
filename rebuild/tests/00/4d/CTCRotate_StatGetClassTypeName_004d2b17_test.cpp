#include <cstdio>
struct GAN004d2b17 { void* m(void* s, int n); };
static void* gSelf; static void* gStr; static int gN;
void* GAN004d2b17::m(void* s, int n){ gSelf=this; gStr=s; gN=n; return 0; }
void* __fastcall CTCRotate_StatGetClassTypeName(GAN004d2b17* result){ result->m((void*)0x123a014, -1); return result; }
int main(){ GAN004d2b17 x; void* r=CTCRotate_StatGetClassTypeName(&x);
 if(r!=&x||gSelf!=&x||gStr!=(void*)0x123a014||gN!=-1){ printf("004d2b17_TEST FAIL\n"); return 1; }
 printf("004d2b17_TEST PASS\n"); return 0; }