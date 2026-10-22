struct CEventReceivedItem {
    void* vftable;
};

void __fastcall CEventReceivedItem_dtor(CEventReceivedItem* event);
void operator delete(void*);

void* __fastcall CEventReceivedItem_scalar_deleting_destructor(CEventReceivedItem* event, int edx_unused, unsigned int flags)
{
    CEventReceivedItem_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}