#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCOnHeroPush_ScalarDeletingDestructor(void* push, int, unsigned int flags)
{
    ((CTCBase*)(push))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(push);
    return push;
}