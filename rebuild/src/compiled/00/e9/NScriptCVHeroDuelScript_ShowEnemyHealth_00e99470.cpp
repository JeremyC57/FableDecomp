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
class CCPPointerInfo { public:                                 // CCountedPointer's shared block
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);          // destroys the pointee
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0
class CScriptThing : public CBaseClass { public:                      // 12 bytes, vtable 0x01238C8C; PImp is the game-side thing
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008();
    virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016();
    virtual void t017(); virtual void t018(); virtual void t019(); virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024();
    virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); virtual void t030(); virtual void t031(); virtual void t032();
    virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); virtual void t040();
    virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048();
    virtual void t049(); virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056();
    virtual void t057(); virtual void t058(); virtual void t059(); virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064();
    virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); virtual void t070(); virtual void t071(); virtual void t072();
    virtual void t073(); virtual void t074();
    CCountedPointer<CScriptThing> PImp;                 // +0x04 Data, +0x08 Info
    ~CScriptThing() {}                                  // out of line 0x004AA840
    virtual bool IsAlive() const { return PImp.Data && PImp.Data->IsAlive(); }   // slot 75 (+0x12c), out of line 0x004AB130
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CScriptBase { public:                                     // retail 0x40 bytes
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                    // 0x00CB7940
};
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