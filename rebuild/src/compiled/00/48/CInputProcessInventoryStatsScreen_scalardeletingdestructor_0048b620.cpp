// CInputProcessInventoryStatsScreen::`vector deleting destructor'
// Retail 0x0048b620 — MSVC VC7.1 vector deleting destructor.
//
// push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1;
// je +; push esi; call operator_delete; add esp,4; +: mov eax,esi; pop esi; ret 4

// scalar destructor at 0x00a0d3d0 (this in ecx)
// operator delete at 0x00bfe9bc (cdecl)
class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};

class CInputProcessInventoryStatsScreen
{ public:
    void * __fastcall scalar_deleting_destructor(int edx, unsigned int flags);
};

void * __fastcall CInputProcessInventoryStatsScreen::scalar_deleting_destructor(int edx, unsigned int flags)
{
    (void)edx;
    ((CAInputProcess*)(this))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(this);
    return this;
}