struct CSimplePointBase {
    void* vftable;
};

void __fastcall CSimplePointBase_dtor(CSimplePointBase* point);
void operator delete(void*);

void* __fastcall CSimplePointBase_scalar_deleting_destructor(CSimplePointBase* point, int edx_unused, unsigned int flags)
{
    CSimplePointBase_dtor(point);
    if (flags & 1)
        operator delete(point);
    return point;
}