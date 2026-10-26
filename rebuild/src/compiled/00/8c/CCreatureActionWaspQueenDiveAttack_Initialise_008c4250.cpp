// CCreatureAction_WaspQueenDiveAttack::Initialise() -- unless the creature already has a hit-on-collision component (interface type 200), adds "CTCHitOnCollision" to it and arms its two bytes at +0x18/+0x19; then the base Initialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008C4250, length 96 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8, twice), CCharString::CCharString(const char*, long) @ 0x0099EBF0,
// CThing::AddTC(const CCharString&, bool, CTCParamsBase*) @ 0x004C9D60 (landed naming of 0x008C08B0), CCharString::~CCharString @ 0x0099EAE0,
// CCombatActionBase::Initialise @ 0x00857520 (non-virtual call).
// Shape: the presence test is the inlined bitset probe (type 200 = bit 8 of the dword at +0x38, `test ch,1`); the name is a scoped named local
// (re-addressed with `lea` for the argument), the new component rides in esi across the string destructor and is null-checked before the two stores, which share one register value (a chained assignment).
#include "fable_string.h"
class CTCParamsBase;
class CTCHitOnCollision { public:                  // the component AddTC returns: two arming bytes
    unsigned char _pad[0x18];
    unsigned char m_hitsCreatures;          // +0x18
    unsigned char m_hitsObjects;            // +0x19
};
class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];         // +0x20 std::bitset of interface types
    CTCHitOnCollision* AddTC(const CCharString& name, bool activate, CTCParamsBase* params);   // 0x004C9D60
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    virtual void Initialise();              // 0x00857520
    unsigned char _pad04[4];
    CIntelligentPointer<CThing> PCreature;      // +8
    unsigned char _pad10[0x114 - 0x10];
};

class CCreatureAction_WaspQueenDiveAttack : public CCombatActionBase { public:
    virtual void Initialise();
};

void CCreatureAction_WaspQueenDiveAttack::Initialise()
{
    if (!(PCreature.GetPItem()->m_presentBits[200 >> 5] & (1UL << (200 & 31)))) {   // the inlined HasTC probe: `test ch,1` on the dword at +0x38
        CTCHitOnCollision* pHit;
        {
            CCharString name("CTCHitOnCollision", -1);
            pHit = PCreature.GetPItem()->AddTC(name, false, 0);
        }
        if (pHit)
            pHit->m_hitsObjects = pHit->m_hitsCreatures = 1;   // chained: one register value stored twice (two immediates otherwise)
    }
    CCombatActionBase::Initialise();
}