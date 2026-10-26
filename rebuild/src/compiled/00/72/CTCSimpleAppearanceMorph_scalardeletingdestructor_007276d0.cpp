#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_0071ee70(void* member);
void* __fastcall CTCSimpleAppearanceMorph_CompositeScalarDeletingDestructor(void* morph, int, unsigned int flags)
{
    DestructMember_0071ee70(static_cast<unsigned char*>(morph) + 12);
    ((CTCBase*)(morph))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(morph);
    return morph;
}