struct CCombatSequenceGeneric {
    void* vftable;
};

void __fastcall CCombatSequenceGeneric_dtor(CCombatSequenceGeneric* sequence);
void operator delete(void*);

void* __fastcall CCombatSequenceGeneric_scalar_deleting_destructor(CCombatSequenceGeneric* sequence, int edx_unused, unsigned int flags)
{
    CCombatSequenceGeneric_dtor(sequence);
    if (flags & 1)
        operator delete(sequence);
    return sequence;
}