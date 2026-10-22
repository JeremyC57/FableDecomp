struct CTCStealth {
    void* vftable;
};

void __fastcall CTCStealth_dtor(CTCStealth* stealth);
void operator delete(void*);

void* __fastcall CTCStealth_scalar_deleting_destructor(CTCStealth* stealth, int edx_unused, unsigned int flags)
{
    CTCStealth_dtor(stealth);
    if (flags & 1)
        operator delete(stealth);
    return stealth;
}