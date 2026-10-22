struct CCreatureAction_IceTrollIcicleAssaultLoop {
    void* vftable;
};

void __fastcall CCreatureAction_IceTrollIcicleAssaultLoop_dtor(CCreatureAction_IceTrollIcicleAssaultLoop* action);
void operator delete(void*);

void* __fastcall CCreatureAction_IceTrollIcicleAssaultLoop_scalar_deleting_destructor(CCreatureAction_IceTrollIcicleAssaultLoop* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_IceTrollIcicleAssaultLoop_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}