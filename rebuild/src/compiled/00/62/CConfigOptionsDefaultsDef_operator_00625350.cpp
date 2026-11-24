// NUISystem::CUIStateDef::operator=(const CUIStateDef& other) -- def assignment: the CParentDefClassBase part out of line, then seven words, a 16-byte record, a word, a member with its own assignment and a flag byte
// Leaf: ??4CUIStateDef@NUISystem@@QAEAAV01@ABV01@@Z
// Name fix (NF382, QUEUE q2; unit with binder 0x006314C0): was CConfigOptionsDefaultsDef::operator=. The 0x7c element it assigns
//   is NUISystem::CUIStateDef (engine/NUISystem_CUIStateDef.h, sizeof 0x7c: GraphicIndex +0x3c .. LinearChange +0x78, the +0x58
//   16-byte record = Colour, the +0x6c member with its own assignment = ChildrenNotAffected); Ego_r CConfigOptionsDefaultsDef is
//   0x74 with floats from +0x58 (as the old note says). Callers: the landed STLport __copy<CUIStateDef*> 0x006314C0 and the landed
//   binders 0x00625340 (CUIStateDef::Copy) / 0x00632F80 / 0x00632FF0 spell ??4CUIStateDef@NUISystem; FableWin ui_def1.obj copy
//   helpers and CUIStateDef::Copy call ??4CUIStateDef@NUISystem@@QAEAAV01@ABV01@@Z. Same bytes.
// Pick-any form (NF387, T3 nf382 0x0067C490 line): operator= is defined in the class, so it is an inline member and its COMDAT is
//   pick-any (Any), like the copy the landed CUIStateDef::Copy 0x00625340 emits; HEAD's out-of-line definition was NoDuplicates (a
//   duplicate at link beside it). Same body, same bytes.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625350, 115 bytes (128 with padding).
// Callees: CParentDefClassBase::operator= 0x00431ED0, the +0x6c member's operator= 0x0045BC09 (manifest
//          label CActiveFile::operator=, propagated).
//
// Shape: the def is modelled by the copy it performs, with the member types kept from the landed model (the shared
// engine/NUISystem_CUIStateDef.h declares no operator= and types the +0x6c member as a vector, whose assignment
// spelling would move the 0x0045BC09 callee -- header debt, not this rename). The base is 0x3c bytes. GOTCHAS wave 23d: the DDK out-lines an implicit operator= of a record
// owning a member with its own assignment, so the memberwise copy is written out: the seven words go
// through eax/ecx/edx in order, the 16-byte POD record at +0x58 is one implicit record assignment
// (`lea` of both ends + four moves), the +0x68 word is stored around the member call, the byte last.
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

struct SRecord16 {
    long m_values[4];
};

struct CAssignedMember {
    void* m_words[3];
    CAssignedMember& operator=(const CAssignedMember& other);   // 0x0045BC09
};

namespace NUISystem {
class CUIStateDef : public CParentDefClassBase { public:
    long m_3c;                              // +0x3c
    long m_40;                              // +0x40
    long m_44;                              // +0x44
    long m_48;                              // +0x48
    long m_4c;                              // +0x4c
    long m_50;                              // +0x50
    long m_54;                              // +0x54
    SRecord16 m_58;                         // +0x58
    long m_68;                              // +0x68
    CAssignedMember m_6c;                   // +0x6c
    bool m_78;                              // +0x78
    CUIStateDef& operator=(const CUIStateDef& other)   // class-inline: VC7.1 emits it as a pick-any COMDAT (selection Any)
    {
        CParentDefClassBase::operator=(other);
        m_3c = other.m_3c;
        m_40 = other.m_40;
        m_44 = other.m_44;
        m_48 = other.m_48;
        m_4c = other.m_4c;
        m_50 = other.m_50;
        m_54 = other.m_54;
        m_58 = other.m_58;
        m_68 = other.m_68;
        m_6c = other.m_6c;
        m_78 = other.m_78;
        return *this;
    }
};

}  // namespace NUISystem

namespace {
// Fidelity: scaffolding, not retail: the address of the class-inline operator=, so this TU emits it out of line as a pick-any COMDAT, as
// retail's UI def TUs do (the landed CUIStateDef::Copy 0x00625340 carries an Any copy too). Internal to this TU.
typedef NUISystem::CUIStateDef& (NUISystem::CUIStateDef::*TUIStateDefAssign)(const NUISystem::CUIStateDef&);
TUIStateDefAssign ReferenceUIStateDefAssign()
{
    return &NUISystem::CUIStateDef::operator=;
}
}
