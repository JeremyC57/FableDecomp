class CTCTargetingBase { public:
    void* vftable;
};

void __fastcall CTCTargetingBase_dtor(CTCTargetingBase* targeting);
void operator delete(void*);

void* __fastcall CTCTargetingBase_scalar_deleting_destructor(CTCTargetingBase* targeting, int edx_unused, unsigned int flags)
{
    CTCTargetingBase_dtor(targeting);
    if (flags & 1)
        operator delete(targeting);
    return targeting;
}