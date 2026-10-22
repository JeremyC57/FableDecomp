struct C3DSkeletalMorph {
    void* vftable;
};

void __fastcall C3DSkeletalMorph_dtor(C3DSkeletalMorph* morph);
void operator delete(void*);

void* __fastcall C3DSkeletalMorph_scalar_deleting_destructor(C3DSkeletalMorph* morph, int edx_unused, unsigned int flags)
{
    C3DSkeletalMorph_dtor(morph);
    if (flags & 1)
        operator delete(morph);
    return morph;
}