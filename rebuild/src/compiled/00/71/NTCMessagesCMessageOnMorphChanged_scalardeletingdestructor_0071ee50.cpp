struct CMessageOnMorphChanged {
    void* vftable;
};

void __fastcall CMessageOnMorphChanged_dtor(CMessageOnMorphChanged* message);
void operator delete(void*);

void* __fastcall CMessageOnMorphChanged_scalar_deleting_destructor(CMessageOnMorphChanged* message, int edx_unused, unsigned int flags)
{
    CMessageOnMorphChanged_dtor(message);
    if (flags & 1)
        operator delete(message);
    return message;
}