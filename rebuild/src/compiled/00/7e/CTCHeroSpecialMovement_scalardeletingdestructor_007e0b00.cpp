class CTCHeroSpecialMovement { public:
    void* vftable;
};

void __fastcall CTCHeroSpecialMovement_dtor(CTCHeroSpecialMovement* movement);
void operator delete(void*);

void* __fastcall CTCHeroSpecialMovement_scalar_deleting_destructor(CTCHeroSpecialMovement* movement, int edx_unused, unsigned int flags)
{
    CTCHeroSpecialMovement_dtor(movement);
    if (flags & 1)
        operator delete(movement);
    return movement;
}