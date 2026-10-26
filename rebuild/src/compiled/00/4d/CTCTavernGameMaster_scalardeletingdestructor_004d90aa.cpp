#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCTavernGameMaster_ScalarDeletingDestructor(void* master, int, unsigned int flags)
{
    ((CTCBase*)(master))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(master);
    return master;
}