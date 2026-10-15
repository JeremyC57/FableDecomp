// CThingAICreature::vfunc_14(bool resuming) -- cutscene hook: when resuming, let the creature's summon-spell component (interface 0x10) resume after the cutscene; otherwise restart the creature's continuous sounds
// Retail: Fable.exe (TLC, VC7.1) @ 0x008337B0, 75 bytes.
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface lookup),
// CTCSummonSpell::ResumeForCutscene @ 0x0066AEB0, CThingCreatureBase::PlayCreatureContinuousSounds @ 0x00665F90.
// Shape: the landed GetInterface out-parameter template; interface 0x10 is bit 16 of bitset word 0
// (`test dword ptr [ecx+20h],10000h`); the `if (resuming)` arm comes first, the else arm is placed last.
// VC7.1 declines to inline the template inside the if-arm here, so it is `__forceinline`.
struct CTCBase;

struct CTCSummonSpell {
    void ResumeForCutscene();                        // 0x0066AEB0
};

struct CInterfaceMap {                               // CVectorMap<ETCInterfaceType, CTCBase*> {first, last}
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);            // 0x0040F020
};

struct CThing {
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};

struct CThingCreatureBase : CThing {
    void PlayCreatureContinuousSounds();             // 0x00665F90
};

struct CThingAICreature : CThingCreatureBase {
    void vfunc_14(bool resuming);
};

void CThingAICreature::vfunc_14(bool resuming)
{
    if (resuming) {
        CTCSummonSpell* summon;
        if (GetInterface(&summon, 0x10))
            summon->ResumeForCutscene();
    } else {
        PlayCreatureContinuousSounds();
    }
}