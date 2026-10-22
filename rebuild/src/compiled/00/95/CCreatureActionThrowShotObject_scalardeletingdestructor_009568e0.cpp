struct CCreatureAction_ThrowShotObject {
    void* vftable;
};

void __fastcall CCreatureAction_ThrowShotObject_dtor(CCreatureAction_ThrowShotObject* action);
void operator delete(void*);

void* __fastcall CCreatureAction_ThrowShotObject_scalar_deleting_destructor(CCreatureAction_ThrowShotObject* action, int edx_unused, unsigned int flags)
{
    CCreatureAction_ThrowShotObject_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}