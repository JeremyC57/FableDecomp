#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCGift_ScalarDeletingDestructor(void* gift, int, unsigned int flags)
{
    ((CTCBase*)(gift))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(gift);
    return gift;
}