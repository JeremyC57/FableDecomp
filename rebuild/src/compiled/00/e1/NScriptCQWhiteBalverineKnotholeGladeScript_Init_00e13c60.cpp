// CQ_WhiteBalverineKnotholeGladeScript::Init() -- register the quest's three regions and its two boasts, then reset the quest state
// Leaf: ?Init@CQ_WhiteBalverineKnotholeGladeScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00E13C60, length 439 bytes (padded to 448).
// Callees: CCharString(const char*, long) @ 0x0099EBF0, ~CCharString @ 0x0099EAE0; through the script's interface
//   (+0x40): AddQuestRegion (slot 271, +0x43c) and AddBoast (slot 303, +0x4bc).
// Data: the script def @ 0x0143E90C; its WhiteBalv boast block sits 4 bytes below Ego_r.pdb's CScriptDef offsets
//   (WhiteBalvNakedBoastCost PDB +0x214 -> retail +0x210).
// Layout (retail = Ego_r PDB -0x14): MissionFailed +0x48 .. GreenWife +0x4c, BalverineState +0x50,
//   QuestStartScreenShown +0x60.
// Shape: every string literal is passed straight to a `const CCharString&` parameter (the conversion temporaries are
//   built right to left before any push); the def's cost/reward are read inline in the argument list.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* Data;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptDef { public:
    unsigned char _pad[0x210];
    long WhiteBalvNakedBoastCost;                       // +0x210
    long WhiteBalvNakedBoastReward;                     // +0x214
    long WhiteBalvNoDamageBoastCost;                    // +0x218
    long WhiteBalvNoDamageBoastReward;                  // +0x21C
};
extern CScriptDef* pGlobalScriptDef;                        // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
class CScriptBase { public:                                    // retail 0x40 bytes
    unsigned char _body[0x40];
};
namespace NScript {
class CQ_WhiteBalverineKnotholeGladeScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                         // +0x40
    void* Data;                                         // +0x44
    bool MissionFailed;                                 // +0x48
    bool MissionSucceeded;                              // +0x49
    bool WifeCutsceneStart;                             // +0x4a
    bool WifeCutsceneStop;                              // +0x4b
    bool GreenWife;                                     // +0x4c
    long BalverineState;                                // +0x50
    unsigned char KG_Chief[0xc];                        // +0x54 CScriptThing
    bool QuestStartScreenShown;                         // +0x60
    void Init();
};
} // namespace NScript
using NScript::CQ_WhiteBalverineKnotholeGladeScript;

void NScript::CQ_WhiteBalverineKnotholeGladeScript::Init()
{
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "KnotholeGlade");
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "Witchwood4");
    Game->AddQuestRegion("Q_WhiteBalverineKnotholeGlade", "DemonDoor_KnotholeGlade");
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NAKED", 1, pGlobalScriptDef->WhiteBalvNakedBoastCost,
                   pGlobalScriptDef->WhiteBalvNakedBoastReward, false, "", 0);
    Game->AddBoast("TEXT_QST_BOAST_DESCRIPTION_NODAMAGE", 3, pGlobalScriptDef->WhiteBalvNoDamageBoastCost,
                   pGlobalScriptDef->WhiteBalvNoDamageBoastReward, false, "", 0);
    BalverineState = 0;
    MissionSucceeded = false;
    MissionFailed = false;
    WifeCutsceneStart = false;
    WifeCutsceneStop = false;
    GreenWife = false;
    QuestStartScreenShown = false;
}