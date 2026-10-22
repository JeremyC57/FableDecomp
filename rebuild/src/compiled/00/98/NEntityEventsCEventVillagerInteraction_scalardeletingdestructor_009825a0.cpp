struct CEventVillagerInteraction {
    void* vftable;
};

void __fastcall CEventVillagerInteraction_dtor(CEventVillagerInteraction* event);
void operator delete(void*);

void* __fastcall CEventVillagerInteraction_scalar_deleting_destructor(CEventVillagerInteraction* event, int edx_unused, unsigned int flags)
{
    CEventVillagerInteraction_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}