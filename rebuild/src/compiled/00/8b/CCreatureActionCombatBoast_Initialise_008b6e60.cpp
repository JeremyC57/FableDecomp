// CCreatureAction_CombatBoast::Initialise() -- the combat dialogue manager says the dialogue def's boast speech for the creature, the action plays "SPECIAL_BOAST", then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B6E60, length 114 bytes, plain `ret`.
// Callees: CCombatActionBase::GetCombatManager @ 0x00856B10 (PCreature.GetPItem()->GetCombatManager(), out of line;
//          manifest label "_Dest_val<...>" is propagated), CBaseIntelligentPointer::GetPItem @ 0x00A01B50,
//          CombatDialogueManager::SaySpeech(CThingCreatureBase&, ulong) @ 0x008AFC40, CCharString::CCharString
//          @ 0x0099EBF0, CCombatActionBase::SetAnimationName @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
//          CCombatActionBase::Initialise @ 0x00857520.
// Shape: the dialogue def pointer is copied into a block-scoped CDefPointer from a named CCombatManager* local (copying
//   straight from `GetCombatManager()->...` leaves a dead `add eax,44h`: the copy ctor parameter address), null-tested
//   `inc [p+4]` add-ref, the
//   speech id (+0x5c) read into ebx, and the copy released by its destructor (count decremented, slot 1 on zero);
//   the combat manager for SaySpeech is a named local fetched before the pushes; the animation string is a scoped
//   named local; the base Initialise runs last.
// Layout: retail CCombatManager holds DialogueManager at +0x38 (PDB +0x40: the two leading maps are 12 bytes in
//   retail) and CombatDialogueManager::PDialogueDef at its +0x0c (after the 12-byte map); CCombatActionBase is 0x114
//   bytes with PCreature at +8. CCombatDialogueDef +0x5c is the boast speech id (name descriptive).
class CThingCreatureBase;

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CDefBase {                           // counted game definition: vtable + reference count
    virtual void v00();
    virtual void Destroy();                 // slot 1
    long m_refCount;                        // +4
    void AddRef() const { ++((CDefBase*)this)->m_refCount; }
    void Release() const { if (--((CDefBase*)this)->m_refCount == 0) ((CDefBase*)this)->Destroy(); }
};

class CCombatDialogueDef : public CDefBase { public:
    unsigned char _pad08[0x5c - 0x08];
    unsigned long BoastSpeech;              // +0x5c
};

template <class T>
class CDefPointer { public:
    T* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CDefPointer() { m_pDef->Release(); }
    T* operator->() const { return m_pDef; }
};

class CombatDialogueManager { public:
    unsigned char SpeechGroupInfoMap[0x0c];                  // +0x00 map<ulong, TSpeechGroupInfo>
    CDefPointer<const CCombatDialogueDef> PDialogueDef;      // +0x0c
    bool SaySpeech(CThingCreatureBase& creature, unsigned long speech);   // 0x008AFC40
};

class CCombatManager { public:
    unsigned char _pad00[0x38];
    CombatDialogueManager DialogueManager;  // +0x38
};

struct CBaseIntelligentPointer {            // CIntelligentPointer<T>: vptr + item
    void* __vftable;                        // +0x00
    CThingCreatureBase* m_pItem;            // +0x04
    CThingCreatureBase* GetPItem() const;   // 0x00A01B50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00(); virtual void v01();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];
    CCombatManager* GetCombatManager();     // 0x00856B10
    void SetAnimationName(const CCharString& name);   // 0x008574D0
};

class CCreatureAction_CombatBoast : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_CombatBoast::Initialise()
{
    unsigned long speech;
    {
        CCombatManager* manager = GetCombatManager();
        CDefPointer<const CCombatDialogueDef> def = manager->DialogueManager.PDialogueDef;
        speech = def->BoastSpeech;
    }
    CCombatManager* manager = GetCombatManager();
    manager->DialogueManager.SaySpeech(*PCreature.GetPItem(), speech);
    {
        CCharString name("SPECIAL_BOAST", -1);
        SetAnimationName(name);
    }
    CCombatActionBase::Initialise();
}