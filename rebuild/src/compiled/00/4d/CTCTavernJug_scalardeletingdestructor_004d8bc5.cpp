#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCTavernJug_ScalarDeletingDestructor(void* jug, int, unsigned int flags)
{
    ((CTCBase*)(jug))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(jug);
    return jug;
}