#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCCreatureHive_ScalarDeletingDestructor(void* hive, int, unsigned int flags)
{
    ((CTCBase*)(hive))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(hive);
    return hive;
}