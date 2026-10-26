#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCAbsorbHits_ScalarDeletingDestructor(void* hits, int, unsigned int flags)
{
    ((CTCBase*)(hits))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(hits);
    return hits;
}