// CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(CThingCreatureBase*) -- base CCombatActionBase(owner, 250, zeroed CSoundPair), own vtable, the name "ST_TELEPORT", then the creature's current position as the target
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BCEA0, length 127 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) @ 0x00858030, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (literal 0x0127E488 "ST_TELEPORT"), CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0,
// CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CThing::GetPosition @ 0x004C73D0. Own vtable 0x0127E344 (rtti-vtables.tsv).
// Shape: `ret 4`; the landed 0x008CBCF0 constructor shape (sound pair zeroed from eax, scoped named CCharString in the 8-byte frame), then a body
// assignment of the whole C3DVector (TargetPos +0x114 addressed through `lea ecx,[esi+114h]`: a struct assignment).
// The PDB header (CCreatureAction_TeleportOut.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
class CThingCreatureBase;
class C3DVector { public: float X, Y, Z; };
class CThing { public:
    const C3DVector& GetPosition() const;   // 0x004C73D0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
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
    unsigned char _pad04[4];                // +0x04
    CIntelligentPointer<CThing> PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
    unsigned char _pad10[0x114 - 0x10];     // +0x10
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);                 // 0x00858030
    CCombatActionBase(CThingCreatureBase* owner, CThing* thing, long actionCode, const CSoundPair& sound);  // 0x00857F30
public:
    void SetCombatAnimation(const CCharString&);   // 0x008574D0
};
class CCreatureAction_TeleportOut : public CCombatActionBase { public:   // vtable 0x0127E344
    C3DVector TargetPos;                    // +0x114
    bool Instant;                           // +0x120
    virtual void v00();
    CCreatureAction_TeleportOut(CThingCreatureBase* owner);
};

CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(CThingCreatureBase* owner)
    : CCombatActionBase(owner, 250, CSoundPair())
{
    {
        CCharString name("ST_TELEPORT", -1);   // scoped named local (a bare temporary would reuse eax, GOTCHAS)
        SetCombatAnimation(name);
    }
    TargetPos = PCreature.GetPItem()->GetPosition();
}