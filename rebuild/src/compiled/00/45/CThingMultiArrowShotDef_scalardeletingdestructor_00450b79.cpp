#pragma optimize("s",on)
extern const void* CThingMultiArrowShotDef_vftable;
extern "C" void __cdecl FableFreeMem(void* p);

struct CThingMultiArrowShotDef {
    const void** vft;
};

void __fastcall CThingMultiArrowShotDef_dtor(CThingMultiArrowShotDef* def);

void* __fastcall CThingMultiArrowShotDef_scalar_deleting_destructor(CThingMultiArrowShotDef* def, int edx, unsigned int flags) {
    (void)edx;
    def->vft = (const void**)&CThingMultiArrowShotDef_vftable;
    CThingMultiArrowShotDef_dtor(def);
    if (flags & 1) {
        FableFreeMem(def);
    }
    return def;
}