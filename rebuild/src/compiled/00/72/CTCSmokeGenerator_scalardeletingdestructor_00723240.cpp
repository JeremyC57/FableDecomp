#pragma optimize("t", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
extern void __fastcall DestructMember_00722eb0(void* member);
void* __fastcall CTCSmokeGenerator_CompositeScalarDeletingDestructor(void* generator, int, unsigned int flags)
{
    DestructMember_00722eb0(static_cast<unsigned char*>(generator) + 16);
    ((CTCBase*)(generator))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(generator);
    return generator;
}