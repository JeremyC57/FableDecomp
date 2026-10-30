// CTortureDoorPassage::Main() -- keep the torture-door passage locked until the mother is rescued, showing the "need to have mother" hint whenever the hero uses it; then unlock it
// Leaf: Main
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00DD8990, length 290 bytes (+ int3 padding). Entity-script slot 1 (Main).
// Callees: CGameScriptInterface slots 518 EntitySetAsLocked (+0x818), 7 NewScriptFrame (+0x1c), 115 DisplayGameInfo
// (+0x1cc), 40 MsgIsGameInfoClickedPast (+0xa0) (ghidra_out/cgsi_slots_aligned.tsv); CScriptThing slot 20 MsgIsUsedBy
// (+0x50, 0x004AAB10); the entity script's IsActiveThreadTerminating @ 0x00F35B30; CCharString(const char*, long)
// @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0.
// Strings: 0x0125D1C8 "SCRIPT_NAME_HERO", 0x012DC484 "TEXT_QST_055_HINT_NEED_TO_HAVE_MOTHER".
// Layout: Ego_r.pdb CTortureDoorPassage (= retail): Game +0x04, Me +0x08 (the entity-script base, as the landed
// CQ_SecretPassageScript::CTortureDoorCourtyard::Main 0x00DEEFC0), ParentClass +0x14 (CQ_PrisonEscapeRescueMotherScript,
// its mother-rescued flag at +0x67), PMasterData +0x18.
// Shape: both strings are implicit conversion temporaries (the use test's result kept in bl across its destructor); the
// outer loop tests the parent flag at the bottom, every wait checks IsActiveThreadTerminating and returns.
namespace NScript { class CPredicateBase; }
class CCharString { public:                                     // 4 bytes: the shared string block pointer
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"
class CQ_PrisonEscapeRescueMotherScript {
public:
    unsigned char _pad00[0x67];
    bool          MotherRescued;                         // +0x67
};
class CQ_SunnyvaleMasterData;
class CEntityScriptBase
{
public:
    bool IsActiveThreadTerminating() const;   // 0x00F35B30
};

class CTortureDoorPassage : public CEntityScriptBase {
public:
    void*                              __vftable;        // +0x00
    CGameScriptInterface*              Game;             // +0x04
    CScriptThing                       Me;               // +0x08
    CQ_PrisonEscapeRescueMotherScript* ParentClass;      // +0x14
    CQ_SunnyvaleMasterData*            PMasterData;      // +0x18
    void Main();
};

void CTortureDoorPassage::Main()
{
    Game->EntitySetAsLocked(Me, true);
    while (!ParentClass->MotherRescued) {
        Game->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (Me.MsgIsUsedBy("SCRIPT_NAME_HERO")) {
            if (IsActiveThreadTerminating())
                return;
            Game->DisplayGameInfo("TEXT_QST_055_HINT_NEED_TO_HAVE_MOTHER");
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
    Game->EntitySetAsLocked(Me, false);
}