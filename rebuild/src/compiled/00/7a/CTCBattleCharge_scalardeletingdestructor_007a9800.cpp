struct CTCBattleCharge {
    void* vftable;
};

void __fastcall CTCBattleCharge_dtor(CTCBattleCharge* charge);
void operator delete(void*);

void* __fastcall CTCBattleCharge_scalar_deleting_destructor(CTCBattleCharge* charge, int edx_unused, unsigned int flags)
{
    CTCBattleCharge_dtor(charge);
    if (flags & 1)
        operator delete(charge);
    return charge;
}