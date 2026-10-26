class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct CTCTimeAppearanceFade {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCTimeAppearanceFade_scalar_deleting_destructor(CTCTimeAppearanceFade* fade, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(fade))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(fade);
    return fade;
}