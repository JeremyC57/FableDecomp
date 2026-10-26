class CTCSpecialAbilities { public:
    void* vftable;
};

void __fastcall CTCSpecialAbilities_dtor(CTCSpecialAbilities* abilities);
void operator delete(void*);

void* __fastcall CTCSpecialAbilities_scalar_deleting_destructor(CTCSpecialAbilities* abilities, int edx_unused, unsigned int flags)
{
    CTCSpecialAbilities_dtor(abilities);
    if (flags & 1)
        operator delete(abilities);
    return abilities;
}