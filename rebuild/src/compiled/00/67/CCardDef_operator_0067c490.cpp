// CBlurredAttitudesDef::operator=(const CBlurredAttitudesDef& other) -- base assignment, then the id, the vector and flag block, the flag byte and the Attitudes vector
// Leaf: ??4CBlurredAttitudesDef@@QAEAAV0@ABV0@@Z
// Name fix (NF382): was CCardDef::operator=. Its only retail caller 0x0067D450 is the 0x48-stride vector operator= that the landed
//   COpinionReactionManagerDef::operator= 0x0067CE50 calls on its first member, and FableWin COpinionReactionManagerDef::operator=
//   calls vector<CBlurredAttitudesDef>::operator= there (opinion_reaction_manager_def.obj). The landed CBlurredAttitudesDef::Copy
//   0x0067C980 expands this same member-wise copy (base 0x00431F10, +0x25, the 0x00431F90 vector, the flag, 0x00471E44 on +0x3C).
//   CCardDef (vtable 0x01243A44) ends with two longs (Ego_r CardName / CardVal) and has no member with its own assignment.
//   The callee stand-ins keep their landed spellings. Same bytes.
// Pick-any form (NF387, T3 nf382 0x0067C490): the operator= is defined in the class, so it is an inline member and its COMDAT is
//   pick-any (Any), like the copies the other landed def TUs emit; HEAD's out-of-line definition was NoDuplicates and clashed with
//   them at link (NoDuplicates v Any counts as a duplicate). Same body, same bytes.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C490, length 63 bytes (+ int3 padding).
// Callees: CBase_00431f10::operator= @ 0x00431F10 (the def base; manifest label CThingBuildingDef::operator=,
// propagated), retail vector<long>::operator= @ 0x00431F90 on the packed member at +0x29,
// CSubObject_00471e44::operator= @ 0x00471E44 on the member at +0x3c.
// Layout (packed; the offsets were first read from the constructor @ 0x007D7A00 of CCardDef, vtable 0x01243A44, which shares
// the def-chain prefix; the +0x3c member is the 12-byte STLport Attitudes vector, Ego_r CBlurredAttitudesDef):
// +0x25 long, +0x29 12-byte vector, +0x36 byte, +0x3c sub-object with its own operator=.
// Shape: memberwise assignment -- base operator= out of line, members in declaration order, `this`
// returned. Retail keeps `&other+0x29` in ebx and `this+0x29` in ebp and copies the flag byte at
// +0x0d FROM THOSE registers: the vector and the flag are one sub-object whose inline operator=
// is expanded here (a flat member list loads the byte from [edi+36h] instead).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)
#pragma pack(push, 1)

struct CRetailVector_long {                     // VC6-era Dinkumware vector: first / last / end
    long* first;
    long* last;
    long* end;
    CRetailVector_long& operator=(const CRetailVector_long& other);   // 0x00431F90
};

struct SAttitudeBlock {                        // +0x29..+0x36: the vector and its flag share one sub-object
    CRetailVector_long m_attitudes;             // +0x00 (this+0x29)
    unsigned char      m_pad0c;                 // +0x0c (not copied)
    unsigned char      m_flag;                  // +0x0d (this+0x36)
    SAttitudeBlock& operator=(const SAttitudeBlock& other)
    {
        m_attitudes = other.m_attitudes;
        m_flag = other.m_flag;
        return *this;
    }
};

struct CSubObject_00471e44 {
    unsigned char _body[0x0c];
    CSubObject_00471e44& operator=(const CSubObject_00471e44& other);   // 0x00471E44
};

class CBlurredAttitudesDef : public CDefClassBase { public:
    long                m_id;                   // +0x25
    SAttitudeBlock      m_block;                // +0x29 (vector + flag)
    unsigned char       m_pad37[5];             // +0x37..+0x3b
    CSubObject_00471e44 m_tail;                 // +0x3c (Attitudes)
    CBlurredAttitudesDef& operator=(const CBlurredAttitudesDef& other)   // class-inline: VC7.1 emits it as a pick-any COMDAT (selection Any)
    {
        CDefClassBase::operator=(other);
        m_id = other.m_id;
        m_block = other.m_block;
        m_tail = other.m_tail;
        return *this;
    }
    virtual void Transfer(CPersistContext& context);
    virtual void Copy(const CDefClassBase* other);
    virtual unsigned long GetSizeofClass();
};
#pragma pack(pop)

namespace {
// Fidelity: scaffolding, not retail: the address of the class-inline operator=, so this TU emits it out of line as a pick-any COMDAT, as
// retail's own TUs do (the landed CBlurredAttitudesDef::Copy 0x0067C980 and three more TUs carry the same Any copy). Internal to this TU.
typedef CBlurredAttitudesDef& (CBlurredAttitudesDef::*TBlurredAttitudesAssign)(const CBlurredAttitudesDef&);
TBlurredAttitudesAssign ReferenceBlurredAttitudesAssign()
{
    return &CBlurredAttitudesDef::operator=;
}
}