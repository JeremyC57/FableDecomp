struct CTCUserActionPointBase {
    void* vftable;
};

void __fastcall CTCUserActionPointBase_dtor(CTCUserActionPointBase* point);
void operator delete(void*);

void* __fastcall CTCUserActionPointBase_scalar_deleting_destructor(CTCUserActionPointBase* point, int edx_unused, unsigned int flags)
{
    CTCUserActionPointBase_dtor(point);
    if (flags & 1)
        operator delete(point);
    return point;
}