#ifndef FABLE_ENGINE_CABILITYTRACKLEVELDEF_H
#define FABLE_ENGINE_CABILITYTRACKLEVELDEF_H
#include "engine/CParentDefClassBase.h"
#include "engine/EHeroAbility.h"
// Layout: Ego_r ExperienceCost/AbilityUnlocked/Description at64/68/72, native60/64/68.
// RTTI1236504 proves CParentDefClassBase; the parent is4 bytes smaller in retail.
class CPersistContext;
class CAbilityTrackLevelDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CABILITYTRACKLEVELDEF_VIRTUALS
    // Retail opt-in (chunk 1015, row 0x00478AE5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackLevelDef>): the def's own
    // overrides of the CDefClassBase pure virtuals (vtable 0x01236504: slot 18 Transfer 0x00478827, 19 Copy 0x00478871, 20
    // GetSizeofClass 0x00478823; Ego_r.pdb ?Transfer@CAbilityTrackLevelDef@@UAEXAAVCPersistContext@@@Z, ?Copy@..@@UAEXPBVCDefClassBase@@@Z,
    // ?GetSizeofClass@..@@UAEKXZ), declarations only, so a default element can be built: the implicit constructor and destructor
    // (0x00478B98 CParentDefClassBase() + the 0x01236504 vtable store, 0x00478BB8 ~CParentDefClassBase). Needs FABLE_RETAIL_DEF_CLASS_BASE.
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
#endif
    long ExperienceCost; EHeroAbility AbilityUnlocked; long Description;
};
FABLE_STATIC_ASSERT(sizeof(CAbilityTrackLevelDef)==0x48);
#endif
