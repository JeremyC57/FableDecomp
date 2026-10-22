struct CSlider {
    void* vftable;
};

void __fastcall CSlider_dtor(CSlider* slider);
void operator delete(void*);

void* __fastcall CSlider_scalar_deleting_destructor(CSlider* slider, int edx_unused, unsigned int flags)
{
    CSlider_dtor(slider);
    if (flags & 1)
        operator delete(slider);
    return slider;
}