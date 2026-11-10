#ifndef FABLE_ENGINE_CABILITYTRACKDEF_H
#define FABLE_ENGINE_CABILITYTRACKDEF_H
#include "engine/CAbilityTrackLevelDef.h"
#include "engine/EHeroExperienceStatCategory.h"
#include "stlport/fable_containers.h"
// Layout: Ego_r ExperienceStat64/Abilities68/MaxedOutDescription84 -> native60/64/76.
// Native stat access multiplies index by80; RTTI123649c proves CParentDefClassBase.
class CPersistContext;
class CAbilityTrackDef : public CParentDefClassBase {
public:
#ifdef FABLE_RETAIL_CABILITYTRACKDEF_MEMBERS
    // Retail opt-in (chunk 1015, row 0x0047A2F5 CPersistContext::TransferVectorOfSubComponents<CAbilityTrackDef>): the out-of-line
    // constructor 0x0047A3EA (??0CAbilityTrackDef@@QAE@XZ, landed) and destructor 0x00478722 (FableWin.pdb ??1CAbilityTrackDef@@UAE@XZ,
    // virtual through CParentDefClassBase; 0x0047A3A8 / 0x0047A3BE build and destroy the default element with them) and the def's own
    // overrides (vtable 0x0123649C: slot 0 0x00478706, 18 Transfer 0x00478AB7 (landed), 19 Copy 0x00478A68 (landed), 20 GetSizeofClass
    // 0x00478702; Ego_r.pdb spellings). Declarations only. Needs FABLE_RETAIL_DEF_CLASS_BASE.
    CAbilityTrackDef();                                      // 0x0047A3EA
    virtual ~CAbilityTrackDef();                             // 0x00478722
    virtual void Transfer(CPersistContext&);                 // slot 18
    virtual void Copy(const CDefClassBase*);                 // slot 19
    virtual unsigned long GetSizeofClass();                  // slot 20
#endif
    EHeroExperienceStatCategory ExperienceStat; std::vector<CAbilityTrackLevelDef> Abilities; long MaxedOutDescription;
};
FABLE_STATIC_ASSERT(sizeof(CAbilityTrackDef)==0x50);
#endif
