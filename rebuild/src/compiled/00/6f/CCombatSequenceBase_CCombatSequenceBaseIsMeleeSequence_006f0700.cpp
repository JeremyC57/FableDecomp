struct CCombatSequenceBase {
    struct VTable {
        void* slot0;
        void* slot4;
        void* slot8;
        int (__fastcall *GetSequenceType)(CCombatSequenceBase* sequence);
    };
    VTable* vfptr;
};

int __fastcall CCombatSequenceBase_IsMeleeSequence(CCombatSequenceBase* sequence, void* edx)
{
    return sequence->vfptr->GetSequenceType(sequence) == 1;
}