class CTCHeroReceiveItems { public:
    void* vftable;
};

void __fastcall CTCHeroReceiveItems_dtor(CTCHeroReceiveItems* items);
void operator delete(void*);

void* __fastcall CTCHeroReceiveItems_scalar_deleting_destructor(CTCHeroReceiveItems* items, int edx_unused, unsigned int flags)
{
    CTCHeroReceiveItems_dtor(items);
    if (flags & 1)
        operator delete(items);
    return items;
}