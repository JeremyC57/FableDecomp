// CV_HeroDuelScript::ShowEnemyHealth() -- once the duel starts show Thunder's health bar and track his health each frame until he is beaten or the mission ends
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E99470, 0x23c bytes, plain `ret`.
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   IsActiveThreadTerminating 0x00CB7940, ~CScriptThing 0x004AA840, __ftol2 0x00BFEA70; the thing's IsAlive (slot 75,
//   inline null test); CGameScriptInterface slots 7 NewScriptFrame, 72 GetThingWithScriptName, 264 GetHealth,
//   321 DisplayQuestInfo, 325 AddQuestInfoBarHealth.
// Data: five function-local statics sharing the guard word 0x0143E95C (bits 1..0x10): three CRGBColours 0x0143E958 /
//   0x0143E954 / 0x0143E950 and two C2DVectors 0x0143E948 / 0x0143E940 -- initialised here and never read (script
//   translator constants); the bar colour actually passed is a CRGBColour(255, 255, 0, 255) temporary.
// Shape: WAIT_UNTIL(DuelStarted) then a `while (!ThunderDefeated)` loop that yields twice per pass; the default-constructed
//   CCharString local is unused but lives to the end.
// Layout (retail = Ego_r.pdb - 0x14): Game +0x40, MissionSucceeded +0x48, MissionFailed +0x49, DuelStarted +0x4c,
//   ThunderDefeated +0x4d, MyHealth +0x58.
// NScript (sweep 3): the script classes live in namespace NScript (Ego_r/FableWin name sets, e.g.
//   ?IsHeroDressedAsLady@CV_BordelloScript@NScript@@QAE_NXZ; every CV_/CQ_ script class is spelled ...@NScript@@ and
//   class-keyed); this row's class is now declared there (wrapped, struct->class). Same bytes.
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString();                                       // 0x0099E4B0
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CRGBColour { public:                                      // B,G,R,A bytes (D3DCOLOR order)
    unsigned char B, G, R, A;
    CRGBColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
};
class C2DVector { public:
    float X, Y;
    C2DVector(float x, float y) { X = x; Y = y; }
};
void operator delete(void* p);                          // 0x00BFE9BC (MSVCR71)
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
namespace NScript {
class CV_HeroDuelScript : public CScriptBase {
public:
    CGameScriptInterface* Game;                          // +0x40
    void* PMasterData;                                   // +0x44
    bool MissionSucceeded;                               // +0x48
    bool MissionFailed;                                  // +0x49
    bool MissionOver;                                    // +0x4a
    bool GotToExecutionTree;                             // +0x4b
    bool DuelStarted;                                    // +0x4c
    bool ThunderDefeated;                                // +0x4d
    unsigned char _pad4e[0x58 - 0x4e];
    unsigned long MyHealth;                              // +0x58
    void ShowEnemyHealth();
};
} // namespace NScript
using NScript::CV_HeroDuelScript;

void NScript::CV_HeroDuelScript::ShowEnemyHealth()
{
    while (!DuelStarted) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    CScriptThing thunder = Game->GetThingWithScriptName("Thunder");
    static const CRGBColour healthHigh(0, 128, 0, 255);
    static const CRGBColour healthMedium(128, 64, 0, 255);
    static const CRGBColour healthLow(128, 0, 0, 255);
    static const C2DVector barPosition(370.0f, 50.0f);
    static const C2DVector barSpacing(0.0f, 25.0f);
    CCharString unused;
    Game->AddQuestInfoBarHealth(thunder, CRGBColour(255, 255, 0, 255), "HUD_QUEST_ICON_THUNDER", 1.0f);
    Game->DisplayQuestInfo(true);
    while (!ThunderDefeated) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (thunder.IsAlive()) {
            if (IsActiveThreadTerminating())
                return;
            MyHealth = (unsigned long)Game->GetHealth(thunder);
        }
        if (MissionSucceeded || MissionFailed) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayQuestInfo(false);
            return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
}