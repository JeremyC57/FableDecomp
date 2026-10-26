class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessWatchForRangedWeaponThirdPersonModeTermination { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CInputProcessWatchForRangedWeaponThirdPersonModeTermination_scalar_deleting_destructor(CInputProcessWatchForRangedWeaponThirdPersonModeTermination* process, int edx_unused, unsigned int flags)
{
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}