
#include <stdio.h>
struct CAnimComponentCombatComboChain { void* v; void* f4; void* f8; void* fc; };
struct VT { void* s0; void* s1; void* s2; void* s3; CAnimComponentCombatComboChain* (__fastcall* clone)(void*); };
static CAnimComponentCombatComboChain g_new;
static CAnimComponentCombatComboChain* __fastcall do_clone(void* component){ g_new.f8=(void*)1; g_new.fc=(void*)2; return &g_new; }
static VT g_vt = {0,0,0,0,do_clone};
struct T2 { void* v; void* f4; void* f8; void* fc; };
extern "C" CAnimComponentCombatComboChain* clone_model(CAnimComponentCombatComboChain* component){
    VT* vt=*(VT**)component; CAnimComponentCombatComboChain* n=vt->clone(component); n->fc=component->fc; n->f8=component->f8; return n;
}
int main(){
    CAnimComponentCombatComboChain component; component.v=&g_vt; component.f4=(void*)0xAA; component.f8=(void*)0x88; component.fc=(void*)0xCC;
    CAnimComponentCombatComboChain* r=clone_model(&component);
    if(r==&g_new && r->f8==(void*)0x88 && r->fc==(void*)0xCC){ printf("OK_005db2c0\n"); return 0; }
    printf("FAIL\n"); return 1;
}