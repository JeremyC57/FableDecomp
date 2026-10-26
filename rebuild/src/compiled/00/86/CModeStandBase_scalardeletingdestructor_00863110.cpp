struct CModeStandBase {
    void* vftable;

    virtual ~CModeStandBase();   // 0x0073D860
};

void operator delete(void*);

void* __fastcall CModeStandBase_scalar_deleting_destructor(CModeStandBase* mode, int edx_unused, unsigned int flags)
{
    ((CModeStandBase*)(mode))->CModeStandBase::~CModeStandBase();
    if (flags & 1)
        operator delete(mode);
    return mode;
}