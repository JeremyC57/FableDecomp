// CTCQuestCompletionUI::Activate(CompletionType type, const CCharString& scriptName, bool arg, const CWideString& failureReason, bool canTeleport) -- open the quest-completion screen for a script: (type 3 first closes the boast UI) store the script name, failure text and teleport flag, activate against the script's active quest card, and on a type-0 completion drop the script's accepted boasts
// Leaf: ?Activate@CTCQuestCompletionUI@@QAEXW4CompletionType@@ABVCCharString@@_NABVCWideString@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005E4740, 277 bytes, `ret 14h`.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interfaces 0xE8 = CTCBoastUI and 0x6C = CTCQuestCard), CTCBoastUI::Deactivate
//   @ 0x0056A880, CCharString::operator= @ 0x0099EFB0, CWideString::operator= @ 0x0099B7D0, Activate(CompletionType,
//   CTCQuestCard*, bool) @ 0x005E4430, CQuestManager::GetActiveQuestCardFromScriptName(const CCharString&) @ 0x004B0C80,
//   CCharString copy ctor @ 0x0099EC30, CQuestManager::RemoveAcceptedBoastsForScript(CCharString) @ 0x004AFFA0.
//   g_pQuestManager @ 0x013B89FC.
// Layout: Ego_r.pdb CTCQuestCompletionUI with retail containers: ScriptName +0x50 / FailureReason +0x54 (PDB -8),
//   CanTeleportToGuildMember +0xD4 (PDB 0xE8, -0x14 after the three 12-byte maps). CThing interface bitset +0x20, map +0x44.
// Shape: the out-parameter GetInterface for the boast UI (the key in the frame word; HasTC returns the raw masked word, a
//   `!= 0` bool gives `shr ecx,8`); the card pointer is a function-scope zero (edi) declared after the boast block, so it is
//   also the zero of the type-3 call and type stays in its argument slot; it is filled under HasTC (key in the dead
//   canTeleport slot) and the two Activate calls tail-merge; the quest manager is read into a named local before the
//   ScriptName copy is built in the argument slot.
class CTCBase;
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);              // 0x0099EC30
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};
struct CWideString {
    void* m_pData;
    CWideString& operator=(const CWideString& other);   // 0x0099B7D0
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    __forceinline unsigned long HasTC(long type) { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    template <class T> __forceinline T* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
    template <class T> __forceinline bool GetInterface(long type, T** out)
    {
        if (HasTC(type)) {
            *out = GetTC<T>(type);
            return true;
        }
        return false;
    }
};
struct CTCBoastUI {
    void Deactivate();                                  // 0x0056A880
};
struct CTCQuestCard;
struct CQuestManager {
    CThing* GetActiveQuestCardFromScriptName(const CCharString& name);   // 0x004B0C80
    void RemoveAcceptedBoastsForScript(CCharString name);               // 0x004AFFA0
};
extern CQuestManager* g_pQuestManager;                  // 0x013B89FC
enum CompletionType { COMPLETION_0, COMPLETION_1, COMPLETION_2, COMPLETION_3 };
struct CTCQuestCompletionUI {
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
        if (ParentThing->GetInterface(0xe8, &boast))
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
    CThing* cardThing = g_pQuestManager->GetActiveQuestCardFromScriptName(scriptName);
    if (cardThing) {
        if (cardThing->HasTC(0x6c))
            card = cardThing->GetTC<CTCQuestCard>(0x6c);
        Activate(type, card, arg);
    } else {
        Activate(type, 0, arg);
    }
    if (type == COMPLETION_0) {
        CQuestManager* questManager = g_pQuestManager;
        questManager->RemoveAcceptedBoastsForScript(ScriptName);
    }
}
