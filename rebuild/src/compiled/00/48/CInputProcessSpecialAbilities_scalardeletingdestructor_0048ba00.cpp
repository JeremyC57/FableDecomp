class CInputProcessSpecialAbilities { public:
    void* vftable;
};

void __fastcall CInputProcessSpecialAbilities_dtor(CInputProcessSpecialAbilities* process);
void operator delete(void*);

void* __fastcall CInputProcessSpecialAbilities_scalar_deleting_destructor(CInputProcessSpecialAbilities* process, int edx_unused, unsigned int flags)
{
    CInputProcessSpecialAbilities_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}