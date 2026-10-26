#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCCupboard_ScalarDeletingDestructor(void* cupboard, int, unsigned int flags)
{
    ((CTCBase*)(cupboard))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(cupboard);
    return cupboard;
}