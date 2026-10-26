class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCAppearanceMorphBase { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCAppearanceMorphBase_scalar_deleting_destructor(CTCAppearanceMorphBase* morph, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(morph))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(morph);
    return morph;
}