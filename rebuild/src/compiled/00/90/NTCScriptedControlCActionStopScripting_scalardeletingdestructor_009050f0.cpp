struct CActionStopScripting {
    void* vftable;
};

void __fastcall CActionStopScripting_dtor(CActionStopScripting* action);
void operator delete(void*);

void* __fastcall CActionStopScripting_scalar_deleting_destructor(CActionStopScripting* action, int edx_unused, unsigned int flags)
{
    CActionStopScripting_dtor(action);
    if (flags & 1)
        operator delete(action);
    return action;
}