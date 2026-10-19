// CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase*, const CDefPointer<CDefBase>&, CThing*, const SMessageData&, long) -- base CCreatureActionBase(owner, -1.0, 0.0, the owner's STANDARD_IDLE animation, zeroed CSoundPair, 0, 0), then copy the def pointer (add-ref), the 12-byte message data, the recipient intelligent pointer and the last word
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982210, length 194 bytes (+ int3 padding), `ret 14h`. Vtable 0x0129947C.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, the animation lookup by name @ 0x006924B0 (`__fastcall`:
// ecx owner, edx the name), CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, float, float, CAnimationEntry*,
// const CSoundPair&, long, long) @ 0x00693B30, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, CBaseIntelligentPointer::SetPItem @ 0x00A01B90.
// Shape: the literal converts implicitly to a CCharString temporary built before every other base argument (GOTCHAS:
// a literal for a `const CCharString&` parameter is built first) and destroyed after the base constructor; after the
// vtable store the init-list members in declaration order: a CDefPointer copied from a reference (null-tested add-ref),
// a 12-byte record with a memberwise user copy constructor (dword moves, no `lea`), the CIntelligentPointer<CThing>
// (inline converting ctor: base ctor, vtable 0x0123E7D8, SetPItem) and the last argument.
// Layout: no engine header; Ego_r.pdb sizeof 0xd0 with unnamed members -- names are descriptive, offsets from the bytes
// (retail CCreatureActionBase is 0xa8).
struct CThingCreatureBase;
struct CThing;
struct CAnimationEntry;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CSoundPair {
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CAnimationEntry* __fastcall GetAnimationFromName(CThingCreatureBase* owner, const CCharString& name);   // 0x006924B0

struct CCreatureActionBase {                // retail sizeof 0xa8
    virtual void v00();
    unsigned char _body[0xa8 - 4];
    CCreatureActionBase(CThingCreatureBase* owner, float a, float b, CAnimationEntry* animation, const CSoundPair& sound, long c, long d);   // 0x00693B30
};

struct CDefBase {
    void* __vftable;
    long  m_refCount;                       // +4
};
struct CDefPointer {
    CDefBase* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->m_refCount; }
};

struct SMessageData {                       // 12 bytes, memberwise copy
    long m_type;
    long m_value1;
    long m_value2;
    SMessageData(const SMessageData& o) : m_type(o.m_type), m_value1(o.m_value1), m_value2(o.m_value2) { }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CBaseIntelligentPointer();              // 0x00A01B10
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x0123E7D8
    virtual void v00();
    CIntelligentPointer(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
};

struct CCreatureAction_SendMessage : CCreatureActionBase {   // vtable 0x0129947C
    CDefPointer         m_def;              // +0xa8
    SMessageData        m_message;          // +0xac
    CIntelligentPointer m_recipient;        // +0xb8
    long                m_flags;            // +0xc0
    virtual void v00();
    CCreatureAction_SendMessage(CThingCreatureBase* owner, const CDefPointer& def, CThing* recipient, const SMessageData& message, long flags);
};

CCreatureAction_SendMessage::CCreatureAction_SendMessage(CThingCreatureBase* owner, const CDefPointer& def, CThing* recipient, const SMessageData& message, long flags)
    : CCreatureActionBase(owner, -1.0f, 0.0f, GetAnimationFromName(owner, "STANDARD_IDLE"), CSoundPair(), 0, 0)
    , m_def(def)
    , m_message(message)
    , m_recipient(recipient)
    , m_flags(flags)
{
}
