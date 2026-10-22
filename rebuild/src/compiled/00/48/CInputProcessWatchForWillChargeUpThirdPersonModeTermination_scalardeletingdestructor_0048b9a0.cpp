struct CInputProcessWatchForWillChargeUpThirdPersonModeTermination {
    void* vftable;
};

void __fastcall CInputProcessWatchForWillChargeUpThirdPersonModeTermination_dtor(CInputProcessWatchForWillChargeUpThirdPersonModeTermination* process);
void operator delete(void*);

void* __fastcall CInputProcessWatchForWillChargeUpThirdPersonModeTermination_scalar_deleting_destructor(CInputProcessWatchForWillChargeUpThirdPersonModeTermination* process, int edx_unused, unsigned int flags)
{
    CInputProcessWatchForWillChargeUpThirdPersonModeTermination_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}