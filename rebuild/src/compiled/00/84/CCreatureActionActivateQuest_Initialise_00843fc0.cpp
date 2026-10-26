// CCreatureAction_ActivateQuest::Initialise() -- record the object used on the quest manager (and, when there is one, its definition's display string), activate the named quest with the LoadResources flag, mark the action initialised
// Leaf: Initialise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00843FC0, 207 bytes (208 with padding), virtual `?Initialise@CCreatureAction_ActivateQuest@@UAEXXZ`.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable
//   0x01238C6C), CThing::GetDefName 0x004C7CC0 (CDefString by hidden pointer), CDefStringTable::GetString 0x009D49B0 on the
//   global table 0x013CA828, CCharString::operator= 0x0099EFB0, ~CCharString 0x0099EAE0, CQuestManager::ActivateQuest
//   0x004B4A10 (manifest "CQuestManager_ActivateQuest"); the quest manager singleton 0x013B89FC.
// Layout: retail CCreatureActionBase is 0xa8 bytes (landed FrameUpdate 0x00844090): ScriptName +0xa8, LoadResources +0xac,
//   PObjectToUse +0xad (unaligned: engine/CCreatureAction_ActivateQuest.h is pack(1), PDB +0xb4/+0xb8/+0xb9); Initialised
//   +0x63; CQuestManager LastObjectUsed pointer +0xa8, LastObjectUsedName +0xb0.
// Shape: the manager is read once into a named local for the pointer assignment (the landed "assign through a stack
//   CIntelligentPointer" shape), then re-read through the global for the later uses; the used object is a named local
//   fetched before GetDefName's hidden result pointer is pushed; the display string is a named local. ActivateQuest takes
//   its real PDB signature (const CCharString&, bool, bool) -> bool: the bool LoadResources argument is what gives the
//   `xor edx,edx ; mov dl,[edi+0ACh]` widening (an unsigned long parameter gives movzx).
class CDefString { public:
    long m_index;
    CDefString& operator=(const CDefString& other);   // user assignment: returned through the hidden pointer, copied as a dword
};

class CCharString { public:
    void* PStringData;
    ~CCharString();                             // 0x0099EAE0
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
};

struct CDefStringTable {
    CCharString GetString(CDefString name);     // 0x009D49B0
};
extern CDefStringTable g_DefStringTable;        // 0x013CA828

class CThing { public:
    CDefString GetDefName();                    // 0x004C7CC0
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +4
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};

class CQuestManager { public:
    unsigned char _pad00[0xa8];
    CIntelligentPointer LastObjectUsed;         // +0xa8
    CCharString LastObjectUsedName;             // +0xb0
    bool ActivateQuest(const CCharString& name, bool a2, bool loadResources);   // 0x004B4A10
};
extern CQuestManager* g_pQuestManager;          // 0x013B89FC

#pragma pack(push, 1)                           // the real (PDB) layout: PObjectToUse is unaligned
class CCreatureAction_ActivateQuest { public:
    virtual void v00();
    unsigned char _pad04[0x63 - 4];
    bool Initialised;                           // +0x63
    unsigned char _pad64[0xa8 - 0x64];
    CCharString ScriptName;                     // +0xa8
    bool LoadResources;                         // +0xac
    CIntelligentPointer PObjectToUse;           // +0xad
    void Initialise();
};
#pragma pack(pop)

void CCreatureAction_ActivateQuest::Initialise()
{
    CQuestManager* questManager = g_pQuestManager;
    questManager->LastObjectUsed = CIntelligentPointer(PObjectToUse.GetPItem());
    if (PObjectToUse.GetPItem()) {
        CThing* object = PObjectToUse.GetPItem();
        CCharString name = g_DefStringTable.GetString(object->GetDefName());
        g_pQuestManager->LastObjectUsedName = name;
    }
    g_pQuestManager->ActivateQuest(ScriptName, false, LoadResources);
    Initialised = true;
}