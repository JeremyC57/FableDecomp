class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessPhotojournalCapture { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CInputProcessPhotojournalCapture_scalar_deleting_destructor(CInputProcessPhotojournalCapture* process, int edx_unused, unsigned int flags)
{
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}