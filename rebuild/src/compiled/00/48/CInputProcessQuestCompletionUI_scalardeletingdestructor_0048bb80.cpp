class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessQuestCompletionUI { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CInputProcessQuestCompletionUI_scalar_deleting_destructor(CInputProcessQuestCompletionUI* process, int edx_unused, unsigned int flags)
{
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}