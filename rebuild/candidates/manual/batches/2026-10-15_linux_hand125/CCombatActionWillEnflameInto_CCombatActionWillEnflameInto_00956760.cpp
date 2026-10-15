// CCombatAction_WillEnflameInto::CCombatAction_WillEnflameInto(CThingCreatureBase*, long, long) -- constructor
// Leaf: ??0CCombatAction_WillEnflameInto
// Retail: Fable.exe (TLC, VC7.1) @ 0x00956760, 112-byte slot (102 bytes of code).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0 (__fastcall,
//          CSoundPair returned through the hidden pointer in ecx, owner in edx),
//          CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
//          CCharString::~CCharString() @ 0x0099EAE0.
// Shape: the base constructor gets action code 150 and GetSoundPair(owner, "SWORD_SWING"): the literal
// converts implicitly to a CCharString temporary (re-addressed with `lea`), the 8-byte sound pair lands
// in a frame temporary; the string dies after the base call, then the vtable 0x0129461C and the two long
// arguments are stored (both loaded before `pop edi`). Level is the PDB member (+0x12c there, +0x114 in
// retail where CCombatActionBase is 0x114 bytes); the word at +0x118 is not in the PDB layout -- the
// retail class carries the third constructor argument there.
struct CThingCreatureBase;

struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

struct CSoundPair {                       // user constructor: returned through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

struct CCombatActionBase {                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

struct CCombatAction_WillEnflameInto : CCombatActionBase {   // vtable 0x0129461C
    long Level;                             // +0x114
    long m_spellArgument;                   // +0x118 (third constructor argument; absent from the PDB layout)
    virtual void v00();
    CCombatAction_WillEnflameInto(CThingCreatureBase* owner, long level, long spellArgument);
};

CCombatAction_WillEnflameInto::CCombatAction_WillEnflameInto(CThingCreatureBase* owner, long level, long spellArgument)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, "SWORD_SWING")),
      Level(level),
      m_spellArgument(spellArgument)
{
}
