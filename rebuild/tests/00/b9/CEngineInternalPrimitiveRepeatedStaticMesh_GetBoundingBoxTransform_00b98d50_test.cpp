#include <cstdio>
struct Base00b98d50 { void m(void* matrix); };
static void* gBase; static void* gArg;
void Base00b98d50::m(void* matrix){ gBase=this; gArg=matrix; }
char __fastcall CEngineInternalPrimitiveRepeatedStaticMesh_GetBoundingBoxTransform(char* mesh, int _edx, void* matrix){ ((Base00b98d50*)(mesh+0x58))->m(matrix); return 1; }
int main(){ char blk[0x100]; int a; char r=CEngineInternalPrimitiveRepeatedStaticMesh_GetBoundingBoxTransform(blk,0,&a);
 if(r!=1||gBase!=blk+0x58||gArg!=&a){ printf("00b98d50_TEST FAIL\n"); return 1; }
 printf("00b98d50_TEST PASS\n"); return 0; }