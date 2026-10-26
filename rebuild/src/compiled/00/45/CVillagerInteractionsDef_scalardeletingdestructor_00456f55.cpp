#pragma optimize("s",on)
class CVillagerInteractionsDef { public:
    int f0;
};

extern void __fastcall CVillagerInteractionsDef_dtor(CVillagerInteractionsDef* def);
extern void __cdecl op_delete(void* p);

void* __fastcall CVillagerInteractionsDef_scalar_deleting_destructor(CVillagerInteractionsDef* def, int edx, unsigned int flags) {
    (void)edx;
    CVillagerInteractionsDef_dtor(def);
    if (flags & 1) {
        op_delete(def);
    }
    return def;
}