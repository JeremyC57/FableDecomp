class CCreatureAction_IceTrollIcicleAssaultOutOf { public:
    void* vftable;
};

void __fastcall CCreatureAction_IceTrollIcicleAssaultOutOf_dtor(CCreatureAction_IceTrollIcicleAssaultOutOf* action);
void operator delete(void*);

void* __fastcall CCreatureAction_IceTrollIcicleAssaultOutOf_scalar_deleting_destructor(CCreatureAction_IceTrollIcicleAssaultOutOf* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_IceTrollIcicleAssaultOutOf_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}