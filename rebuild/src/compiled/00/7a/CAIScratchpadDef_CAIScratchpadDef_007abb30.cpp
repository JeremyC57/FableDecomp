// CAIScratchpadDef::CAIScratchpadDef() -- inlined def base over CResource (a long and a 3-bit flag field cleared), own vtable, four CCharString members
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007ABB30, length 71 bytes.
// Callees: CResource::CResource @ 0x009FBEC0, CCharString::CCharString() @ 0x0099E4B0 (x4).
// This class's vtable is 0x0126D014 (rtti-vtables.tsv: CAIScratchpadDef).
// Layout from engine/CAIScratchpadDef.h: Action +0x64, TargetName +0x6c, TableName +0x8c,
// SeatName +0x90 are the CCharString members built here; everything else is left for the
// loader. The dword at +0x1c and the low three bits of the byte at +0x24 are cleared BEFORE the
// vtable store, so they belong to an inlined intermediate definition base (abstract,
// `__declspec(novtable)`) sitting between CResource (+0..+0x1c) and this class's members at
// +0x28. The flag clear is a read/and/write of the byte through al (a 3-bit bitfield set to 0).
#include "fable_string.h"

class CResource { public:
    virtual void v00();
    unsigned char _body[0x18];              // +0x04..+0x1c
    CResource();                            // 0x009FBEC0
};

struct __declspec(novtable) CDefinitionBase : CResource {   // abstract: no vtable store of its own
    long          m_defIndex;               // +0x1c
    long          m_reserved;               // +0x20 (not initialised here)
    unsigned char m_flags : 3;              // +0x24 low three bits
    CDefinitionBase() : CResource(), m_defIndex(0), m_flags(0) { }
};

class CAIScratchpadDef : public CDefinitionBase { public:  // vtable 0x0126D014
    unsigned char _pad0[0x3c];              // +0x28..+0x64 (phrases, wander centre, counters ...)
    CCharString   Action;                   // +0x64
    bool          FaceHeroForAction;        // +0x68
    CCharString   TargetName;               // +0x6c
    unsigned char _pad1[0x1c];              // +0x70..+0x8c
    CCharString   TableName;                // +0x8c
    CCharString   SeatName;                 // +0x90
    virtual void v00();
    CAIScratchpadDef();
};

CAIScratchpadDef::CAIScratchpadDef()
    : CDefinitionBase()
    , Action()
    , TargetName()
    , TableName()
    , SeatName()
{
}