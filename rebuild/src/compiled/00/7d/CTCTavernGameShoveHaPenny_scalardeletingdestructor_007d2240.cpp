struct CTCTavernGameShoveHaPenny {
    void* vftable;
};

void __fastcall CTCTavernGameShoveHaPenny_dtor(CTCTavernGameShoveHaPenny* penny);
void operator delete(void*);

void* __fastcall CTCTavernGameShoveHaPenny_scalar_deleting_destructor(CTCTavernGameShoveHaPenny* penny, int edx_unused, unsigned int flags)
{
    CTCTavernGameShoveHaPenny_dtor(penny);
    if (flags & 1)
        operator delete(penny);
    return penny;
}