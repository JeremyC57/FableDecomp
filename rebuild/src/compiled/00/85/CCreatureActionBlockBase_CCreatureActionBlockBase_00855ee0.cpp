// CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase&) -- copy constructor: base CCombatActionBase(other), own vtable, BlowStrength and MovementDirection copied
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00855EE0, length 75 bytes.
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0 (manifest).
// The manifest label "CQuickAccessItemButtonGuiDef::CQuickAccessItemButtonGuiDef" is propagated and rejected: the
// vtable stored is 0x01275F3C, CCreatureAction_BlockBase's (rtti-vtables.tsv), and the landed
// CCreatureAction_BlockRespond copy constructor (0x00855E80) calls this body as its base copy.
// Layout: PDB BlowStrength +0x12c, MovementDirection (C3DVector) +0x130 over a 0x12c-byte CCombatActionBase; the
// retail base is 0x114 bytes (engine/CCreatureAction_BlockBase.h exists but carries the PDB 0x13c layout and no
// copy constructor, so the class is modelled locally as in the landed BlockRespond copy).
// Shape: BlowStrength through eax, then C3DVector's memberwise copy constructor (one register per dword:
// ecx, edx, eax), `mov eax,esi` returns `this`.
#include "engine/C3DVector.h"

class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_BlockBase : public CCombatActionBase { public:  // vtable 0x01275F3C, retail sizeof 0x124
    long      BlowStrength;                     // +0x114
    C3DVector MovementDirection;                // +0x118
    virtual void v00();
    CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other);
};

CCreatureAction_BlockBase::CCreatureAction_BlockBase(const CCreatureAction_BlockBase& other)
    : CCombatActionBase(other)
    , BlowStrength(other.BlowStrength)
    , MovementDirection(other.MovementDirection)
{
}