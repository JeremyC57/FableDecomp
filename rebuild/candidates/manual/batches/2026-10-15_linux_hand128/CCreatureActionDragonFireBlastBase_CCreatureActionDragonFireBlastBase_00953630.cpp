// CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(CThingCreatureBase*, CThing*) -- combat action on a target with the SWORD_SWING sound pair (action code 250) a default-constructed intelligent pointer member and a zeroed CEnginePrimitiveHandle
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00953630, length 132 bytes, `ret 8`.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair (__fastcall) @ 0x008561A0,
//          CCombatActionBase::CCombatActionBase(owner, target, actionCode, const CSoundPair&) @ 0x00857F30,
//          CCharString::~CCharString @ 0x0099EAE0, CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Vtables: CCreatureAction_DragonFireBlastBase 0x01293F9C, CIntelligentPointer 0x01238C6C (rtti-vtables.tsv).
// Shape: the landed SnapDragon/DragonLand base call (implicit "SWORD_SWING" CCharString temporary, CSoundPair through
//   the hidden pointer, string destroyed after the base); after the class vtable store the member pointer at +0x114 is
//   addressed once in edi: out-of-line base constructor, then the inline CIntelligentPointer constructor's vtable store;
//   the CEnginePrimitiveHandle at +0x11c (CBase::CBase @ 0x0099A2D0, vtable 0x0122F598) is re-addressed in edi the same
//   way and its inline constructor zeroes the handle word after its vtable store.
// No engine header exists for this class; the retail CCombatActionBase is 0x114 bytes.
struct CThingCreatureBase;
struct CThing;

struct CCharString {                        // one pointer; the defaulted length makes a literal convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                         // user constructor: the 8-byte result comes back through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CBaseIntelligentPointer {            // 8 bytes: vtable + item pointer
    virtual void v00();
    void* m_pItem;                          // +4
    CBaseIntelligentPointer();              // 0x00A01B10
};

struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
};

struct CBase {                              // engine root object: vtable only
    virtual void v00();
    CBase();                                // 0x0099A2D0
};

struct CEnginePrimitiveHandle : CBase {     // vtable 0x0122F598; 8 bytes
    long m_id;                              // +4, zero = no primitive
    virtual void v00();
    CEnginePrimitiveHandle() : m_id(0) { }
};

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, CThing* target, long actionCode, const CSoundPair& sound);   // 0x00857F30
};

struct CCreatureAction_DragonFireBlastBase : CCombatActionBase {   // vtable 0x01293F9C
    CIntelligentPointer PTarget;            // +0x114 CIntelligentPointer<...>
    CEnginePrimitiveHandle FireHandle;      // +0x11c
    virtual void v00();
    CCreatureAction_DragonFireBlastBase(CThingCreatureBase* owner, CThing* target);
};

CCreatureAction_DragonFireBlastBase::CCreatureAction_DragonFireBlastBase(CThingCreatureBase* owner, CThing* target)
    : CCombatActionBase(owner, target, 250, GetSoundPair(owner, "SWORD_SWING"))
{
}
