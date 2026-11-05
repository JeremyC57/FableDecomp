// CTCSpecialAbilities::ResumeAbilitiesForCutscene() -- after a cutscene, resume each active spell component the hero has (interfaces 0x93, 0x91, 0x9A, 0x88)
// Leaf: ResumeAbilitiesForCutscene
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x0072BEC0, 245 bytes (plain `ret`).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (four inlined lookups), the components' ResumeForCutscene (manifest labels): CTCPhysicalShieldSpell 0x0079E520
//   (interface 0x93), CTCSummonSpell::ResumeForCutscene 0x0079A6A0 (interface 0x91, landed), CTCGhostSword 0x007C54F0 (0x9A),
//   CTCMultiArrow 0x007CC230 (interface 0x88).
// Layout: retail CThing interface bitset +0x20 (0x93 = word +0x30 bit 19, 0x91 = bit 17, 0x9A = bit 26, 0x88 = bit 8), map
//   +0x44; CTCSpecialAbilities ParentThing +0x04.
// Shape: four `if (HasTC) GetTC()->ResumeForCutscene();` statements, each re-reading ParentThing; the shared key word.
class CTCBase;
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCPhysicalShieldSpell { public: void ResumeForCutscene(); };        // 0x0079E520 (interface 0x93)
class CTCSummonSpell { public: void ResumeForCutscene(); };    // 0x0079A6A0 (interface 0x91)
class CTCGhostSword { public: void ResumeForCutscene(); };        // 0x007C54F0 (interface 0x9A)
class CTCMultiArrow { public: void ResumeForCutscene(); };        // 0x007CC230 (interface 0x88)
class CTCSpecialAbilities { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04 (CTCBase)
    void ResumeAbilitiesForCutscene();
};

void CTCSpecialAbilities::ResumeAbilitiesForCutscene()
{
    if (ParentThing->HasTC(TCI_PHYSICAL_SHIELD_SPELL))
        ((CTCPhysicalShieldSpell*)&ParentThing->GetTC(TCI_PHYSICAL_SHIELD_SPELL))->ResumeForCutscene();
    if (ParentThing->HasTC(TCI_SUMMON_SPELL))
        ((CTCSummonSpell*)&ParentThing->GetTC(TCI_SUMMON_SPELL))->ResumeForCutscene();
    if (ParentThing->HasTC(TCI_GHOST_SWORD))
        ((CTCGhostSword*)&ParentThing->GetTC(TCI_GHOST_SWORD))->ResumeForCutscene();
    if (ParentThing->HasTC(TCI_MULTI_ARROW))
        ((CTCMultiArrow*)&ParentThing->GetTC(TCI_MULTI_ARROW))->ResumeForCutscene();
}