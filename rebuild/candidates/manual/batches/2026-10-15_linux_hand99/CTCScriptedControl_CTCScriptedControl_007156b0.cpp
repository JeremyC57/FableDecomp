// CTCScriptedControl::CTCScriptedControl(CThing* thing) -- CTCBase(thing), own vtable, an empty script-handle intelligent pointer, an empty action list, unlocked, priority 0, an empty attitude-tag list
// Leaf: ??0CTCScriptedControl
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007156B0, length 92 bytes.
// Callees: CTCBase::CTCBase(CThing*) @ 0x00686800, CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10, malloc
// @ 0x00BFEA0E (twice, the two lists' sentinel nodes). Vtables 0x012658E4 = CTCScriptedControl, 0x0126552C = CIntelligentPointer
// (rtti-vtables.tsv): the chunk label "CTCRegionDisplay::CTCRegionDisplay" is propagated.
// Layout = Ego_r.pdb CTCScriptedControl (0x34) with 4-byte retail lists (one sentinel pointer, as the landed
// CTCCreatureExpression constructor 0x00729360): PScriptHandleImp (CIntelligentPointer<CScriptGameResourceObjectScriptedThing>)
// +0x0c, PActionList (list<CCountedPointer<CActionBase>>, 16-byte nodes) +0x14, Locked +0x18, ScriptAIPriority +0x1c,
// AttitudeTags (list<pair<long, EConversationAttitude>>, 16-byte nodes) +0x20.
// Shape: speed TU; the intelligent pointer is the out-of-line base constructor plus the inline vtable store through `edi`; each
// list is the inline VC6 constructor (head nulled from the `xor ebx,ebx` register, node self-linked next then prev, head
// stored); the two mallocs' arguments are popped together (`add esp,8`) after the second.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E

struct CThing;

struct CTCBase_Calls {                                      // engine/CTCBase.h layout (0xc bytes)
    virtual void v00();
    CThing* ParentThing;                                    // +0x04
    bool    ToBeRemoved;                                    // +0x08
    bool    IsInGlobalUpdateTCs;                            // +0x09
    CTCBase_Calls(CThing* thing);                           // 0x00686800
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* PData;                                            // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
};
struct CIntelligentPointer : CBaseIntelligentPointer {      // vtable 0x0126552C
    virtual void v00();
    CIntelligentPointer() { }
};
#pragma pack(pop)

template<class T>
struct CRetailList {                                        // retail std::list: one sentinel pointer
    struct node { node* next; node* prev; T value; };
    node* head;
    CRetailList() : head(0)
    {
        node* sentinel = (node*)malloc(sizeof(node));
        sentinel->next = sentinel;
        sentinel->prev = sentinel;
        head = sentinel;
    }
};

struct SCountedActionPointer { void* m_pObject; void* m_pBlock; };   // CCountedPointer<CTCScriptedControl::CActionBase>
struct SAttitudeTag { long m_tag; long m_attitude; };              // pair<long, EConversationAttitude>

struct CTCScriptedControl : CTCBase_Calls {                 // vtable 0x012658E4
    CIntelligentPointer                PScriptHandleImp;    // +0x0c
    CRetailList<SCountedActionPointer> PActionList;         // +0x14
    bool                               Locked;              // +0x18
    long                               ScriptAIPriority;    // +0x1c EScriptAIPriority
    CRetailList<SAttitudeTag>          AttitudeTags;        // +0x20
    virtual void v00();
    CTCScriptedControl(CThing* thing);
};

CTCScriptedControl::CTCScriptedControl(CThing* thing)
    : CTCBase_Calls(thing)
    , PScriptHandleImp()
    , PActionList()
    , Locked(false)
    , ScriptAIPriority(0)
    , AttitudeTags()
{
}
