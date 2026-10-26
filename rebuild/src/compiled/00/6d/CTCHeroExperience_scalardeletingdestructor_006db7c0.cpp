class CTCHeroExperience { public:
    void* vftable;
};

void __fastcall CTCHeroExperience_dtor(CTCHeroExperience* experience);
void operator delete(void*);

void* __fastcall CTCHeroExperience_scalar_deleting_destructor(CTCHeroExperience* experience, int edx_unused, unsigned int flags)
{
    CTCHeroExperience_dtor(experience);
    if (flags & 1)
        operator delete(experience);
    return experience;
}