struct CAIScratchpadDef {
    void* vftable;
};

void __fastcall CAIScratchpadDef_dtor(CAIScratchpadDef* def);
void operator delete(void*);

void* __fastcall CAIScratchpadDef_scalar_deleting_destructor(CAIScratchpadDef* def, int edx_unused, unsigned int flags)
{
    CAIScratchpadDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}