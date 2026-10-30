// CV_TrophyDealerScript::Init() -- set the active quest's card objective to "find the trophy dealer" in Witchwood
// Leaf: ?Init@CV_TrophyDealerScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00EE6D60, length 142 bytes (padded to 144).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x3), CGameScriptInterface::GetActiveQuestName
// (slot 655, +0xa3c, CCharString by value) and SetQuestCardObjective(const CCharString&, x4) (slot 296, +0x4a0) through
// Game at +0x40, CCharString::~CCharString @ 0x0099EAE0 (x4).
// Strings: "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_..." @ 0x012F5208, "Witchwood2" @ 0x012DCEF8, "" @ 0x0122D70E.
// Shape: three named string locals declared last-argument-first (retail builds them before reading Game); the quest-name
// temporary is destroyed first, then the locals in reverse order.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
#define GSI_V10(p) virtual void v##p##0(); virtual void v##p##1(); virtual void v##p##2(); virtual void v##p##3(); virtual void v##p##4(); virtual void v##p##5(); virtual void v##p##6(); virtual void v##p##7(); virtual void v##p##8(); virtual void v##p##9();
#define GSI_V100(p) GSI_V10(p##0) GSI_V10(p##1) GSI_V10(p##2) GSI_V10(p##3) GSI_V10(p##4) GSI_V10(p##5) GSI_V10(p##6) GSI_V10(p##7) GSI_V10(p##8) GSI_V10(p##9)
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
extern const char g_trophyDealerObjective[];       // 0x012F5208 "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_..."
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CQuestScriptObject : CScriptBase {
    CGameScriptInterface* Game;                    // +0x40
    void*                 Data;                    // +0x44
};
namespace NScript {
class CV_TrophyDealerScript : public CQuestScriptObject {
public:
    void Init();
};
} // namespace NScript
using NScript::CV_TrophyDealerScript;
void NScript::CV_TrophyDealerScript::Init()
{
    CCharString extra("");
    CCharString region("Witchwood2");
    CCharString text(g_trophyDealerObjective);
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), text, region, extra);
}