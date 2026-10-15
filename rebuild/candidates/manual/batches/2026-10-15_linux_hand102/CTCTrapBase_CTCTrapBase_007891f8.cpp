// CTCTrapBase::~CTCTrapBase() -- releases the trap def, destroys the trigger/immune/hit containers and the obstruction pointer, then ~CTCBase (tail jump)
#pragma optimize("s", on)
// Leaf: ??1CTCTrapBase
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007891F8, length 64 bytes. Called from the deleting destructors
// 0x007894B7 (CTCTrapBase, vtable 0x0126B7FC slot 0), 0x007894D3 (CTCMeleeTrap, 0x0126B884) and 0x007894EF
// (CTCProjectileTrap, 0x0126B90C), so the catalog label "CTCTargetingBase::~CTCTargetingBase" is propagated and rejected.
// Callees: vector<CIntelligentPointer<CThing>>::~vector @ 0x004406D0 (x2), CBaseIntelligentPointer::~CBaseIntelligentPointer
// @ 0x00A01C10, vector<pair<CIntelligentPointer<CThing>,long>>::~vector @ 0x004E1B08 (manifest "CActiveFile::OnReadFinished"
// is propagated), CTCBase::~CTCBase @ 0x00686830.
// Layout: engine/CTCTrapBase.h (Ego_r.pdb) has HitThings +0x0c, ImmuneThings +0x1c, PObstruction +0x2c, TriggerThings
// +0x34, TrapOpener +0x44, PTrapDef +0x60, Active +0x64 with 16-byte vectors; the retail vectors are 12 bytes, so every
// member after each vector sits 4 bytes lower (HitThings +0x0c, ImmuneThings +0x18, PObstruction +0x24, TriggerThings
// +0x2c, TrapOpener +0x38, PTrapDef +0x54, Active +0x58 -- the landed ManualReset 0x00787C0C reads Active at +0x58).
// The header's layout therefore disagrees with the bytes and the class is modelled locally with its member names.
// Shape: the ~CTCTargetingBase 0x004DA8B7 shape (no vtable store: `__declspec(novtable)`, size TU, inline def release with
// `and [..],0`); TrapOpener (CFadeInFadeOutBase) has a trivial destructor and emits nothing.
struct CThing;
struct CBaseObject;

struct CTCBase {
    virtual void v00();
    CThing* ParentThing;                     // +0x04
    bool    ToBeRemoved;                     // +0x08
    bool    IsInGlobalUpdateTCs;             // +0x09
    ~CTCBase();                              // 0x00686830
};

struct CDefPointee {                         // counted def: count at +4, slot 1 destroys
    virtual void v00();
    virtual void Destroy();                  // slot 1 (+4)
    long m_refCount;                         // +0x04
};

struct CDefPointer {                         // CDefPointer<const CTrapDef>
    CDefPointee* Object;
    __forceinline ~CDefPointer()
    {
        CDefPointee* def = Object;
        if (def) {
            if (--def->m_refCount == 0)
                def->Destroy();
            Object = 0;
        }
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CBaseObject* m_pItem;                    // +0x04
    ~CBaseIntelligentPointer();              // 0x00A01C10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>
};

struct CHitThingVector {                     // retail vector<pair<CIntelligentPointer<CThing>,long> >: first, last, end
    void* first;
    void* last;
    void* end;
    ~CHitThingVector();                      // 0x004E1B08
};

struct CThingPointerVector {                 // retail vector<CIntelligentPointer<CThing> >: first, last, end
    void* first;
    void* last;
    void* end;
    ~CThingPointerVector();                  // 0x004406D0
};

struct CFadeInFadeOutBase {                  // 0x1c bytes, trivially destructible here
    unsigned char _body[0x1c];
};

struct __declspec(novtable) CTCTrapBase : CTCBase {
    CHitThingVector     HitThings;           // +0x0c
    CThingPointerVector ImmuneThings;        // +0x18
    CIntelligentPointer PObstruction;        // +0x24
    CThingPointerVector TriggerThings;       // +0x2c
    CFadeInFadeOutBase  TrapOpener;          // +0x38
    CDefPointer         PTrapDef;            // +0x54
    bool                Active;              // +0x58
    bool                Triggered;           // +0x59
    bool                LastTriggerOnProximity;   // +0x5a
    long                TrapType;            // +0x5c ETrapType
    ~CTCTrapBase();
};

CTCTrapBase::~CTCTrapBase()
{
}
