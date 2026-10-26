#pragma optimize("t", on)
class CModeStandBase {
public:
    virtual ~CModeStandBase();   // 0x0073D860
};
extern void __fastcall DestructMember_00739df0(void* member);
void* __fastcall CModeStandRight_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739df0(static_cast<unsigned char*>(mode) + 104);
    ((CModeStandBase*)(mode))->CModeStandBase::~CModeStandBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}