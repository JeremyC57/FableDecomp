#pragma optimize("s",on)
class COpinionPersonalityDef { public: int f0; };
extern void __fastcall COpinionPersonalityDef_dtor(COpinionPersonalityDef* def);

void* __fastcall COpinionPersonalityDef_scalar_deleting_destructor(COpinionPersonalityDef* def, int edx, unsigned int flags) {
    (void)edx;
    COpinionPersonalityDef_dtor(def);
    if (flags & 1) {
        operator delete((void*)def);
    }
    return (void*)def;
}