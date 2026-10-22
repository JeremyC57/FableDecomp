struct CTCAppearanceMorphBase {
    void* vftable;
};

void __fastcall CTCAppearanceMorphBase_dtor(CTCAppearanceMorphBase* morph);
void operator delete(void*);

void* __fastcall CTCAppearanceMorphBase_scalar_deleting_destructor(CTCAppearanceMorphBase* morph, int edx_unused, unsigned int flags)
{
    CTCAppearanceMorphBase_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}