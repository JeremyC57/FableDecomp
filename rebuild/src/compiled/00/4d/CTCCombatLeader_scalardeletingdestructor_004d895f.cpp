#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCCombatLeader_ScalarDeletingDestructor(void* leader, int, unsigned int flags)
{
    ((CTCBase*)(leader))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(leader);
    return leader;
}