class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessCreatureMovementWatchForControlAngleChange { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CInputProcessCreatureMovementWatchForControlAngleChange_scalar_deleting_destructor(CInputProcessCreatureMovementWatchForControlAngleChange* process, int edx_unused, unsigned int flags)
{
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}