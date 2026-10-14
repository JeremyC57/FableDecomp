// CCreatureAction_TrollVulnerableInto::CCreatureAction_TrollVulnerableInto(CThingCreatureBase*) -- base CCombatActionBase(owner, 250, zeroed CSoundPair), own vtable (0x01285DEC, rtti-vtables.tsv), then the animation name "VULNERABLE_INTO"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E4420, length 90 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "VULNERABLE_INTO"), CCombatActionBase::SetAnimationName
// @ 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated; the landed combat-action shells name it so),
// CCharString::~CCharString @ 0x0099EAE0
// Shape: the sound pair is built zeroed on the stack by its inline constructor (register-zeroed stores before the
// base call), the vtable store sits between the base call and the scoped CCharString (a named local in its own
// block: a bare temporary would reuse the constructor's returned pointer, GOTCHAS), the name is destroyed before
// `this` is returned.
struct CThingCreatureBase;

struct CSoundPair {                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

struct CCharString {                        // retail CCharString is one pointer (fable_string.h); declared locally for the
    char* m_pData;                          //   defaulted length of the conversion constructor
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

struct CCombatActionBase {                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    void SetAnimationName(const CCharString& name);                                           // 0x008574D0
};

struct CCreatureAction_TrollVulnerableInto : CCombatActionBase {             // vtable 0x01285DEC; adds no data
    virtual void v00();
    CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner);
};

CCreatureAction_TrollVulnerableInto::CCreatureAction_TrollVulnerableInto(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 250, CSoundPair())
{
    {
        CCharString name("VULNERABLE_INTO");         // conversion-constructor length default (-1), re-addressed with lea
        SetAnimationName(name);            // 0x008574D0
    }
}
