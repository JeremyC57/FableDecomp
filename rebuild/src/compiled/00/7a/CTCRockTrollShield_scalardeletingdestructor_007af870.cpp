class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
struct CTCRockTrollShield {
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCRockTrollShield_scalar_deleting_destructor(CTCRockTrollShield* shield, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(shield))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(shield);
    return shield;
}