// Behaviour test for 0x004A9140: the two string members are destroyed in reverse member order (+0x4 first, then +0x0), each once.
#include <cstdio>
static int gN; static const void* gOrder[2];
struct WideMember { void* Data; void Destroy() { gOrder[gN++] = this; } };
struct DV { WideMember First; WideMember Second; };
static void __fastcall fn_004a9140(DV* self){ self->Second.Destroy(); self->First.Destroy(); }
int main(){ DV d; gN=0; fn_004a9140(&d);
 if(gN!=2||gOrder[0]!=&d.Second||gOrder[1]!=&d.First){ printf("004a9140_TEST FAIL\n"); return 1; }
 printf("004a9140_TEST PASS\n"); return 0; }
