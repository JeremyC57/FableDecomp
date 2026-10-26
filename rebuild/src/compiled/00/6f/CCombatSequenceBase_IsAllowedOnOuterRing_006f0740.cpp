struct CCombatSequenceBase_vtbl
{
    void* slots[16];
};

class CCombatSequenceBase
{ public:
    CCombatSequenceBase_vtbl* vtbl;
};

typedef unsigned int (__fastcall *IsAllowedOnOuterRing_fn)(CCombatSequenceBase* sequence, void* edx);

unsigned int __fastcall CCombatSequenceBase_IsAllowedOnOuterRing(CCombatSequenceBase* sequence, void* edx)
{
    return (((IsAllowedOnOuterRing_fn)(*(void***)sequence)[0xF])(sequence, edx) >> 7) & 1;
}