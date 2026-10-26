#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_00756ab0(void* member);
void* __fastcall CTCHeroRememberCrimes_CompositeScalarDeletingDestructor(void* crimes, int, unsigned int flags)
{
    DestructMember_00756ab0(static_cast<unsigned char*>(crimes) + 12);
    ((CTCBase*)(crimes))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(crimes);
    return crimes;
}