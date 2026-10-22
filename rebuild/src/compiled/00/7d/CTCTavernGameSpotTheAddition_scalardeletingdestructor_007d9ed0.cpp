struct CTCTavernGameSpotTheAddition {
    void* vftable;
};

void __fastcall CTCTavernGameSpotTheAddition_dtor(CTCTavernGameSpotTheAddition* addition);
void operator delete(void*);

void* __fastcall CTCTavernGameSpotTheAddition_scalar_deleting_destructor(CTCTavernGameSpotTheAddition* addition, int edx_unused, unsigned int flags)
{
    CTCTavernGameSpotTheAddition_dtor(addition);
    if (flags & 1)
        operator delete(addition);
    return addition;
}