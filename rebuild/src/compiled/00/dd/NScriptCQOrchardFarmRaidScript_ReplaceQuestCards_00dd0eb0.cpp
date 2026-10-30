// CQ_OrchardFarmRaidScript::ReplaceQuestCards() -- give the hero both Orchard Farm raid quest cards (protect the farm, good and evil)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD0EB0, length 161 bytes (PDB: ?ReplaceQuestCards@CQ_OrchardFarmRaidScript@NScript@@QAEXXZ);
// called from 0x00DCFA60 and 0x00DD1640.
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, and AddQuestCard (slot 292, +0x490) through
// the quest's game script interface (+0x40).
// Shape: both string arguments are implicit conversion temporaries, built right to left before the two `false` pushes
// and destroyed in reverse order at the end of each statement.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
void operator delete(void* p);                           // 0x00BFE9BC (MSVCR71)

#include "engine/CCountedPointer.h"


#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0

// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// Retail expands these at this row's sites (the local model had them in class): defined inline after the header.
inline CScriptThing::~CScriptThing() {}
inline bool CScriptThing::IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"

struct CQuestScriptBase : CScriptBase {                  // retail 0x48 bytes
    CGameScriptInterface* GSI;                           // +0x40
    void*                 Owner;                         // +0x44
};

namespace NScript {
class CQ_OrchardFarmRaidScript : public CQuestScriptBase {
public:
    void ReplaceQuestCards();
};
} // namespace NScript
using NScript::CQ_OrchardFarmRaidScript;

void NScript::CQ_OrchardFarmRaidScript::ReplaceQuestCards()
{
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM", "Q_OrchardFarmRaidGood", false, false);
    GSI->AddQuestCard("OBJECT_QUEST_CARD_PROTECT_FARM_EVIL", "Q_OrchardFarmRaidEvil", false, false);
}