// NScript::CQ_AmbushTradersScript::AskBanditHireQuestion(const CScriptThing&, CScriptGameResourceObjectScriptedThingBase&) -- ask the hero whether to hire the bandit; on "yes" with enough gold pay him and return true, otherwise make the bandit say why not (if he still has health) and return false
// Leaf: ?AskBanditHireQuestion@CQ_AmbushTradersScript@NScript@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F07ED0, length 782 bytes (padded to 784), ret 8.
// Callees: IsActiveThreadTerminating 0x00CB7940; CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0;
// CScriptThing vtable 0x01238C8C slot 75 IsAlive, out-of-line ~CScriptThing 0x004AA840 (first GetScriptThing temporary
// only), ~CBaseClass 0x0099A2E0, operator delete 0x00BFE9BC; CScriptGameResourceObjectScriptedThingBase vtable 0x0127094C
// slots 12 GetScriptThing, 13 Speak, 26 IsPerformingScriptTask; CGameScriptInterface slots 7, 39, 70, 114, 126, 127, 264, 520.
// Layout: Game +0x40 (Ego_r.pdb - 0x14); pGlobalScriptDef (0x0143E90C) AmbushTradersBanditHireCost is PDB 0xa7c, retail +0x9dc.
// Shape: the "cash poor" health test destroys its CScriptThing temporary out of line (inline_depth(0) region) while the
// "no hire" one inlines the destructor (__forceinline); the question's last literal reuses the bandit argument's slot.
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
void operator delete(void* p);                         // 0x00BFE9BC
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    __forceinline ~CCountedPointer()
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
class CScriptThing : public CBaseClass { public:                     // vtable 0x01238C8C, 12 bytes
    virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007(); virtual void t008(); virtual void t009(); 
    virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015(); virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019(); 
    virtual void t020(); virtual void t021(); virtual void t022(); virtual void t023(); virtual void t024(); virtual void t025(); virtual void t026(); virtual void t027(); virtual void t028(); virtual void t029(); 
    virtual void t030(); virtual void t031(); virtual void t032(); virtual void t033(); virtual void t034(); virtual void t035(); virtual void t036(); virtual void t037(); virtual void t038(); virtual void t039(); 
    virtual void t040(); virtual void t041(); virtual void t042(); virtual void t043(); virtual void t044(); virtual void t045(); virtual void t046(); virtual void t047(); virtual void t048(); virtual void t049(); 
    virtual void t050(); virtual void t051(); virtual void t052(); virtual void t053(); virtual void t054(); virtual void t055(); virtual void t056(); virtual void t057(); virtual void t058(); virtual void t059(); 
    virtual void t060(); virtual void t061(); virtual void t062(); virtual void t063(); virtual void t064(); virtual void t065(); virtual void t066(); virtual void t067(); virtual void t068(); virtual void t069(); 
    virtual void t070(); virtual void t071(); virtual void t072(); virtual void t073(); virtual void t074(); 
    virtual bool IsAlive() const;                      // slot 75 (+0x12c) 0x004AB130
    CCountedPointer<CScriptThing> PImp;
    __forceinline ~CScriptThing() { }                  // out-of-line copy 0x004AA840
};
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_2 = 2 };
class CScriptGameResourceObjectScriptedThingBase { public:    // vtable 0x0127094C
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011();
    virtual CScriptThing GetScriptThing() const;       // slot 12 (+0x30) 0x007E7490
    virtual void Speak(const CScriptThing& listener, const char* textGroup, ETextGroupSelectionMethod method, bool a, bool b, bool c);   // slot 13 (+0x34) 0x007E7390
    virtual void v014(); virtual void v015(); virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021();
    virtual void v022(); virtual void v023(); virtual void v024(); virtual void v025();
    virtual bool IsPerformingScriptTask() const;       // slot 26 (+0x68) 0x007E7450
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

class CScriptDef { public:
    unsigned char _pad[0x9dc];
    long AmbushTradersBanditHireCost;                  // +0x9dc
};
extern CScriptDef* pGlobalScriptDef;                       // 0x0143E90C (Ego_r.pdb `pGlobalScriptDef`, a def pointer: its first word)
// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
namespace NScript {
class CQ_AmbushTradersScript : public CScriptBase { public:
    CGameScriptInterface* Game;                        // +0x40
    void*                 Data;                        // +0x44
    bool AskBanditHireQuestion(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject);
};

bool CQ_AmbushTradersScript::AskBanditHireQuestion(const CScriptThing& bandit, CScriptGameResourceObjectScriptedThingBase& banditObject)
{
    if (bandit.IsAlive()) {
        if (IsActiveThreadTerminating()) return false;
        Game->GiveHeroYesNoQuestion("TEXT_QST_003_QUEST_ON_QUESTION", "TEXT_OBJECT_HERO_ANSWER_YES", "TEXT_OBJECT_HERO_ANSWER_NO", "", true);
        long answer = Game->MsgIsQuestionAnsweredYesOrNo();
        while (answer < 0) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return false;
            answer = Game->MsgIsQuestionAnsweredYesOrNo();
        }
        if (IsActiveThreadTerminating()) return false;
        if (answer == 1) {
            if (IsActiveThreadTerminating()) return false;
            CScriptDef* def = pGlobalScriptDef;
            if (Game->GetHeroGold() >= -def->AmbushTradersBanditHireCost) {
                if (IsActiveThreadTerminating()) return false;
                Game->GiveHeroGold(pGlobalScriptDef->AmbushTradersBanditHireCost);
                Game->EntityGiveGold(bandit, -pGlobalScriptDef->AmbushTradersBanditHireCost);
                return true;
            }
            if (IsActiveThreadTerminating()) return false;
#pragma inline_depth(0)
            bool alive = Game->GetHealth(banditObject.GetScriptThing()) > 0.0f;
#pragma inline_depth()
            if (alive) {
                banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_CASH_POOR", TEXT_GROUP_SELECTION_2, false, true, false);
                while (banditObject.IsPerformingScriptTask()) {
                    Game->NewScriptFrame();
                    if (IsActiveThreadTerminating()) return false;
                }
                if (IsActiveThreadTerminating()) return false;
            }
            return false;
        }
    }
    bool alive = Game->GetHealth(banditObject.GetScriptThing()) > 0.0f;
    if (alive) {
        banditObject.Speak(Game->GetHero(), "TEXT_QST_003_BANDIT_NO_HIRE", TEXT_GROUP_SELECTION_2, false, true, false);
        while (banditObject.IsPerformingScriptTask()) {
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating()) return false;
        }
        if (IsActiveThreadTerminating()) return false;
    }
    return false;
}
}