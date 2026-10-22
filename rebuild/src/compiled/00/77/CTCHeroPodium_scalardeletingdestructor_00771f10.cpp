struct CTCHeroPodium {
    void* vftable;
};

void __fastcall CTCHeroPodium_dtor(CTCHeroPodium* podium);
void operator delete(void*);

void* __fastcall CTCHeroPodium_scalar_deleting_destructor(CTCHeroPodium* podium, int edx_unused, unsigned int flags)
{
    CTCHeroPodium_dtor(podium);
    if (flags & 1)
        operator delete(podium);
    return podium;
}