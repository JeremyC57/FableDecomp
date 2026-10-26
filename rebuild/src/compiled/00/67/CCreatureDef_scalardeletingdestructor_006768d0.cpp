class CCreatureDef { public:
    void* vftable;
};

void __fastcall CCreatureDef_dtor(CCreatureDef* def);
void operator delete(void*);

void* __fastcall CCreatureDef_scalar_deleting_destructor(CCreatureDef* def, int edx_unused, unsigned int flags)
{
    CCreatureDef_dtor(def);
    if (flags & 1)
        operator delete(def);
    return def;
}