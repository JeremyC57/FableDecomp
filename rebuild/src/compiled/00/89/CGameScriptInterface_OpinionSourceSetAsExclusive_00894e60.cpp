// CGameScriptInterface::OpinionSourceSetAsExclusive(const CScriptThing& thing, bool exclusive) const -- when the script thing holds a thing (slot 11, no validity check) with the opinion-source interface (type 0x50: bitset word 2 bit 16): make it the opinion reaction manager's exclusive source (CWorld+0x74, 0x00700770) when asked, else clear the exclusive source when it is this thing (the intelligent pointer at +0x714, GetPItem @ 0x00A01B50)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00894E60, length 89 bytes.
// Callees: the script thing's virtual slot 11 (+0x2c) GetThing (no validity check), the inline HasInterface bitset test, CBaseIntelligentPointer::GetPItem @ 0x00A01B50, the manager's exclusive-source setter @ 0x00700770 (label propagated). Shape: the interface in edi, the thing in esi, the manager re-read through World in every arm, `ret 8` tail-duplicated.
// Leaf: ?OpinionSourceSetAsExclusive@CGameScriptInterface@@UBEXABVCScriptThing@@_N@Z
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Port (NF177, q5090): moved off the row-local CThing_Retail / CWorld / COpinionReactionManager_Retail models onto engine/CThing.h,
// CWorld.h, COpinionReactionManager.h and CScriptThing.h (the 0x008909F0 recipe). Callee: Ego_r.pdb 0x00550510 + FableWin.pdb
// ?SetThingOpinionSource@COpinionReactionManager@@QAEXPAVCThing@@@Z (0x00700770; the header declares it); the exclusive-source read is
// the inline ?PeekThingOpinionSource@COpinionReactionManager@@QBEPBVCThing@@XZ (GetPItem 0x00A01B50). Interface 0x50 = TCI_OPINION_SOURCE.
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_COPINIONREACTIONMANAGER_PEEKSOURCE
#include "fable_compiler.h"
#include "engine/CThing.h"
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
#include "engine/CWorld.h"
#include "engine/COpinionReactionManager.h"
#include "engine/CGameScriptInterface.h"

void CGameScriptInterface::OpinionSourceSetAsExclusive(const CScriptThing& thing, bool exclusive) const
{
    CThing* t = thing.GetPThing();
    if (t && t->HasTC(TCI_OPINION_SOURCE)) {
        if (exclusive)
            World->GetOpinionReactionManager().SetThingOpinionSource(t);
        else if (World->GetOpinionReactionManager().PeekThingOpinionSource() == t)
            World->GetOpinionReactionManager().SetThingOpinionSource(0);
    }
}
