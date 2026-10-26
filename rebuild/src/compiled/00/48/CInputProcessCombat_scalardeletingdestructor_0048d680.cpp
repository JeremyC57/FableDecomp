#pragma optimize("t", on)
class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
extern void __fastcall DestructMember_0048d520(void* member);
void* __fastcall CInputProcessCombat_CompositeScalarDeletingDestructor(void* process, int, unsigned int flags)
{
    DestructMember_0048d520(static_cast<unsigned char*>(process) + 40);
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}