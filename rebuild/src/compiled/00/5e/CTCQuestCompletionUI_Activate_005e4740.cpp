// CTCQuestCompletionUI::Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport) -- open the quest-completion screen for a script: (type 3 first closes the boast UI) store the script name, failure text and teleport flag, activate against the script's active quest card, and on a type-0 completion drop the script's accepted boasts
// Leaf: ?Activate@CTCQuestCompletionUI@@QAEXW4CompletionType@1@ABVCCharString@@_NABVCWideString@@2@Z
// agentNF20 (2026-11-11): CompletionType is the class's nested enum (Ego_r.pdb / FableWin.pdb W4CompletionType@1@ in both Activate
//   overloads: this row and the callee 0x005E4430 ?Activate@CTCQuestCompletionUI@@QAEXW4CompletionType@1@PAVCTCQuestCard@@_N@Z).
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8).
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Naming wave 18: the global stand-in g_pQuestManager (0x013B89FC) is the private static CCountedPointer<CQuestManager>
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass, read through the inline static accessor Get() (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E4740, 277 bytes, `ret 14h`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interfaces 0xE8 = CTCBoastUI and 0x6C = CTCQuestCard), CTCBoastUI::Deactivate
//   @ 0x0056A880, CCharString::operator= @ 0x0099EFB0, CWideString::operator= @ 0x0099B7D0, Activate(CompletionType,
//   CTCQuestCard*, bool) @ 0x005E4430, CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80,
//   CCharString copy ctor @ 0x0099EC30, CQuestManager::RemoveAcceptedBoastsForScript(CCharString) @ 0x004AFFA0.
//   CTBaseSingletonNoDefaultAlloc<CQuestManager>::PClass @ 0x013B89FC.
// Layout: Ego_r.pdb CTCQuestCompletionUI with retail containers: ScriptName +0x50 / FailureReason +0x54 (PDB -8),
//   CanTeleportToGuildMember +0xD4 (PDB 0xE8, -0x14 after the three 12-byte maps). CThing interface bitset +0x20, map +0x44.
// Shape: the out-parameter GetInterface for the boast UI (the key in the frame word; HasTC returns the raw masked word, a
//   `!= 0` bool gives `shr ecx,8`); the card pointer is a function-scope zero (edi) declared after the boast block, so it is
//   also the zero of the type-3 call and type stays in its argument slot; it is filled under HasTC (key in the dead
//   canTeleport slot) and the two Activate calls tail-merge; the quest manager is read into a named local before the
//   ScriptName copy is built in the argument slot.
class CTCBase;
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CWIDESTRING
#include "engine/CThing.h"

#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBoastUI { public:
    void Deactivate();                                  // 0x0056A880
};
class CTCQuestCard;
#include "engine/CCountedPointer.h"
template <class T> class CTBaseSingletonNoDefaultAlloc {              // the CRTP singleton base of CQuestManager
    static CCountedPointer<T> PClass;                                // 0x013B89FC ?PClass@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@0V?$CCountedPointer@VCQuestManager@@@@A (private static)
public:
    static T& __fastcall Get() { return *PClass; }                   // Ego_r / FableWin ?Get@?$CTBaseSingletonNoDefaultAlloc@VCQuestManager@@@@SIAAVCQuestManager@@XZ (inline)
};
class CQuestManager { public:
    void RemoveAcceptedBoastsForScript(CCharString name);               // 0x004AFFA0
public:
    CThing* GetActiveQuestCardFromScriptName(const CCharString&) const;   // 0x004B0C80
};
class CTCQuestCompletionUI { public:
    enum CompletionType { COMPLETION_0, COMPLETION_1, COMPLETION_2, COMPLETION_3 };   // enumerator names descriptive
    virtual void v00();
    CThing*       ParentThing;                          // +0x04 (CTCBase)
    unsigned char _pad08[0x50 - 0x08];
    CCharString   ScriptName;                           // +0x50
    CWideString   FailureReason;                        // +0x54
    unsigned char _pad58[0xd4 - 0x58];
    bool          CanTeleportToGuildMember;             // +0xd4
    void Activate(CompletionType type, CTCQuestCard* card, bool arg);   // 0x005E4430
    void Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport);
};

void CTCQuestCompletionUI::Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport)
{
    if (type == COMPLETION_3) {
        CTCBoastUI* boast;
        if (ParentThing->GetTC(TCI_BOAST_UI, &boast))
            boast->Deactivate();
    }
    CTCQuestCard* card = 0;
    ScriptName = scriptName;
    FailureReason = failureReason;
    CanTeleportToGuildMember = canTeleport;
    if (type == COMPLETION_3) {
        Activate(COMPLETION_3, card, arg);
        return;
    }
    CThing* cardThing = CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get().GetActiveQuestCardFromScriptName(scriptName);
    if (cardThing) {
        if (cardThing->HasTC(TCI_QUEST_CARD))
            card = ((CTCQuestCard*)&cardThing->GetTC(TCI_QUEST_CARD));
        Activate(type, card, arg);
    } else {
        Activate(type, 0, arg);
    }
    if (type == COMPLETION_0) {
        CQuestManager* questManager = (&CTBaseSingletonNoDefaultAlloc<CQuestManager>::Get());
        questManager->RemoveAcceptedBoastsForScript(ScriptName);
    }
}