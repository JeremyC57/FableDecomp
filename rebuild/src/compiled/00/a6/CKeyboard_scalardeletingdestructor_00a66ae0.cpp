struct CKeyboard {
    void* vftable;
};

void __fastcall CKeyboard_dtor(CKeyboard* keyboard);
void operator delete(void*);

void* __fastcall CKeyboard_scalar_deleting_destructor(CKeyboard* keyboard, int edx_unused, unsigned int flags)
{
    CKeyboard_dtor(keyboard);
    if (flags & 1)
        operator delete(keyboard);
    return keyboard;
}