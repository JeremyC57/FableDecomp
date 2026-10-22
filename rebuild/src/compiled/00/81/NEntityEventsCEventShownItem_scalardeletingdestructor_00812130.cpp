struct CEventShownItem {
    void* vftable;
};

void __fastcall CEventShownItem_dtor(CEventShownItem* event);
void operator delete(void*);

void* __fastcall CEventShownItem_scalar_deleting_destructor(CEventShownItem* event, int edx_unused, unsigned int flags)
{
    CEventShownItem_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}