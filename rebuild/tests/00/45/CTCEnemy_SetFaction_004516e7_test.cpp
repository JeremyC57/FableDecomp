#include <cstdio>

class CTCEnemy { public: void* vtbl; int ParentThing; };

static int g_helper_a, g_helper_b;
void __stdcall SF_helper(int a, int b){ g_helper_a=a; g_helper_b=b; }
int __fastcall SF_make(void* obj, int p1, int p2){ return (int)(long)obj + p1*10 + p2; }

CTCEnemy* __fastcall CTCEnemy_SetFaction(CTCEnemy* self, int edx_unused, int p1, int p2, int p3)
{
    SF_helper(p1, p3);
    void* vt = *(void**)self;
    self->ParentThing = SF_make(vt, p1, p2);
    return self;
}

int main(){
    CTCEnemy e; e.vtbl=(void*)0; e.ParentThing=-1;
    CTCEnemy* r = CTCEnemy_SetFaction(&e, 0, 3, 5, 7);
    if(r!=&e){ std::printf("FAIL ret\n"); return 1; }
    if(g_helper_a!=3 || g_helper_b!=7){ std::printf("FAIL helper %d %d\n", g_helper_a, g_helper_b); return 1; }
    if(e.ParentThing != (0 + 3*10 + 5)){ std::printf("FAIL field4 %d\n", e.ParentThing); return 1; }
    std::printf("CTCEnemy_004516e7_TEST PASS\n");
    return 0;
}