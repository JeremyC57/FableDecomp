// CInputProcessInventoryQuestsScreen::`vector_deleting_destructor' 0x0048b5e0
// Standard MSVC vector deleting destructor shape:
//   push esi; mov esi,ecx; call scalar_dtor; test [esp+8],1; je; push esi; call op_delete; add esp,4; mov eax,esi; ret 4

class CAInputProcess {
public:
    virtual ~CAInputProcess();   // 0x00A0D3D0
};
class CInputProcessInventoryQuestsScreen;

// scalar destructor (direct non-virtual call) @ 0xa0d3d0
// operator delete @ 0xbfe9bc

// vector deleting destructor, this in ecx, flags pushed on stack.
void* __fastcall scalar_deleting_destructor(CInputProcessInventoryQuestsScreen* process, int edx, unsigned int flags)
{
    (void)edx;
    ((CAInputProcess*)(process))->CAInputProcess::~CAInputProcess();
    if (flags & 1)
        operator delete(process);
    return process;
}