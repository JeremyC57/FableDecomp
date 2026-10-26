#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004406d0(void* member);
void* __fastcall CTCFadeOutAndIn_CompositeScalarDeletingDestructor(void* andIn, int, unsigned int flags)
{
    DestructMember_004406d0(static_cast<unsigned char*>(andIn) + 24);
    ((CTCBase*)(andIn))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(andIn);
    return andIn;
}