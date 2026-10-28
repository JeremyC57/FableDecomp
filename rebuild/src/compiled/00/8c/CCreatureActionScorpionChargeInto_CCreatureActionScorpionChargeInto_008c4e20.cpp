// CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(CThingCreatureBase*, CThing*, long) -- base CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING")), an empty target pointer, the charge value
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4E20, length 114 bytes (+ int3 padding). Vtable 0x01280ED4.
// Model: rebuild/include/engine/CCreatureAction_ScorpionChargeInto.h is the flattened PDB layout and sits high against the bytes (CCombatActionBase is 0x12c bytes in the PDB, 0x114 in retail: retail containers are 12 bytes), so `this` is a local model with the retail offsets.
// Callees: CCharString::CCharString(const char*, long) @ 0x0099EBF0, GetSoundPair @ 0x008561A0, CCombatActionBase::CCombatActionBase(
// CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10.
// Shape: after the vtable store the CIntelligentPointer member at +0x118 is built (base ctor, then its vtable 0x01238C6C), and the
// third argument is stored to +0x114 last (a body assignment: scalars stored after member constructors are body code).
class CThingCreatureBase;
class CThing;

class CCharString { public:                        // one pointer; the default length makes a literal convert implicitly
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
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                  // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);   // 0x00857F30
    void SetCombatAnimation(const CCharString& name);   // 0x008574D0
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCreatureAction_ScorpionChargeInto : public CCombatActionBase { public:   // vtable 0x01280ED4
    long m_chargeCount;                     // +0x114 (descriptive)
    CIntelligentPointer<CThing> PTarget;            // +0x118
    virtual void v00();
    CCreatureAction_ScorpionChargeInto(CThingCreatureBase* owner, CThing* thing, long chargeCount);
};

CCreatureAction_ScorpionChargeInto::CCreatureAction_ScorpionChargeInto(CThingCreatureBase* owner, CThing* thing, long chargeCount)
    : CCombatActionBase(owner, thing, 150, GetSoundPair(owner, "SWORD_SWING"))
{
    m_chargeCount = chargeCount;
}