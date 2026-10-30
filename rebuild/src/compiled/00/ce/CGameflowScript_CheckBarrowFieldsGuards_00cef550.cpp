// CGameflowScript::CheckBarrowFieldsGuards() -- unless a Trader Conflict quest is already done, wait for one to start, require the evil one, wait for it to complete, wait for Barrow Fields to unload and reload, then enable the Barrowfields village guards
// Leaf: CheckBarrowFieldsGuards
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CEF550, length 912 bytes (+ int3 padding; plain `ret`).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString(const char*, long) 0x0099EBF0 / ~CCharString
//   0x0099EAE0, ~CScriptThing 0x004AA840; CGameScriptInterface (at +0x40) slots 7 NewScriptFrame (+0x1c),
//   12 IsRegionLoaded (+0x30), 25 MsgOnQuestCompleted (+0x64), 72 GetThingWithScriptName (+0x120, CScriptThing by
//   hidden pointer), 284 IsQuestActive (+0x470), 286 IsQuestCompleted (+0x478), 443 EnableGuards (+0x6ec)
//   (names from ghidra_out/cgsi_slots_aligned.tsv).
// Data: "Q_TraderConflictEvil" 0x012C52DC, "Q_TraderConflictGood" 0x012C529C, "BarrowFields" 0x012C5D94,
//   "VILLAGE_BARROWFIELDS" 0x012C5D7C.
// Layout: CScriptBase is 0x40 bytes, the game script interface pointer at +0x40 (as the landed quest scripts, e.g.
//   CQ_HangingTreeEvilScript::MonitorEnemies 0x00D6BE20). Interface modelled locally: engine/CGameScriptInterface.h is
//   data-only.
// Shape: the two `!A && !B` conditions carry conditionally constructed CCharString temporaries, so VC keeps
//   construction flags in ebx (1/2, 4/8) and materialises the condition in a frame byte; each wait loop is
//   `while (...) { NewScriptFrame(); if (IsActiveThreadTerminating()) return; }` preceded by its own terminate check;
//   the first region wait compares IsRegionLoaded `!= false` (`test al,al ; setne bl`; a plain condition gives `mov bl,al`, `== true` gives dec/neg/sbb), the negated waits use `!` (neg/sbb/inc).
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class CScriptThing { public:
    void* PImp_Data;
    void* PImp_Info;
    virtual ~CScriptThing();                                        // 0x004AA840
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

// CScriptBase: the retail NScript model from the shared headers (FABLE_RETAIL_NSCRIPT: CScriptBase :
//   CBaseClassNonCopyable : CBaseClass, vtable 0x012C1648, 5 slots, 0x40).
#include "engine/CScriptBase.h"
struct CGameflowScript : CScriptBase {
    CGameScriptInterface* GSI;                              // +0x40
    void CheckBarrowFieldsGuards();
};

void CGameflowScript::CheckBarrowFieldsGuards()
{
    if (!GSI->IsQuestCompleted("Q_TraderConflictEvil") && !GSI->IsQuestCompleted("Q_TraderConflictGood")) {
        if (IsActiveThreadTerminating())
            return;
        while (!GSI->IsQuestActive("Q_TraderConflictEvil") && !GSI->IsQuestActive("Q_TraderConflictGood")) {
            GSI->NewScriptFrame();
            if (IsActiveThreadTerminating())
                return;
        }
        if (IsActiveThreadTerminating())
            return;
        if (GSI->IsQuestActive("Q_TraderConflictEvil")) {
            if (IsActiveThreadTerminating())
                return;
            while (!GSI->MsgOnQuestCompleted("Q_TraderConflictEvil")) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (GSI->IsRegionLoaded("BarrowFields") != false) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            while (!GSI->IsRegionLoaded("BarrowFields")) {
                GSI->NewScriptFrame();
                if (IsActiveThreadTerminating())
                    return;
            }
            if (IsActiveThreadTerminating())
                return;
            GSI->EnableGuards(GSI->GetThingWithScriptName("VILLAGE_BARROWFIELDS"), true);
        }
    }
}