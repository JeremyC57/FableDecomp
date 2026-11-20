// CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours() const -- true when the creature's village says it is work time
// Leaf: ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0091A6F0, length 83 bytes (+ int3 padding).
// Identity (two sources): Ego_r.pdb 0x0093E1A0 + FableWin.pdb 0x02BA9910 ?IsInWorkHours@?$CAIStateGroup_WorkBase@VCAIStateGroupBase@@@@IBE_NXZ
//   (protected const member of the class template); every landed row that calls 0x0091A6F0 binds this spelling (0x0091A390, 0x0091B170,
//   0x00943C20, 0x00944730, 0x00944E80, 0x00946F30, 0x00947280, 0x009473E0). Was ?IsInWorkHours@CAIStateGroup_WorkBase@@QAE_NXZ.
// Callees: CVectorMap::LowerBound 0x0040F020 (the creature's interface lookup, TCI_VILLAGE_MEMBER = bit 3 of the dword at +0x24),
//   CTCVillageMember::GetPVillage 0x007EE540, CTCVillage::IsWorkTime 0x00658B20.
// NF131 (2026-11-20, q4946): the row-local CThing / CTCVillage / CTCVillageMember / CAIBrain / CAIStateGroupBase / CAIStateGroup_WorkBase
//   models give way to the shared headers. The body is the class template's header body (engine/CAIStateGroup_WorkBase.h
//   FABLE_RETAIL_CAISTATEGROUP_WORKBASE_ISINWORKHOURS, the one 0x00945060 / 0x00944940 expand), emitted here by an explicit
//   instantiation: a pick-any COMDAT like every includer's copy, so the class template keeps one definition (GOTCHAS nf80).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_WORKBASE_ISINWORKHOURS
#define FABLE_RETAIL_CTCVILLAGE
#define FABLE_RETAIL_CTCVILLAGE_ISWORKTIME
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCVillageMember.h"
#include "engine/CTCVillage.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_WorkBase.h"

template bool CAIStateGroup_WorkBase<CAIStateGroupBase>::IsInWorkHours() const;
