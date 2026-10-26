#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCAmbientBird_ScalarDeletingDestructor(void* bird, int, unsigned int flags)
{
    ((CTCBase*)(bird))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(bird);
    return bird;
}