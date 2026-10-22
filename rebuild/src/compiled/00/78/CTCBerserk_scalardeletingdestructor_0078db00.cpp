struct CTCBerserk {
    void* vftable;
};

void __fastcall CTCBerserk_dtor(CTCBerserk* berserk);
void operator delete(void*);

void* __fastcall CTCBerserk_scalar_deleting_destructor(CTCBerserk* berserk, int edx_unused, unsigned int flags)
{
    CTCBerserk_dtor(berserk);
    if (flags & 1)
        operator delete(berserk);
    return berserk;
}