#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCDNavigationSeed_ScalarDeletingDestructor(void* seed, int, unsigned int flags)
{
    ((CTCBase*)(seed))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(seed);
    return seed;
}