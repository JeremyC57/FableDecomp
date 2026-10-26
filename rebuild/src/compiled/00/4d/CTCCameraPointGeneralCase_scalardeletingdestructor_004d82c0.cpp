#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCCameraPointGeneralCase
{ public:
    void* vftable;
};
extern void* const g_CTCCameraPointGeneralCase_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointGeneralCase* generalCase, int, unsigned int flags)
{
    generalCase->vftable = (void*)&g_CTCCameraPointGeneralCase_vftable;
    ((CTCBase*)(generalCase))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(generalCase);
    return generalCase;
}