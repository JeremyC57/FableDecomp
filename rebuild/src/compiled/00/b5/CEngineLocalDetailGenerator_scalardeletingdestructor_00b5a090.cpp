struct CEngineLocalDetailGenerator {
    void* vftable;
};

void __fastcall CEngineLocalDetailGenerator_dtor(CEngineLocalDetailGenerator* generator);
void operator delete(void*);

void* __fastcall CEngineLocalDetailGenerator_scalar_deleting_destructor(CEngineLocalDetailGenerator* generator, int edx_unused, unsigned int flags)
{
    CEngineLocalDetailGenerator_dtor(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}