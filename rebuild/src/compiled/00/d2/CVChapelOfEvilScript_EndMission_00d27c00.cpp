// CV_ChapelOfEvilScript::EndMission() -- quest end: wait for the end flag, wait until the hero guild's inside region is loaded, mark the guild's main map unusable and stop the hero guide showing quest cards
// Leaf: ?EndMission@CV_ChapelOfEvilScript@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00D27C00, 288 bytes, plain `ret`.
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0, ~CCharString
//   0x0099EAE0, ~CScriptThing 0x004AA840 (out of line); through the interface (+0x40) NewScriptFrame (slot 7),
//   IsRegionLoaded (slot 12, +0x30), GetThingWithScriptName (slot 72, +0x120, CScriptThing by hidden pointer),
//   SetThingAsUsable (slot 447, +0x6fc), SetHeroGuideToShowQuestCardsWhenSpokenTo (slot 544, +0x880).
// Strings: "HeroGuildComplexInside" 0x01238FCC, "MainGuildMap" 0x01291BBC.
// Shape: two script wait loops each followed by the `if (IsActiveThreadTerminating()) return;` macro; the region test's
//   bool survives the string temporary's destructor in bl (neg/sbb/inc), ebx/edi pushes are sunk to their first use;
//   the nested `Game->SetThingAsUsable(Game->GetThingWithScriptName("..."), false)` loads the outer vtable into edi
//   and pushes the bool before the inner call (right-to-left), then destroys the thing and the string.
// Identity: the label is doubtful -- Ego_r.pdb CV_ChapelOfEvilScript has no bool at retail +0x51 (PDB +0x65 sits in
//   the long DonationCounter), and the body (guild region, guild map, hero guide) is a guild-quest ending; the
//   neighbours at 0x00D26A50.. are CQ_DragonBossFightScript. The end flag at +0x51 is named descriptively.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:                                       // vtable 0x01238C8C, 12 bytes
    void* PImp_Data;
    void* PImp_Info;
    virtual ~CScriptThing();                                        // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                        // retail 0x40 bytes
    virtual void v00();
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* Game;                             // +0x40
    void*                 Data;                             // +0x44
};
struct CV_ChapelOfEvilScript : CQuestScriptBase {
    unsigned char _pad48[9];                                // +0x48
    bool          MissionEnding;                            // +0x51
    void EndMission();
};
void CV_ChapelOfEvilScript::EndMission()
{
    while (!MissionEnding) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    while (!Game->IsRegionLoaded("HeroGuildComplexInside")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    Game->SetThingAsUsable(Game->GetThingWithScriptName("MainGuildMap"), false);
    Game->SetHeroGuideToShowQuestCardsWhenSpokenTo(false);
}