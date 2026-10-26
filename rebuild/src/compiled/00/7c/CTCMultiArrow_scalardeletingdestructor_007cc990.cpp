class CTCMultiArrow { public:
    void* vftable;
};

void __fastcall CTCMultiArrow_dtor(CTCMultiArrow* arrow);
void operator delete(void*);

void* __fastcall CTCMultiArrow_scalar_deleting_destructor(CTCMultiArrow* arrow, int edx_unused, unsigned int flags)
{
    CTCMultiArrow_dtor(arrow);
    if (flags & 1)
        operator delete(arrow);
    return arrow;
}