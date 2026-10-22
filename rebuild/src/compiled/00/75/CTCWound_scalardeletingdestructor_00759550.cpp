struct CTCWound {
    void* vftable;
};

void __fastcall CTCWound_dtor(CTCWound* wound);
void operator delete(void*);

void* __fastcall CTCWound_scalar_deleting_destructor(CTCWound* wound, int edx_unused, unsigned int flags)
{
    CTCWound_dtor(wound);
    if (flags & 1)
        operator delete(wound);
    return wound;
}