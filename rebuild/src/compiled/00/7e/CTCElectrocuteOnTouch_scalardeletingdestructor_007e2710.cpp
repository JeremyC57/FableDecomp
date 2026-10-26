class CTCElectrocuteOnTouch { public:
    void* vftable;
};

void __fastcall CTCElectrocuteOnTouch_dtor(CTCElectrocuteOnTouch* touch);
void operator delete(void*);

void* __fastcall CTCElectrocuteOnTouch_scalar_deleting_destructor(CTCElectrocuteOnTouch* touch, int edx_unused, unsigned int flags)
{
    CTCElectrocuteOnTouch_dtor(touch);
    if (flags & 1)
        operator delete(touch);
    return touch;
}