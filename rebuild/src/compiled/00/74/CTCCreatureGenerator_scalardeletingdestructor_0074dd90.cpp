class CTCCreatureGenerator { public:
    void* vftable;
};

void __fastcall CTCCreatureGenerator_dtor(CTCCreatureGenerator* generator);
void operator delete(void*);

void* __fastcall CTCCreatureGenerator_scalar_deleting_destructor(CTCCreatureGenerator* generator, int edx_unused, unsigned int flags)
{
    CTCCreatureGenerator_dtor(generator);
    if (flags & 1)
        operator delete(generator);
    return generator;
}