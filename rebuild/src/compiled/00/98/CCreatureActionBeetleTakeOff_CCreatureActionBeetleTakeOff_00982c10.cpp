// CCreatureAction_BeetleTakeOff::CCreatureAction_BeetleTakeOff(CThingCreatureBase*) -- base CCombatActionBase(owner, 0, zeroed CSoundPair), own vtable (0x012995EC, rtti-vtables.tsv), then the animation name "TAKE_OFF"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00982C10, length 96 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCharString::CCharString(const char*, long) @ 0x0099EBF0 (literal "TAKE_OFF"), CCombatActionBase::SetCombatAnimation
// @ 0x008574D0 (manifest label "CTCInGameMenu::SetPDef" is propagated; the landed combat-action shells name it so),
// CCharString::~CCharString @ 0x0099EAE0
// Shape: the sound pair is built zeroed on the stack by its inline constructor (register-zeroed stores before the
// base call), the vtable store sits between the base call and the scoped CCharString (a named local in its own
// block: a bare temporary would reuse the constructor's returned pointer, GOTCHAS), the name is destroyed before
// `this` is returned.
class CThingCreatureBase;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCharString { public:                        // retail CCharString is one pointer (fable_string.h); declared locally for the
    char* m_pData;                          //   defaulted length of the conversion constructor
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BeetleTakeOff : public CCombatActionBase { public:             // vtable 0x012995EC; adds no data
    virtual void v00();
    CCreatureAction_BeetleTakeOff(CThingCreatureBase* owner);
};

CCreatureAction_BeetleTakeOff::CCreatureAction_BeetleTakeOff(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 0, CSoundPair())
{
    {
        CCharString name("TAKE_OFF");         // conversion-constructor length default (-1), re-addressed with lea
        SetCombatAnimation(name);            // 0x008574D0
    }
}