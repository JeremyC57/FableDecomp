class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCTavern { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCTavern_scalar_deleting_destructor(CTCTavern* tavern, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(tavern))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(tavern);
    return tavern;
}