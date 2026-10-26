// CCreatureAction_BuildUpDivineWrathSpell::CCreatureAction_BuildUpDivineWrathSpell(CThingCreatureBase*, CCharString) -- base CCombatActionBase(owner, 150, zeroed CSoundPair), own vtable (0x0127E8D4), then the by-value animation name is applied
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BD170, length 80 bytes.
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030,
// CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0
// Shape: the second argument is a CCharString passed BY VALUE (one dword in the argument area: retail passes its
// address, `lea edx,[esp+14h]`, to the setter and destroys it in place before returning -- __fastcall callee-destroyed
// by-value parameter, hence `ret 8` with a destructor call on the argument slot). No local string is built.
class CThingCreatureBase;

class CSoundPair { public:                         // two sound ids, zeroed by the inline constructor (the register-zeroed
    long FirstSound;                        //   stack stores before the base call: an embedded object's inline ctor)
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

class CCharString { public:                        // retail CCharString is one pointer (fable_string.h)
    char* m_pData;
    ~CCharString();                         // 0x0099EAE0
};

class CCombatActionBase { public:                  // retail sizeof 0x114 (the PDB build is 0x12c: three containers are 4 bytes wider there)
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};

class CCreatureAction_BuildUpDivineWrathSpell : public CCombatActionBase { public:             // vtable 0x0127E8D4; adds no data
    virtual void v00();
    CCreatureAction_BuildUpDivineWrathSpell(CThingCreatureBase* owner, CCharString animName);
};

CCreatureAction_BuildUpDivineWrathSpell::CCreatureAction_BuildUpDivineWrathSpell(CThingCreatureBase* owner, CCharString animName)
    : CCombatActionBase(owner, 150, CSoundPair())
{
    SetCombatAnimation(animName);             // 0x008574D0
}