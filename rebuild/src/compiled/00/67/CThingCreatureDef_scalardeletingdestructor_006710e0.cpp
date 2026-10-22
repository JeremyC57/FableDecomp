struct CThingCreatureDef {
    void* vftable;
};

void __fastcall CThingCreatureDef_dtor(CThingCreatureDef* def);
void operator delete(void*);

void* __fastcall CThingCreatureDef_scalar_deleting_destructor(CThingCreatureDef* def, int edx_unused, unsigned int flags)
{
    CThingCreatureDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}