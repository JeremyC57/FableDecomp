// CBordelloEntrance::Main() -- Bowerstone bordello door: lock the door (Me); until Q_TraderEscort is completed, tell the hero it is closed whenever he uses it (game info, wait for click-past); then set the "BordelloSign" readable text (refuge once it became a nunnery, otherwise open), unlock the door and idle
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) @ 0x00E3B4A0, 671 bytes (672 with padding); entity-script vtable slot 1 (Main).
// Callees: CEntityScriptBase::IsActiveThreadTerminating 0x00F35B30, CCharString(const char*, long) 0x0099EBF0 /
//   ~CCharString 0x0099EAE0, ~CScriptThing 0x004AA840; CScriptThing slot 20 MsgIsUsedBy; CGameScriptInterface slots
//   7 NewScriptFrame, 40 MsgIsGameInfoClickedPast, 72 GetThingWithScriptName, 115 DisplayGameInfo, 286 IsQuestCompleted,
//   518 EntitySetAsLocked, 714 SetReadableObjectTextTag.
//   Literals "Q_TraderEscort" 0x012C560C, "SCRIPT_NAME_HERO" 0x0125D1C8, "TEXT_QST_B13_BORDELLO_CLOSED" 0x012E3BE8,
//   "TEXT_QST_B13_BORDELLO_OPEN" 0x012E3BCC, "BordelloSign" 0x012E3BBC, "TEXT_QST_B13_BORDELLO_REFUGE" 0x012E3B9C.
// Layout: Ego_r.pdb CBordelloEntrance (28 bytes): CEntityScriptBase {vptr, Game +4, Me +8}, ParentClass +0x14
//   (CV_BordelloScript, retail = PDB - 0x14: BecomeNunnery +0x4d), PMasterData +0x18.
// Shape: no alive filter; the sign's text tag is set from the nested by-value lookup (Game's vtable cached in ebx).
class C3DVector { public: float X, Y, Z; };
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
class CScriptThing { public:                                  // vtable 0x01238C8C, 12 bytes
    virtual void t000(); virtual void t001(); virtual void t002(); virtual void t003(); virtual void t004(); virtual void t005(); virtual void t006(); virtual void t007();
    virtual void t008(); virtual void t009(); virtual void t010(); virtual void t011(); virtual void t012(); virtual void t013(); virtual void t014(); virtual void t015();
    virtual void t016(); virtual void t017(); virtual void t018(); virtual void t019();
    virtual bool MsgIsUsedBy(const CCharString& name) const;   // slot 20 (+0x50)
    void* PImp_Data;                                   // +0x04
    void* PImp_Info;                                   // +0x08
    CScriptThing(const CScriptThing& other);           // 0x004ABE90
    virtual ~CScriptThing();                                   // 0x004AA840
};
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
template <class T> class CCountedPointer { public:
    void* Data;
    void* Info;
    CCountedPointer() : Data(0), Info(0) { }
};
#include "engine/CScriptGameResourceObjectBase.h"
#include "engine/CScriptGameResourceObjectScriptedThingBase.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CEntityScriptBase { public:                             // retail 0x14 bytes
    virtual void e00();
    CGameScriptInterface* Game;                        // +0x04
    CScriptThing          Me;                          // +0x08
    void SetPredicate(void* predicate);       // 0x00F35B10
    bool IsActiveThreadTerminating() const;            // 0x00F35B30
};
struct CV_BordelloScript {
    unsigned char _pad[0x4d];
    bool          BecomeNunnery;                       // +0x4d
};
struct CBordelloEntrance : CEntityScriptBase {
    CV_BordelloScript* ParentClass;                    // +0x14
    void*              PMasterData;                    // +0x18 CQ_SunnyvaleMasterData*
    void Main();
};

void CBordelloEntrance::Main()
{
    Game->EntitySetAsLocked(Me, true);
    while (!Game->IsQuestCompleted("Q_TraderEscort")) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_B13_BORDELLO_CLOSED");
            while (!Game->MsgIsGameInfoClickedPast()) {
                Game->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            Game->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
    }
    if (IsActiveThreadTerminating())
        return;
    if (!ParentClass->BecomeNunnery) {
        if (IsActiveThreadTerminating())
            return;
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_OPEN");
        Game->EntitySetAsLocked(Me, false);
    } else {
        if (IsActiveThreadTerminating())
            return;
        Game->SetReadableObjectTextTag(Game->GetThingWithScriptName("BordelloSign"), "TEXT_QST_B13_BORDELLO_REFUGE");
        Game->EntitySetAsLocked(Me, false);
    }
    do {
        Game->NewScriptFrame();
    } while (!IsActiveThreadTerminating());
}