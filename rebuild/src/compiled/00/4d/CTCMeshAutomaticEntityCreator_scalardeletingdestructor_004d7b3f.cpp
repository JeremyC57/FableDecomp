#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
void* __fastcall CTCMeshAutomaticEntityCreator_ScalarDeletingDestructor(void* creator, int, unsigned int flags)
{
    ((CTCBase*)(creator))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(creator);
    return creator;
}