#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_00780000(void* member);
void* __fastcall CTCHeroGuide_CompositeScalarDeletingDestructor(void* guide, int, unsigned int flags)
{
    DestructMember_00780000(static_cast<unsigned char*>(guide) + 12);
    ((CTCBase*)(guide))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(guide);
    return guide;
}