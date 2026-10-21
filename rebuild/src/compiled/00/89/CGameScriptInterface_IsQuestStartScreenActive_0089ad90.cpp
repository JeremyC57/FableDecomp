// CGameScriptInterface::IsQuestStartScreenActive(CCharString&) const -- is the hero's quest-completion UI up as the start screen of this quest
// Leaf: ?IsQuestStartScreenActive@CGameScriptInterface@@UBE_NAAVCCharString@@@Z
// Retail: Fable.exe @ 0x0089AD90, 155 bytes, `ret 4`; Ego_r.pdb same symbol (CGameScriptInterface slot 36).
// Body: hero = g_pMainGameComponent (0x013B86A0)->PPlayerManager (+0x1c)->GetMainPlayer() (0x00449970)
// ->GetPPlayerCharacter() (0x00487DC0), not null-tested; if it holds interface 0xfc (bit 28 of UsingComponents[7], +0x3c):
// ui = the component (non-const LowerBound 0x0040F020); true when ui->Active and !ui->Quitting and ui->Completion == 0
// and ui->ScriptName == questName (inline CCharString ==, Compare 0x00411570).
// Layout: interface 0xfc = CTCQuestCompletionUI by its retail offsets: Ego_r.pdb Active +0xc, Quitting +0x20,
// Completion +0x24, ScriptName +0x58; retail Quitting +0x1c / Completion +0x20 (STLport vector Components is 12 bytes,
// not 16) and ScriptName +0x50 (the Boasts std::list is one node pointer, -8; LastRenderTime double realigns to +0x48).
// Completion 0 is the first CTCQuestCompletionUI::CompletionType value (name not recovered). CThing as 0x00899E90.
// Shape: /O2 (16-aligned entry); HasTC then the unchecked pointer-form lookup; CCharString::operator== must be
// __forceinline (plain inline is called out of line here).
struct CCharStringData {
    const char*   Chars;                                 // +0x00
    unsigned long Length;                                // +0x04
};
long __fastcall Compare(const char* a, const char* b);   // 0x00411570
class CCharString {
public:
    CCharStringData* PStringData;
    __forceinline bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData && PStringData->Length == other.PStringData->Length)
            return Compare(PStringData->Chars, other.PStringData->Chars) == 0;
        return false;
    }
};
struct CTCBase;
class CInterfaceMap {                                    // CThing::Components, CVectorMap<ETCInterfaceType, CTCBase*> (retail 8 bytes)
public:
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                // 0x0040F020
};
class CThing {
public:
    unsigned char _pad00[0x20];
    unsigned long UsingComponents[9];                    // +0x20 bitset<274>
    CInterfaceMap Components;                            // +0x44
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    template <class T> __forceinline T* GetTC()
    {
        long type = T::INTERFACE_TYPE;
        CInterfaceMap* map = &Components;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || T::INTERFACE_TYPE < it->type) ? map->last : it)->component;
    }
};
class CTCQuestCompletionUI {
public:
    enum { INTERFACE_TYPE = 0xfc };
    unsigned char _pad00[0x0c];
    bool          Active;                                // +0x0c
    bool          ReallyActive;                          // +0x0d
    unsigned char _pad0e[0x1c - 0x0e];
    bool          Quitting;                              // +0x1c
    unsigned char _pad1d[3];
    long          Completion;                            // +0x20 CompletionType
    unsigned char _pad24[0x50 - 0x24];
    CCharString   ScriptName;                            // +0x50
};
class CThingPlayerCreature : public CThing { };
class CPlayer {
public:
    CThingPlayerCreature* GetPPlayerCharacter() const;   // 0x00487DC0
};
class CPlayerManager {
public:
    CPlayer& GetMainPlayer();                            // 0x00449970
};
class CMainGameComponent {
public:
    unsigned char   _pad00[0x1c];
    CPlayerManager* PPlayerManager;                      // +0x1c
    CPlayerManager* GetPlayerManager() const { return PPlayerManager; }
};
extern CMainGameComponent* g_pMainGameComponent;         // 0x013B86A0
class CGameScriptInterface {
public:
    virtual bool IsQuestStartScreenActive(CCharString& questName) const;
};

bool CGameScriptInterface::IsQuestStartScreenActive(CCharString& questName) const
{
    CThingPlayerCreature* hero = g_pMainGameComponent->GetPlayerManager()->GetMainPlayer().GetPPlayerCharacter();
    if (hero->HasTC(CTCQuestCompletionUI::INTERFACE_TYPE))
    {
        CTCQuestCompletionUI* ui = hero->GetTC<CTCQuestCompletionUI>();
        if (ui->Active && !ui->Quitting && ui->Completion == 0 && ui->ScriptName == questName)
            return true;
    }
    return false;
}