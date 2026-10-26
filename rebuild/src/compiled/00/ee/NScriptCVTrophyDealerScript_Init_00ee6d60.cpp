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
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                // 0x0099EAE0
};
struct CGameScriptInterface {                      // retail vtable @ 0x01260F0C
    GSI_V100(0) GSI_V100(1)
    GSI_V10(20) GSI_V10(21) GSI_V10(22) GSI_V10(23) GSI_V10(24) GSI_V10(25) GSI_V10(26) GSI_V10(27) GSI_V10(28)
    virtual void v290(); virtual void v291(); virtual void v292(); virtual void v293(); virtual void v294(); virtual void v295();
    virtual void SetQuestCardObjective(const CCharString& quest, const CCharString& text, const CCharString& region, const CCharString& extra) const;   // slot 296 (+0x4a0)
    virtual void v297(); virtual void v298(); virtual void v299();
    GSI_V100(3) GSI_V100(4) GSI_V100(5)
    GSI_V10(60) GSI_V10(61) GSI_V10(62) GSI_V10(63) GSI_V10(64)
    virtual void v650(); virtual void v651(); virtual void v652(); virtual void v653(); virtual void v654();
    virtual CCharString GetActiveQuestName() const;   // slot 655 (+0xa3c)
};
extern const char g_trophyDealerObjective[];       // 0x012F5208 "TEXT_QUEST_FIND_TROPHY_DEALER_OBJECTIVE_..."
struct CScriptBase {
    virtual void v00();
    unsigned char _body[0x3c];
};
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