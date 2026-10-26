class CEngineBillboardGenerator { public:
    void* vftable;
};

void __fastcall CEngineBillboardGenerator_dtor(CEngineBillboardGenerator* generator);
void operator delete(void*);

void* __fastcall CEngineBillboardGenerator_scalar_deleting_destructor(CEngineBillboardGenerator* generator, int edx_unused, unsigned int flags)
{
    CEngineBillboardGenerator_dtor(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}