struct CCombatSequence_WillPhysicalShieldNonMelee {
    void* vftable;
};

void __fastcall CCombatSequence_WillPhysicalShieldNonMelee_dtor(CCombatSequence_WillPhysicalShieldNonMelee* sequence);
void operator delete(void*);

void* __fastcall CCombatSequence_WillPhysicalShieldNonMelee_scalar_deleting_destructor(CCombatSequence_WillPhysicalShieldNonMelee* sequence, int edx_unused, unsigned int flags)
{
    CCombatSequence_WillPhysicalShieldNonMelee_dtor(sequence);
    if (flags & 1)
        operator delete(sequence);
    return sequence;
}