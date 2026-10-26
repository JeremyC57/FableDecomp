class CInputProcessControlCreatureRightStick { public:
    void* vftable;
};

void __fastcall CInputProcessControlCreatureRightStick_dtor(CInputProcessControlCreatureRightStick* process);
void operator delete(void*);

void* __fastcall CInputProcessControlCreatureRightStick_scalar_deleting_destructor(CInputProcessControlCreatureRightStick* process, int edx_unused, unsigned int flags)
{
    CInputProcessControlCreatureRightStick_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}