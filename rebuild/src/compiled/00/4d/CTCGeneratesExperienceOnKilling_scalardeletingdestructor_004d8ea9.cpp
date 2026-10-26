#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCGeneratesExperienceOnKilling_ScalarDeletingDestructor(void* killing, int, unsigned int flags)
{
    ((CTCBase*)(killing))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(killing);
    return killing;
}