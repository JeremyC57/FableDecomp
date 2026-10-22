struct CInputProcessCreatureMovementWatchForControlAngleChange {
    void* vftable;
};

void __fastcall CInputProcessCreatureMovementWatchForControlAngleChange_dtor(CInputProcessCreatureMovementWatchForControlAngleChange* process);
void operator delete(void*);

void* __fastcall CInputProcessCreatureMovementWatchForControlAngleChange_scalar_deleting_destructor(CInputProcessCreatureMovementWatchForControlAngleChange* process, int edx_unused, unsigned int flags)
{
    CInputProcessCreatureMovementWatchForControlAngleChange_dtor(process);
    if (flags & 1)
        operator delete(process);
    return process;
}