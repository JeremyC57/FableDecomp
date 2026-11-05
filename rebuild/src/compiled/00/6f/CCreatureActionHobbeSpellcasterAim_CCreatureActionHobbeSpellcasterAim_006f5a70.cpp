// CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim() -- default constructor
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006F5A70, length 39 bytes (+ int3 padding).
// Callees: the aim-action base constructor @ 0x008BFB00 (itself CCreatureActionBase(0x32) +
// vtable 0x0127F614, CCreatureAction_GenericAimProjectileWeapon in rtti-vtables.tsv), then
// CBaseIntelligentPointer::CBaseIntelligentPointer() @ 0x00A01B10 for the member at +0x114
// followed by its inline CIntelligentPointer vtable store (0x01238C6C). Own vtable 0x01263CF4
// (CCreatureAction_HobbeSpellcasterAim). The manifest label (CAIStateGroupBehaviourBase ctor)
// is propagated garbage; the class is read off the vtable.
class CCreatureAction_GenericAimProjectileWeapon { public:
    virtual void v00();
    CCreatureAction_GenericAimProjectileWeapon();      // 0x008BFB00
    unsigned char _body[0x110];             // +0x4..+0x113
};

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CThing;

class CCreatureAction_HobbeSpellcasterAim : public CCreatureAction_GenericAimProjectileWeapon { public:   // vtable 0x01263CF4
    CIntelligentPointer<CThing> m_pTarget;          // +0x114
    CCreatureAction_HobbeSpellcasterAim();
};

CCreatureAction_HobbeSpellcasterAim::CCreatureAction_HobbeSpellcasterAim()
    : m_pTarget()
{
}