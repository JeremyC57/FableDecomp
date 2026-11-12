// CTCCreatureGroupBoastingCrowd::SetHero(CThing& hero) -- PHero = CIntelligentPointer<CThing>(&hero), through a stack temporary
// Leaf: ?SetHero@CTCCreatureGroupBoastingCrowd@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007841F0, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?SetHero@CTCCreatureGroupBoastingCrowd@@QAEXAAVCThing@@@Z; the landed caller 0x00783A30 binds this
//   exact spelling (rebuild/oracles/reloc_bindings.tsv); engine/CTCCreatureGroupBoastingCrowd.h declares SetHero and PHero
//   (CIntelligentPointer<CThing>, +0x24). Was the stand-in CClass_007841f0::AssignTarget_007841f0.
// Body: the (T*) constructor of a temporary CIntelligentPointer<CThing> (0x00A01B10, vptr 0x01238C6C, SetPItem 0x00A01B90), the inline
//   operator= (GetPItem 0x00A01B50, SetPItem on PHero), ~CBaseIntelligentPointer 0x00A01C10.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CRESOURCE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBOASTINGCROWDCONTROL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CQUESTMANAGER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CThing.h"
#include "engine/CTCCreatureGroupBoastingCrowd.h"

void CTCCreatureGroupBoastingCrowd::SetHero(CThing& hero)
{
    PHero = CIntelligentPointer<CThing>(&hero);
}
