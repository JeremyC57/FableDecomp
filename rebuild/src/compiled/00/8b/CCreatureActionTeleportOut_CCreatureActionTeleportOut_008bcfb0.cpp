// CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(CThingCreatureBase*, CThing*, const C3DVector&, bool) -- base CCombatActionBase(owner, thing, 250, zeroed CSoundPair), own vtable, the target position and flag, then the name "ST_TELEPORT_OUT"
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008BCFB0, length 135 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(CThingCreatureBase*, CThing*, long, const CSoundPair&) @ 0x00857F30, CCharString::CCharString(const char*, long)
// @ 0x0099EBF0 (literal 0x0127E494 "ST_TELEPORT_OUT"), CCombatActionBase::SetCombatAnimation(const CCharString&) @ 0x008574D0, CCharString::~CCharString @ 0x0099EAE0.
// Own vtable 0x0127E344 (rtti-vtables.tsv).
// Shape: `ret 10h`; as the three-argument overload 0x008BCF20 with the thing argument forwarded to the four-argument base constructor; TargetPos
// goes through C3DVector's memberwise copy constructor (fixed-offset stores; an implicit copy re-bases with `lea`).
// Leaf: ??0CCreatureAction_TeleportOut@@QAE@PAVCThingCreatureBase@@PAVCThing@@ABVC3DVector@@_N@Z
// The PDB header (CCreatureAction_TeleportOut.h) flattens the base; modelled locally (CCombatActionBase is 0x114 bytes in retail).
#include "fable_string.h"
class CThingCreatureBase;
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise copy constructor: fixed-offset word moves
};
class CThing { public:
    const C3DVector& GetPos() const;   // 0x004C73D0
};
class CSoundPair { public:
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                        // +4
    CThing* GetPItem() const;               // 0x00A01B50
};

class CCombatActionBase { public:                  // retail sizeof 0x114
    virtual void v00();
    unsigned char _pad04[4];                // +0x04
    CBaseIntelligentPointer PCreature;      // +0x08 CIntelligentPointer<CThingCreatureBase>
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
    CCreatureAction_TeleportOut(CThingCreatureBase* owner, CThing* thing, const C3DVector& target, bool instant);
};

CCreatureAction_TeleportOut::CCreatureAction_TeleportOut(CThingCreatureBase* owner, CThing* thing, const C3DVector& target, bool instant)
    : CCombatActionBase(owner, thing, 250, CSoundPair())
    , TargetPos(target)
    , Instant(instant)
{
    {
        CCharString name("ST_TELEPORT_OUT", -1);   // scoped named local
        SetCombatAnimation(name);
    }
}