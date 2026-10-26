class CTCBase {
public:
    virtual ~CTCBase();   // 0x00686830
};
class CTCElectrocuteOnTouch { public:
    void* vftable;
};

void operator delete(void*);

void* __fastcall CTCElectrocuteOnTouch_scalar_deleting_destructor(CTCElectrocuteOnTouch* touch, int edx_unused, unsigned int flags)
{
    ((CTCBase*)(touch))->CTCBase::~CTCBase();
    if (flags & 1)
        operator delete(touch);
    return touch;
}