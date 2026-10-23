extern "C" void __fastcall MemberDtor_686df0_00cbeeb0(void* self);
extern "C" void __fastcall BaseDtor_686830_009b02e0(void* self);

struct CInputProcessManager {
    void** vtbl;
};

extern const void* const g_vtbl_CInputProcessManager_012c2648[];

extern "C" void __fastcall DtorInputProcessManager(CInputProcessManager* self, int /*edx*/)
{
    self->vtbl = (void**)&g_vtbl_CInputProcessManager_012c2648;
    MemberDtor_686df0_00cbeeb0(self);
    BaseDtor_686830_009b02e0(self);
}