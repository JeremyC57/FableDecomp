// CGameflowAssistanceScript::Tutorial_RenownLevelIncrease() -- while assistance runs, whenever the player-controlled hero's renown level rises have the guildmaster announce levels one to seven and, from level two on, retire the placeholder quest cards
// Leaf: Tutorial_RenownLevelIncrease
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CF02A0, length 672 bytes (+ int3 padding; plain `ret`; no EH frame).
// Callees: CScriptBase::IsActiveThreadTerminating 0x00CB7940, CCharString() 0x0099E4B0 / CCharString(const char*, long)
//   0x0099EBF0 / ~CCharString 0x0099EAE0, the vector destructor iterator 0x00401400 (catalog "vector_constructor_iterator":
//   same signature (array, size 4, count 8, ~CCharString)); CGameScriptInterface (at +0x40) slots 7 NewScriptFrame (+0x1c),
//   162 GetHeroRenownLevel (+0x288), 274 HeroReceiveMessageFromGuildMaster (+0x448), 293 RemoveQuestCardFromGuild (+0x494),
//   394 IsHeroControlledByPlayer (+0x628) (names from ghidra_out/cgsi_slots_aligned.tsv).
// Data: "TEXT_QST_080_RENOWN_LEVEL_ONE" 0x012C5FD0 .. "_SEVEN" 0x012C5F10, "" 0x0122D70E, "DUMMY_QUEST_HAS_NO_SCRIPT" 0x012C59DC.
// Layout: retail = Ego_r.pdb CGameflowAssistanceScript - 0x14: interface +0x40, master data +0x44 (stop flag +0x67),
//   SavedRenownLevel +0x60.
// Shape: a local `CCharString messages[8]` aggregate-initialised with the seven level texts (the eighth default-built).
//   Retail destroys the array with an inline countdown loop at four exits and through the vector destructor iterator at
//   the three terminate exits inside the renown block: those three `return`s sit in `#pragma inline_depth(0)` regions.
//   The entry terminate test guards the loop (`if (!IsActiveThreadTerminating()) for (;;)`) and the loop bottom is
//   `if (!IsActiveThreadTerminating()) continue; break;`, so both share the closing-brace exit as in retail.
class CCharString { public:
    void* PStringData;
    CCharString();                                          // 0x0099E4B0
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

struct CGameflowMasterData {
    unsigned char _pad[0x67];
    bool          StopAssistance;                           // +0x67
};
class CScriptBase { public:
    unsigned char _body[0x40];
    bool IsActiveThreadTerminating();                       // 0x00CB7940
};
struct CGameflowAssistanceScript : CScriptBase {
    CGameScriptInterface* GSI;                              // +0x40
    CGameflowMasterData*  PMasterData;                      // +0x44
    long                  FlourishesOffered;                // +0x48
    long                  FlourishesPerformed;              // +0x4c
    long                  CombatMultHighestLevel;           // +0x50
    long                  CombatMultFramesSinceChange;      // +0x54
    long                  HealthWarningsGiven;              // +0x58
    long                  WillEnergyWarningsGiven;          // +0x5c
    long                  SavedRenownLevel;                 // +0x60
    long                  SaveWarningsGiven;                // +0x64
    void Tutorial_RenownLevelIncrease();
};

void CGameflowAssistanceScript::Tutorial_RenownLevelIncrease()
{
    CCharString messages[8] = {
        "TEXT_QST_080_RENOWN_LEVEL_ONE",
        "TEXT_QST_080_RENOWN_LEVEL_TWO",
        "TEXT_QST_080_RENOWN_LEVEL_THREE",
        "TEXT_QST_080_RENOWN_LEVEL_FOUR",
        "TEXT_QST_080_RENOWN_LEVEL_FIVE",
        "TEXT_QST_080_RENOWN_LEVEL_SIX",
        "TEXT_QST_080_RENOWN_LEVEL_SEVEN",
    };
    if (!IsActiveThreadTerminating())
    for (;;) {
        if (PMasterData->StopAssistance) {
            if (IsActiveThreadTerminating())
                return;
            return;
        }
        long renown = GSI->GetHeroRenownLevel();
        if (GSI->IsHeroControlledByPlayer()) {
            if (IsActiveThreadTerminating())
                return;
            if (renown > SavedRenownLevel) {
#pragma inline_depth(0)
                if (IsActiveThreadTerminating())
                    return;
#pragma inline_depth()
                SavedRenownLevel = renown;
                if (renown < 8) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    GSI->HeroReceiveMessageFromGuildMaster(messages[renown - 1], "", true, true);
                }
                if (renown > 1) {
#pragma inline_depth(0)
                    if (IsActiveThreadTerminating())
                        return;
#pragma inline_depth()
                    GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                    GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                    GSI->RemoveQuestCardFromGuild("DUMMY_QUEST_HAS_NO_SCRIPT");
                }
            }
        }
        GSI->NewScriptFrame();
        if (!IsActiveThreadTerminating())
            continue;
        break;
    }
}