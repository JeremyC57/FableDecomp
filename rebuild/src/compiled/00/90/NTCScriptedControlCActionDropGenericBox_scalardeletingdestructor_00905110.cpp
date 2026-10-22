struct CActionDropGenericBox {
    void* vftable;
};

void __fastcall CActionDropGenericBox_dtor(CActionDropGenericBox* action);
void operator delete(void*);

void* __fastcall CActionDropGenericBox_scalar_deleting_destructor(CActionDropGenericBox* action, int edx_unused, unsigned int flags)
{
    CActionDropGenericBox_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}