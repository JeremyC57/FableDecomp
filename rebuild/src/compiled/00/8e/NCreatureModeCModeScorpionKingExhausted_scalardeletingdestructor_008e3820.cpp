#pragma optimize("t", on)
class CModeStandBase {
public:
    virtual ~CModeStandBase();   // 0x0073D860
};
extern void __fastcall DestructMember_00739db0(void* member);
void* __fastcall CModeScorpionKingExhausted_CompositeScalarDeletingDestructor(void* mode, int, unsigned int flags)
{
    DestructMember_00739db0(static_cast<unsigned char*>(mode) + 108);
    ((CModeStandBase*)(mode))->CModeStandBase::~CModeStandBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}