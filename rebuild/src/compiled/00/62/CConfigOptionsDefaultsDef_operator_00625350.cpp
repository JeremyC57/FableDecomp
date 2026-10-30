// CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef& other) -- def assignment: the CParentDefClassBase part out of line, then seven words, a 16-byte record, a word, a member with its own assignment and a flag byte
// Leaf: ??4CConfigOptionsDefaultsDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x00625350, 115 bytes (128 with padding).
// Callees: CParentDefClassBase::operator= 0x00431ED0, the +0x6c member's operator= 0x0045BC09 (manifest
//          label CActiveFile::operator=, propagated).
//
// Shape: the Ego_r layout of CConfigOptionsDefaultsDef (floats from +0x58) does not fit these bytes, so
// the def is modelled by the copy it performs (the manifest label may be propagated over an identical
// body). The base is 0x3c bytes. GOTCHAS wave 23d: the DDK out-lines an implicit operator= of a record
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

class CConfigOptionsDefaultsDef : public CParentDefClassBase { public:
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
    CConfigOptionsDefaultsDef& operator=(const CConfigOptionsDefaultsDef& other);
};

CConfigOptionsDefaultsDef& CConfigOptionsDefaultsDef::operator=(const CConfigOptionsDefaultsDef& other)
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