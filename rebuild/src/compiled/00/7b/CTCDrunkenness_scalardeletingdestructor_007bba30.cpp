class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCDrunkenness { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCDrunkenness_scalar_deleting_destructor(CTCDrunkenness* drunkenness, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(drunkenness))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(drunkenness);
    return drunkenness;
}