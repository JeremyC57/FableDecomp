class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCCameraPointScripted { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCCameraPointScripted_scalar_deleting_destructor(CTCCameraPointScripted* scripted, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(scripted))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(scripted);
    return scripted;
}