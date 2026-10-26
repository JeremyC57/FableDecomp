#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_0099eae0(void* member);
void* __fastcall CTCRotate_CompositeScalarDeletingDestructor(void* rotate, int, unsigned int flags)
{
    DestructMember_0099eae0(static_cast<unsigned char*>(rotate) + 44);
    ((CTCBase*)(rotate))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(rotate);
    return rotate;
}