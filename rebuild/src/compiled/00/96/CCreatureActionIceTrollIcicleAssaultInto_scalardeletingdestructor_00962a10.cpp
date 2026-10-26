class CCreatureAction_IceTrollIcicleAssaultInto { public:
    void* vftable;
};

void __fastcall CCreatureAction_IceTrollIcicleAssaultInto_dtor(CCreatureAction_IceTrollIcicleAssaultInto* action);
void operator delete(void*);

void* __fastcall CCreatureAction_IceTrollIcicleAssaultInto_scalar_deleting_destructor(CCreatureAction_IceTrollIcicleAssaultInto* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_IceTrollIcicleAssaultInto_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}