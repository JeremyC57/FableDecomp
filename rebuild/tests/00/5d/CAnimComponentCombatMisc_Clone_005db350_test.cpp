#include <cstdio>

struct CAnimComponentCombatMisc;
struct Vtbl {
    void* s0; void* s1; void* s2; void* s3;
    CAnimComponentCombatMisc* (__fastcall* clone)(CAnimComponentCombatMisc* component);
};
struct CAnimComponentCombatMisc {
    Vtbl* vptr;
    char pad4[4];
    char b8;
    char b9;
};

static CAnimComponentCombatMisc g_new;
static CAnimComponentCombatMisc* __fastcall my_clone(CAnimComponentCombatMisc* component)
{
    g_new.vptr = component->vptr;
    g_new.b8 = 0; g_new.b9 = 0;
    return &g_new;
}

void* __fastcall Clone_CombatMisc(CAnimComponentCombatMisc* component)
{
    CAnimComponentCombatMisc* n = component->vptr->clone(component);
    n->b8 = component->b8;
    n->b9 = component->b9;
    return n;
}

int main()
{
    Vtbl vt = {0,0,0,0, my_clone};
    CAnimComponentCombatMisc src;
    src.vptr = &vt;
    src.b8 = (char)0xAB;
    src.b9 = (char)0xCD;
    void* r = Clone_CombatMisc(&src);
    CAnimComponentCombatMisc* n = (CAnimComponentCombatMisc*)r;
    if (r == &g_new && n->b8 == (char)0xAB && n->b9 == (char)0xCD) {
        printf("OK_005db350\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}