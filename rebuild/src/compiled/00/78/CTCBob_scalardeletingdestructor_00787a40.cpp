class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCBob { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCBob_scalar_deleting_destructor(CTCBob* bob, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(bob))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(bob);
    return bob;
}