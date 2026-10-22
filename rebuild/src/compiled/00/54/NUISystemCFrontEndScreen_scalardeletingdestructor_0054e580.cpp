struct CFrontEndScreen {
    void* vftable;
};

void __fastcall CFrontEndScreen_dtor(CFrontEndScreen* screen);
void operator delete(void*);

void* __fastcall CFrontEndScreen_scalar_deleting_destructor(CFrontEndScreen* screen, int edx_unused, unsigned int flags)
{
    CFrontEndScreen_dtor(screen);
    if (flags & 1)
        operator delete(screen);
    return screen;
}