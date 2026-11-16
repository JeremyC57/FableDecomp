// CMessageEvent::GetPOriginator<CThingAICreature>() const -- the message's originator as a CThingAICreature when its type flags carry
// every CThingAICreature flag (CThingAICreature::StatGetTypeFlags() = 0x1f), else null
// Leaf: ??$GetPOriginator@VCThingAICreature@@@CMessageEvent@@QBEPAVCThingAICreature@@XZ
// Retail: Fable.exe (TLC, VC7.1) COMDAT @ 0x0090DF30, 48 bytes: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 on POriginator (+0x48)
// three times (the last a tail jump), the flag test `not eax ; test al,1Fh` on GetTypeFlags (+0x6c).
// Identity (NF42, round 5): FableWin.pdb ??$GetPOriginator@VCThingAICreature@@@CMessageEvent@@QBEPAVCThingAICreature@@XZ; the body is
// engine/CMessageEvent.h's FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL template compiled out of line: the landed caller
// CAIStateGroup_MinionGossip::Update 0x0090D030 expands it at two sites and calls this COMDAT at the third (0x0090D1AD) and binds
// this symbol. Was the placeholder `CThingHolder_0090df30::GetPThing` on local CThing / CIntelligentPointer models; now an explicit
// instantiation over the shared headers, with that row's opt-in set.
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGCREATUREBASE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGCREATUREBASE_CREATURETYPE
#define FABLE_RETAIL_CTHINGCREATUREBASE_GETSEX
#define FABLE_RETAIL_CTHINGCREATUREBASE_ANIMQUERY
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_STATTYPEFLAGS
#define FABLE_RETAIL_CTHINGAICREATURE_HOMEBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CONVERT_INLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_OUTOFLINE
#define FABLE_RETAIL_CMESSAGEEVENT
#define FABLE_RETAIL_CMESSAGEEVENT_ORIGINATOR_ORIGINAL
#include "fable_string.h"
#include "engine/CThingAICreature.h"
#include "engine/CMessageEvent.h"

template CThingAICreature* CMessageEvent::GetPOriginator<CThingAICreature>() const;
