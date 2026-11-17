// CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(EOpinionAttitudeType attitude) -- when the wife has noticed an appearance change, say the
// Leaf: ?WifeClothingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// matching "<category prefix><category suffix>" line to the hero; on success clear the noticed flag.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00985F40, length 201 bytes (`ret 4`). PDB: ?WifeClothingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z
// -- the attitude argument selects the line suffix (retail reads the argument slot `mov edx,[esp+14h]` for the second
// table index); the category out value is declared inside the interface test, so it shares the lookup key's frame word.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0xD7 = CTCWife, bit 23 of the dword at
// +0x38), CTCWife::GetAppearanceChangedNoticed @ 0x006CED50, CCharString copy ctor @ 0x0099EC30 from the category
// string table @ 0x013BAB40, CAIStateGroupBase::GetWorld @ 0x008FCFC0 (result dropped), CCharString::operator+= @
// 0x0099F0A0 with the second table @ 0x013BABD0, SayToThingReactingTo @ 0x0095EE70, CTCWife::
// ClearAppearanceChangedNoticed @ 0x006CECD0, ~CCharString @ 0x0099EAE0.
// Shape: the category out value shares the lookup key's frame word; a NAMED CCharString line destroyed on both returns.
// Engine header names for the state group; the header flattens the base, so the class is modelled locally.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
// NF56 (round 7, queue q10451): the class comes from engine/CAIStateGroup_OpinionReactionRoot.h (overlay opt-in
//   FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE: the retail class on its CAIStateGroupBase base), so the row defines the
//   PDB-private ?WifeClothingUpdate@CAIStateGroup_OpinionReactionRoot@@AAE_NW4EOpinionAttitudeType@@@Z (Ego_r.pdb 0x009A73A0 / FableWin.pdb
//   0x02C7E6B0) and calls the private ?SayToThingReactingTo@..@@AAE?AW4EAIActionReturn@@ABVCCharString@@@Z (both PDBs) and the protected base
//   ?GetWorld@CAIStateGroupBase@@IBEAAVCWorld@@XZ through engine/CAIStateGroupBase.h (PBrain +0x04 from the same header). Were local
//   CAIStateGroupBase / CAIStateGroup_OpinionReactionRoot models (public members, a long-returning SayToThingReactingTo). The local
//   CAIBrain / intelligent-pointer / CTCWife models are pre-existing.
#include "fable_compiler.h"
class CTCBase;
class CThing;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x004365B0.
// NF52 (round 7): the suffix table at 0x013BABD0 is the private static COpinionReactionManager::AttitudeStrings
// (?AttitudeStrings@COpinionReactionManager@@0QBVCCharString@@B; both PDBs' dynamic initializer / atexit destructor), indexed by the
// EOpinionAttitudeType and read through FableWin.pdb's public inline
// ?GetAttitudeString@COpinionReactionManager@@QBEABVCCharString@@W4EOpinionAttitudeType@@@Z on the world's manager (FableWin
// ?GetOpinionReactionManager@CWorld@@QAEAAVCOpinionReactionManager@@XZ, inline): that is why the GetWorld() call survives with its result
// unused. CWorld / COpinionReactionManager / EOpinionAttitudeType come from their shared headers (engine/CWorld.h,
// engine/COpinionReactionManager.h FABLE_RETAIL_GREETING_PASS23). Were local CWorld / EOpinionAttitudeType models and the made-up extern
// g_WifeAppearanceLineSuffix. The prefix table 0x013BAB40 and the local CAIStateGroupBase / CAIBrain / intelligent-pointer / CTCWife
// models are pre-existing (outside this entry).
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GOTOBED_WORLD
#define FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_WIFEUPDATE
#define FABLE_RETAIL_GREETING_PASS23
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/EOpinion.h"
#include "engine/EOpinionAttitudeType.h"
#include "engine/CWorld.h"
class CTCOpinionSource;
#include "engine/COpinionReactionManager.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};


struct CBaseIntelligentPointerCtor {            // CBaseIntelligentPointer (8 bytes: vptr + item)
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointerCtor();              // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointerCtor();             // 0x00A01C10
};

// `PThing = thing` is the converting-temporary + inline `SetPItem(other.GetPItem())` assignment.
struct CIntelligentPointerThing : CBaseIntelligentPointerCtor {
    FABLE_FORCEINLINE CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointerCtor() { SetPItem(item); }
    FABLE_FORCEINLINE CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

extern CCharString g_WifeAppearanceLinePrefix[];    // 0x013BAB40 (indexed by EWifeAppearanceCategories)

class CTCWife { public:
    bool GetAppearanceChangedNoticed(long* category);   // 0x006CED50
    void ClearAppearanceChangedNoticed(const long* category);   // 0x006CECD0
};


#include "engine/CAIStateGroup_OpinionReactionRoot.h"

bool CAIStateGroup_OpinionReactionRoot::WifeClothingUpdate(EOpinionAttitudeType attitude)
{
    CTCWife* wife;
    if (PBrain->PCreature->GetTC(TCI_WIFE, &wife)) {
        long category;
        if (wife->GetAppearanceChangedNoticed(&category)) {
            CCharString line(g_WifeAppearanceLinePrefix[category]);
            line += GetWorld().GetOpinionReactionManager().GetAttitudeString(attitude);
            if (SayToThingReactingTo(line) == AI_ACTION_FINISHED) {
                wife->ClearAppearanceChangedNoticed(&category);
                return true;
            }
            return false;
        }
    }
    return false;
}