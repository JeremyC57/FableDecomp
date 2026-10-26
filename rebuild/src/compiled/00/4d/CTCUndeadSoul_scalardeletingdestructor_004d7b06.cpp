#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCUndeadSoul_ScalarDeletingDestructor(void* soul, int, unsigned int flags)
{
    ((CTCBase*)(soul))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(soul);
    return soul;
}