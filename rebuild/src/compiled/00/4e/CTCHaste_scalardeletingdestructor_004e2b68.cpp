#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_004dd4db(void* member);
void* __fastcall CTCHaste_CompositeScalarDeletingDestructor(void* haste, int, unsigned int flags)
{
    DestructMember_004dd4db(static_cast<unsigned char*>(haste) + 24);
    ((CTCBase*)(haste))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(haste);
    return haste;
}