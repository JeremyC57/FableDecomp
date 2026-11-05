// NScript::CV_BordelloScript::CBordelloEntrance::Main() -- Bowerstone bordello door: lock the door (Me); until Q_TraderEscort is completed, tell the hero it is closed whenever he uses it (game info, wait for click-past); then set the "BordelloSign" readable text (refuge once it became a nunnery, otherwise open), unlock the door and idle
// Leaf: ?Main@CBordelloEntrance@CV_BordelloScript@NScript@@EAEXXZ
// Plan phase 5 (2026-11-05), CScriptThing wave: the local CScriptThing stand-in replaced by the shared engine/CScriptThing.h under FABLE_RETAIL_NSCRIPT (RTTI CScriptThing : CBaseClass, vtable 0x01238C8C, PImp +0x04; ~CScriptThing 0x004AA840 and the copy 0x004ABE90 declared out of line). Also: the engine/CCountedPointer.h include moved up to the header include (the header needs it first).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
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
// Naming wave 35: identity reg 00e39de9 "BordelloEntrance" in 00e39b40; Alloc 00e3b3d0; vtable 012e3b80 slot 1; PDB ?Main@CBordelloEntrance@CV_BordelloScript@NScript@@; refs=1; Anniversary RTTI agrees (confirmed); was under-qualified CBordelloEntrance::Main (wave3/labels_alloc.tsv); PDB ?Main@CBordelloEntrance@CV_BordelloScript@NScript@@EAEXXZ in Ego_r.pdb / FableWin.pdb.
//   Was `?Main@CBordelloEntrance@@QAEXXZ`; this source now defines the PDB member (outer stand-in CV_BordelloScript moved into NScript); same bytes.
#include "engine/C3DVector.h"
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
#define FABLE_RETAIL_NSCRIPT
#include "engine/CCountedPointer.h"
#include "engine/CScriptThing.h"
enum EScriptEntityMoveType { SCRIPT_ENTITY_MOVE_TYPE_WALK };
enum EScriptAIPriority { SCRIPT_AI_PRIORITY_NONE };
enum ETextGroupSelectionMethod { TEXT_GROUP_SELECTION_RANDOM };
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)
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
namespace NScript {
class CV_BordelloScript {
public:
    unsigned char _pad[0x4d];
    bool          BecomeNunnery;                       // +0x4d
public:
    class CBordelloEntrance;         // the entity class below (naming wave 35)
};
}
using NScript::CV_BordelloScript;
namespace NScript {
class CV_BordelloScript::CBordelloEntrance : public CEntityScriptBase {
public:
    CV_BordelloScript* ParentClass;                    // +0x14
    void*              PMasterData;                    // +0x18 CQ_SunnyvaleMasterData*
private:
    virtual void Main();                          // PDB ?Main@CBordelloEntrance@CV_BordelloScript@NScript@@EAEXXZ
};
}

void NScript::CV_BordelloScript::CBordelloEntrance::Main()
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