struct CTCCreditsUI {
    void* vftable;
};

void __fastcall CTCCreditsUI_dtor(CTCCreditsUI* creditsUI);
void operator delete(void*);

void* __fastcall CTCCreditsUI_scalar_deleting_destructor(CTCCreditsUI* creditsUI, int edx_unused, unsigned int flags)
{
    CTCCreditsUI_dtor(creditsUI);
    if (flags & 1)
        operator delete(creditsUI);
    return creditsUI;
}