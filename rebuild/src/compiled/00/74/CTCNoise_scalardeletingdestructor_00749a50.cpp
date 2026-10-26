class CTCNoise { public:
    void* vftable;
};

void __fastcall CTCNoise_dtor(CTCNoise* noise);
void operator delete(void*);

void* __fastcall CTCNoise_scalar_deleting_destructor(CTCNoise* noise, int edx_unused, unsigned int flags)
{
    CTCNoise_dtor(noise);
    if (flags & 1)
        operator delete(noise);
    return noise;
}