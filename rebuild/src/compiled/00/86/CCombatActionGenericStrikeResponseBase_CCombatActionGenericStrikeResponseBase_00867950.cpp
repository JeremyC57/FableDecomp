// CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase() -- base CCombatActionBase(250), PAttacker intelligent pointer at +0x114, flag at +0x135 = true
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00867950, length 51 bytes (+ int3 padding).
// Callees: CCombatActionBase::CCombatActionBase(long) @ 0x00857E40 (the one-argument base constructor, 250),
// CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (the inlined CIntelligentPointer default
// constructor: base call + vtable 0x01237F1C store, no SetPItem).
// Vtable 0x01264734 = CCombatAction_GenericStrikeResponseBase (rtti-vtables.tsv). The member at +0x114 is the
// intelligent pointer that IsOverrideableByActionOfSamePriority (0x00865E70) reads as the attacking thing;
// the byte at +0x135 is set true (name descriptive). No engine header exists for this class.
// Shape: `push esi ; push edi ; push 0FAh ; mov esi,ecx ; call base ; lea edi,[esi+114h] ; mov ecx,edi ;
// mov [esi],vtable ; call CBaseIntelligentPointer ; mov [edi],ptr-vtable ; pop edi ; mov byte [esi+135h],1`.
class CThing;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


class CCombatActionBase { public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(long actionCode);         // 0x00857E40
};

class CCombatAction_GenericStrikeResponseBase : public CCombatActionBase { public:
    CIntelligentPointer<const CThing> PAttacker;              // +0x114
    unsigned char _pad[0x135 - 0x11c];          // +0x11c
    bool m_respondToStrike;                     // +0x135
    virtual void v00();                         // own vtable 0x01264734
    CCombatAction_GenericStrikeResponseBase();
};

CCombatAction_GenericStrikeResponseBase::CCombatAction_GenericStrikeResponseBase()
    : CCombatActionBase(250), m_respondToStrike(true)
{
}