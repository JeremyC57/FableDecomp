class CTCInventoryQuests { public:
    void* vftable;
};

void __fastcall CTCInventoryQuests_dtor(CTCInventoryQuests* quests);
void operator delete(void*);

void* __fastcall CTCInventoryQuests_scalar_deleting_destructor(CTCInventoryQuests* quests, int edx_unused, unsigned int flags)
{
    CTCInventoryQuests_dtor(quests);
    if (flags & 1)
        operator delete(quests);
    return quests;
}