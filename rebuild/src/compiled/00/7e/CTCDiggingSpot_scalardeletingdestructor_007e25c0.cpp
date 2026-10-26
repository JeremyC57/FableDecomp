class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCDiggingSpot { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCDiggingSpot_scalar_deleting_destructor(CTCDiggingSpot* spot, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(spot))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(spot);
    return spot;
}