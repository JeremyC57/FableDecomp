// CCreatureAction_OnGenerate::CCreatureAction_OnGenerate(CThingCreatureBase*) -- base CCombatActionBase(owner, 250, GetSoundPair(owner, CCharString(0)))
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0074DBB0, length 79 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (null text, -1), GetSoundPair(const CCharString&,
// CThingCreatureBase*) @ 0x008561A0 -- really (CThingCreatureBase*, const CCharString&), __fastcall: hidden result pointer
// in ecx, owner in edx, the string reference pushed -- CCombatActionBase::
// CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x01268DEC = CCreatureAction_OnGenerate (rtti-vtables.tsv), so the manifest module
// CCombatAction_Guildmaster_HealFollowersInto is a propagated label and wrong. Same shape as the
// WillEnflameInto family (0x009567F0) with an empty sound name and no extra member: `ret 4`.
// The string argument is a CONVERSION temporary (the literal converts through CCharString(const char*, long = -1)):
// an explicitly written CCharString("...", -1) temporary reuses the constructor's returned eax as the reference and
// stores the member before the vtable, which is 4 bytes shorter than retail. fable_string.h declares the constructor
// without the default, so the string is modelled locally (same ctor/dtor addresses).
struct CCharString_Retail {
    void* m_pData;
    CCharString_Retail(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString_Retail();                                   // 0x0099EAE0
};

class CThingCreatureBase;

class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString_Retail& name);   // 0x008561A0

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureAction_OnGenerate : public CCombatActionBase { public:
    virtual void v00();                         // own vtable 0x01268DEC
    CCreatureAction_OnGenerate(CThingCreatureBase* owner);
};

CCreatureAction_OnGenerate::CCreatureAction_OnGenerate(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, static_cast<const char*>(0)))
{
}