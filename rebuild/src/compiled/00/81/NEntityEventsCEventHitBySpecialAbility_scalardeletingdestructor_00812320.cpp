struct CEventHitBySpecialAbility {
    void* vftable;
};

void __fastcall CEventHitBySpecialAbility_dtor(CEventHitBySpecialAbility* event);
void operator delete(void*);

void* __fastcall CEventHitBySpecialAbility_scalar_deleting_destructor(CEventHitBySpecialAbility* event, int edx_unused, unsigned int flags)
{
    CEventHitBySpecialAbility_dtor(event);
    if (flags & 1)
        operator delete(event);
    return event;
}