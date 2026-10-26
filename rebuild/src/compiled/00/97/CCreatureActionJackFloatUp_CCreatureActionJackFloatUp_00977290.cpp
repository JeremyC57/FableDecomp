// CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(CThingCreatureBase*, long) -- base CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), long member at +0x114
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00977290, length 90 bytes (+ int3 padding).
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 ("SWORD_SWING" @ 0x0127AC80, -1),
// GetSoundPair(CThingCreatureBase*, const CCharString&) @ 0x008561A0 (__fastcall: CSoundPair returned through the
// hidden pointer in ecx, the owner in edx, the string reference pushed), CCombatActionBase::CCombatActionBase
// (CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0.
// Vtable 0x01297F54 = CCreatureAction_JackFloatUp (rtti-vtables.tsv), so the manifest module CCombatAction_WillEnflameInto is a propagated label and wrong (action code 250 here, 150 in the enflame/heal twins).
// Shape: the string temporary lives in the frame for the whole initialiser expression (destroyed after the
// base call); the sound pair result is built at [esp+8] and its address is pushed as the reference argument, then the vtable
// store and the member store from the second argument (mov ecx,[esp+1Ch]). The engine header
// (CCombatAction_WillEnflameInto.h, base sizeof 0x12c) disagrees with the retail base size 0x114, so the
// layout is modelled locally with the PDB member name.
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

class CCreatureAction_JackFloatUp : public CCombatActionBase { public:
    long m_height;                                 // +0x114
    virtual void v00();                         // own vtable 0x01297F54
    CCreatureAction_JackFloatUp(CThingCreatureBase* owner, long value);
};

CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(CThingCreatureBase* owner, long value)
    : CCombatActionBase(owner, 250, GetSoundPair(owner, "SWORD_SWING")), m_height(value)
{
}