#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCBlacksmiths_ScalarDeletingDestructor(void* blacksmiths, int, unsigned int flags)
{
    ((CTCBase*)(blacksmiths))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(blacksmiths);
    return blacksmiths;
}