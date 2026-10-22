// CInputProcessInventoryStatsScreen::`vector deleting destructor'
// Retail 0x0048b620 — MSVC VC7.1 vector deleting destructor.
//
// push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1;
// je +; push esi; call operator_delete; add esp,4; +: mov eax,esi; pop esi; ret 4

// scalar destructor at 0x00a0d3d0 (this in ecx)
void __fastcall CInputProcessInventoryStatsScreen_scalar_dtor(void *process);
// operator delete at 0x00bfe9bc (cdecl)
void __cdecl CInputProcessInventoryStatsScreen_op_delete(void *p);

struct CInputProcessInventoryStatsScreen
{
    void * __fastcall scalar_deleting_destructor(int edx, unsigned int flags);
};

void * __fastcall CInputProcessInventoryStatsScreen::scalar_deleting_destructor(int edx, unsigned int flags)
{
    (void)edx;
    CInputProcessInventoryStatsScreen_scalar_dtor(this);
    if (flags & 1)
        CInputProcessInventoryStatsScreen_op_delete(this);
    return this;
}