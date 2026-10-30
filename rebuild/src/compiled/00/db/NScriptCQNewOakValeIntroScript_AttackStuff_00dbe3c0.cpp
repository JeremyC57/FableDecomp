// CQ_NewOakValeIntroScript::AttackStuff() -- switch Oakvale to its post-attack state: activate the post-attack quest, deactivate the pre-attack one, set night, change the environment theme and set the active quest's card objective
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DBE3C0, length 282 bytes, plain `ret`.
// Callees (CGameScriptInterface through +0x40, retail vtable 0x01260F0C): ActivateQuest (slot 276, +0x450,
// 0x00892E80), DeactivateQuest(const CCharString&, unsigned long) (slot 280, +0x460, 0x00892F00), SetTimeOfDay(float)
// (slot 646, +0xa18, 0x0088FDC0), TransitionToTheme(const CCharString&, float) (slot 656, +0xa40, 0x0088FEE0),
// GetActiveQuestName() (slot 655, +0xa3c, CCharString by value, 0x00891880), SetQuestCardObjective(4 x const
// CCharString&) (slot 296, +0x4a0, 0x00896A30); CCharString::CCharString(const char*, long) @ 0x0099EBF0 (x6),
// CCharString::~CCharString @ 0x0099EAE0 (x7).
// Literals: 0x012D826C, 0x012D9CB4, 0x012D9D7C, "" 0x0122D70E (x2), 0x012D9D54; 23.0f and 0.0f as immediates.
// Shape: the first three strings are implicit conversion temporaries sharing one frame word; the objective
// block is the landed CV_TrophyDealerScript::Init form (0x00EE6D60) in its own {} scope (so the text local shares
// the temporaries' frame word): three named string locals declared
// last-argument-first, the vtable cached in edi across the GetActiveQuestName call.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString {
public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

namespace NScript {
class CQ_NewOakValeIntroScript {
public:
    unsigned char          _base[0x40];              // CScriptBase
    CGameScriptInterface*  m_pInterface;             // +0x40
    void AttackStuff();
};
} // namespace NScript
using NScript::CQ_NewOakValeIntroScript;

void NScript::CQ_NewOakValeIntroScript::AttackStuff()
{
    m_pInterface->ActivateQuest("Q__OakValeIntro_PostAttack");
    m_pInterface->DeactivateQuest("Q_NewOakValeIntro_PreAttack", 0);
    m_pInterface->SetTimeOfDay(23.0f);
    m_pInterface->TransitionToTheme("ENVIRONMENT_OV_POSTATTACK", 0.0f);
    {
        CCharString extra("");
        CCharString region("");
        CCharString text("TEXT_QUEST_OAKVALE_INTRO_OBJECTIVE_06");
        m_pInterface->SetQuestCardObjective(m_pInterface->GetActiveQuestName(), text, region, extra);
    }
}