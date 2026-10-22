struct CAnimComponentCombatComboChain {
    void* vtbl_slot0;
    void* f4;
    void* f8;
    void* fc;
    // vtable is at *(void***)this ; slot at +0x10 = index 4
};

struct VT {
    void* s0;
    void* s1;
    void* s2;
    void* s3;
    CAnimComponentCombatComboChain* (__fastcall* clone)(void* component); // slot 4, [eax+0x10]
};

CAnimComponentCombatComboChain* __fastcall Clone_ComboChain(CAnimComponentCombatComboChain* component)
{
    VT* vt = *(VT**)component;
    CAnimComponentCombatComboChain* n = vt->clone(component);
    n->fc = component->fc;
    n->f8 = component->f8;
    return n;
}