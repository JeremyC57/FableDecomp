// CTCScriptedControl::IsActionRunning(const CCharString& name) -- true when a queued script action's name equals `name`
// Leaf: IsActionRunning
// Retail: Fable.exe 0x00712D70, 123 bytes (`ret 4`); the twin of TerminateActionByName 0x00712CE0 without the store.
// Callees: CTCScriptedControl::CActionBase virtual slot 4 (+0x10, GetName() returning CCharString by value; the
// temporary lives in the dead argument slot), CCharString equality inlined around Compare(const char*, const char*)
// @ 0x00411570 (`__fastcall`), CCharString::~CCharString @ 0x0099EAE0 (manifest "_Cons_val<...>" is propagated).
// Layout = Ego_r.pdb CTCScriptedControl (0x34): PActionList (STLport list<CCountedPointer<CActionBase>>) +0x14;
// CActionBase (0x0C, vtable 0x01265504): PTCScript +0x04, Activated +0x08, Terminated +0x09.
// Shape: the inlined operator== is the two-level form (pointer identity / null tests outside, the length test and
// Compare in an inner inline whose own `return false` gives retail's `xor al,al ; mov bl,al`).
class CCharStringData { public:
    const char*   m_pData;                     // +0x00
    unsigned long m_length;                    // +0x04
};
long __fastcall Compare_00411570(const char* a, const char* b);   // 0x00411570

class CCharString { public:
    CCharStringData* PStringData;
    ~CCharString();                            // 0x0099EAE0
    bool SameText(const CCharString& other) const
    {
        if (PStringData->m_length != other.PStringData->m_length)
            return false;
        return Compare_00411570(PStringData->m_pData, other.PStringData->m_pData) == 0;
    }
    bool operator==(const CCharString& other) const
    {
        if (PStringData == other.PStringData)
            return true;
        if (PStringData && other.PStringData)
            return SameText(other);
        return false;
    }
};
class CTCScriptedControl;
struct CActionBase {                           // CTCScriptedControl::CActionBase, vtable 0x01265504
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual CCharString GetName() const;       // slot 4 (+0x10)
    CTCScriptedControl* PTCScript;             // +0x04
    bool          Activated;                   // +0x08
    bool          Terminated;                  // +0x09
};
class CCPPointerInfo;
struct SActionNode {                           // STLport list node of CCountedPointer<CActionBase>
    SActionNode*    _M_next;                   // +0x00
    SActionNode*    _M_prev;                   // +0x04
    CActionBase*    Data;                      // +0x08
    CCPPointerInfo* Info;                      // +0x0c
};
struct CActionList { SActionNode* _M_node; };
class CTCScriptedControl { public:
    virtual void v00();
    void*         ParentThing;                 // +0x04 (CTCBase)
    bool          ToBeRemoved;                 // +0x08
    bool          IsInGlobalUpdateTCs;         // +0x09
    unsigned char PScriptHandleImp[8];         // +0x0c CIntelligentPointer<CScriptGameResourceObjectScriptedThing>
    CActionList   PActionList;                 // +0x14
    bool IsActionRunning(const CCharString& name);
};

bool CTCScriptedControl::IsActionRunning(const CCharString& name)
{
    for (SActionNode* node = PActionList._M_node->_M_next; node != PActionList._M_node; node = node->_M_next) {
        if (node->Data->GetName() == name)
            return true;
    }
    return false;
}