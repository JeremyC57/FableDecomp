// CV_FisticuffsClubScript::AddQuestInfo() -- shows the quest info panel and adds the hit-count bar (AllowedHits, green on red, "HUD_HERO_ICON"), keeping its id in HitID
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E8AF70, 162 bytes (padded to 176), plain `ret`.
// Callees: CGameScriptInterface slot 321 DisplayQuestInfo(bool) (+0x504, 0x00891920) and slot 324 AddQuestInfoBar(float,
//   float, const CRGBColour&, const CRGBColour&, const CCharString&, const CCharString&, float) (+0x510, 0x00891AC0),
//   CCharString::CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0. Strings: "" 0x0122D70E,
//   "HUD_HERO_ICON" 0x012D6824; 1.0f pushed as an immediate.
// Shape: the two literals are implicit conversion temporaries (built before every other argument, right to left);
//   the colours are CRGBColour(r, g, b, a) temporaries whose body stores R, G, B, A into the B,G,R,A layout
//   (+2/+1/+0/+3, VC groups the 0xff and 0 byte stores); the 0.0f max shares the zero register of the byte stores.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, HitID +0x7c, AllowedHits +0x90.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CRGBColour { public:                                // B, G, R, A bytes
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
namespace NScript {
class CV_FisticuffsClubScript {
public:
    unsigned char _base[0x40];                     // CScriptBase
    CGameScriptInterface* Game;                    // +0x40
    void* Data;                                    // +0x44
    unsigned char _pad48[0x7c - 0x48];
    long HitID;                                    // +0x7c
    long HitCount;                                 // +0x80
    long AttackWarnings;                           // +0x84
    long FightCount;                               // +0x88
    bool EverybodyAttack, LostWhileOutside, FisticuffsActive;   // +0x8c
    long AllowedHits;                              // +0x90
    void AddQuestInfo();
};
} // namespace NScript
using NScript::CV_FisticuffsClubScript;
void NScript::CV_FisticuffsClubScript::AddQuestInfo()
{
    Game->DisplayQuestInfo(true);
    HitID = Game->AddQuestInfoBar((float)AllowedHits, 0.0f, CRGBColour(0, 255, 0, 255), CRGBColour(255, 0, 0, 255),
                                  "HUD_HERO_ICON", "", 1.0f);
}