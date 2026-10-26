// CInputProcessControlCreatureActivateZTarget::`vector_deleting_destructor'
// __fastcall (this in ecx), uint flags on stack, returns this.

class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessControlCreatureActivateZTarget { public:
    // scalar destructor lives at 0xa0d3d0 in retail; model as an extern.
};


void* __fastcall CInputProcessControlCreatureActivateZTarget_scalar_deleting_destructor(
        CInputProcessControlCreatureActivateZTarget* process, int /*edx*/, unsigned int flags)
{
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}