// CThingAICreature::vfunc_14(bool resuming) -- cutscene hook: when resuming, let the creature's summon-spell component (interface 0x10) resume after the cutscene; otherwise restart the creature's continuous sounds
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008337B0, 75 bytes.
// Header: engine/CThingAICreature.h exists but names nothing at these offsets (all `_pad_*`, PDB offsets that
// sit 0x30 above retail at the tail), so the class is modelled locally like the landed 0x00833BA0.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (inlined GetInterface lookup),
// CTCSummonSpell::ResumeForCutscene @ 0x0066AEB0, CThingCreatureBase::PlayCreatureContinuousSounds @ 0x00665F90.
// Shape: the landed GetInterface out-parameter template; interface 0x10 is bit 16 of bitset word 0
// (`test dword ptr [ecx+20h],10000h`); the `if (resuming)` arm comes first, the else arm is placed last.
// VC7.1 declines to inline the template inside the if-arm here, so it is `__forceinline`.
#include "fable_compiler.h"
class CTCBase;

class CTCSummonSpell { public:
};

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    void*               __vftable;                   // +0x00
    unsigned char       _pad04[0x20 - 0x04];
    unsigned long       UsingComponents[9];          // +0x20 bitset<274>
    CInterfaceMap       Components;                  // +0x44
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (UsingComponents[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &Components;
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CThingCreatureBase : public CThing { public:
protected:
    void PlayCreatureContinuousSounds();             // 0x00665F90; NF236: Ego_r / FableWin spelling IAE (protected)
public:
};

class CThingAICreature : public CThingCreatureBase { public:
    void vfunc_14(bool resuming);
};

// Naming (NF438 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration (no layout; rulings r2/r3):
//   0x0066AEB0 ?StopAllAttachedSounds@CTCSoundPlayer@@QAEXXZ (was ?ResumeForCutscene_0066aeb0@CTCSummonSpell@@QAEXXZ)
class CTCSoundPlayer { public: void StopAllAttachedSounds(); };
void CThingAICreature::vfunc_14(bool resuming)
{
    if (resuming) {
        CTCSummonSpell* summon;
        if (GetInterface(&summon, 0x10))
            reinterpret_cast<CTCSoundPlayer*>(summon)->StopAllAttachedSounds();
    } else {
        PlayCreatureContinuousSounds();
    }
}