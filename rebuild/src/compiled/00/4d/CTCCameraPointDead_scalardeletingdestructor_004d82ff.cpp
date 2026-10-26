#pragma optimize("s", on)
class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct CTCCameraPointDead
{
    void* vftable;
};
extern void* const g_CTCCameraPointDead_vftable;
void operator delete(void*);
void* __fastcall scalar_deleting_destructor(CTCCameraPointDead* dead, int, unsigned int flags)
{
    dead->vftable = (void*)&g_CTCCameraPointDead_vftable;
    ((CTCBase*)(dead))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(dead);
    return dead;
}