// CCardDef::operator=(const CCardDef& other) -- base assignment, then the id, the attitude vector, the flag byte and the trailing sub-object
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0067C490, length 63 bytes (+ int3 padding).
// Callees: CBase_00431f10::operator= @ 0x00431F10 (the def base; manifest label CThingBuildingDef::operator=,
// propagated), retail vector<long>::operator= @ 0x00431F90 on the packed member at +0x29,
// CSubObject_00471e44::operator= @ 0x00471E44 on the member at +0x3c.
// Layout (packed, from the constructor @ 0x007D7A00 of the class whose vtable 0x01243A44 is
// CCardDef in rtti-vtables.tsv; the manifest label CBlurredAttitudesDef is propagated):
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

// def vtable 0x01243A44: slot 0 0x004EC0D4 (implicit inline dtor); real overrides from the def vtable scan
class CCardDef : public CDefClassBase { public:
    long                m_id;                   // +0x25
    SAttitudeBlock      m_block;                // +0x29 (vector + flag)
    unsigned char       m_pad37[5];             // +0x37..+0x3b
    CSubObject_00471e44 m_tail;                 // +0x3c
    CCardDef& operator=(const CCardDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004EC0B3
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004EC0F0
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004EC0AF
};
#pragma pack(pop)

CCardDef& CCardDef::operator=(const CCardDef& other)
{
    CDefClassBase::operator=(other);
    m_id = other.m_id;
    m_block = other.m_block;
    m_tail = other.m_tail;
    return *this;
}