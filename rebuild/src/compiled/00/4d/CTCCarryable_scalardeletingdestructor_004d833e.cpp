#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCCarryable_ScalarDeletingDestructor(void* carryable, int, unsigned int flags)
{
    ((CTCBase*)(carryable))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(carryable);
    return carryable;
}