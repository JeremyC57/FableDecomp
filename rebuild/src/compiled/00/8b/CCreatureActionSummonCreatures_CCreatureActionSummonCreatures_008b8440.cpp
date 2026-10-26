// CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures(CThingCreatureBase*) -- base CCombatActionBase(owner, 150, GetSoundPair(owner, null name)), own vtable (0x0127ADDC), the flag at +0x114 cleared
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B8440, length 96 bytes.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0 (null text, default length -1: an implicit conversion of the
// null pointer, re-addressed with `lea`), GetSoundPair(CThingCreatureBase*, const CCharString&) @ 0x008561A0 (__fastcall, 8-byte
// class result through the hidden pointer in ecx), CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&)
// @ 0x00858030, CCharString::~CCharString @ 0x0099EAE0.
// Shape: the landed GetSoundPair constructor family (0x008C4F50) on the three-argument base; the byte is cleared after the vtable. `ret 4`.
class CThingCreatureBase;

class CCharString { public:                        // one pointer; the default length makes the null pointer convert implicitly
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                         // 0x0099EAE0
};

class CSoundPair { public:                         // a user constructor makes the 8-byte result return through the hidden pointer
    long FirstSound;
    long LastSound;
    CSoundPair() : FirstSound(0), LastSound(0) { }
};

CSoundPair __fastcall GetSoundPair(CThingCreatureBase* owner, const CCharString& name);   // 0x008561A0

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
};

class CCreatureAction_SummonCreatures : public CCombatActionBase { public:   // vtable 0x0127ADDC
    unsigned char m_isFromInventory;        // +0x114
    virtual void v00();
    CCreatureAction_SummonCreatures(CThingCreatureBase* owner);
};

CCreatureAction_SummonCreatures::CCreatureAction_SummonCreatures(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 150, GetSoundPair(owner, 0))
    , m_isFromInventory(0)
{
}