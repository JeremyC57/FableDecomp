// CClockDef::operator=(const CClockDef&) -- def copy: CDefClassBase part, sound map, hand type
// Leaf: ??4CClockDef
// Retail: Fable.exe (TLC, VC7.1) @ 0x004E4450, 39 bytes, size-optimised TU (/Os def region, like the landed
//         0x004663AE / 0x004E05AD def assignments).
// Callees: CDefClassBase::operator= @ 0x00431F10 (the manifest label "CThingBuildingDef::operator=" is propagated
//          over every def base assignment), std::map<CCharString,long>::operator= @ 0x00432B38.
// Identification: reached through the incremental-link thunk 0x004E444B (manifest "CClockDef::Copy", vtable 0x01242C80);
//   PDB CClockDef = CDefClassBase + Sound (std::map<CCharString,long>, 12 bytes in retail) +0x28, HandType +0x34.
// Shape: memberwise; the map goes through its out-of-line operator= (labelled "CTCCarryable_assign", propagated).
// The generated engine headers flatten the bases (no base operator= to call), so the classes are modelled locally.
#pragma optimize("s", on)

#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefClassBase.h"                       // retail def chain (0x25 packed, vtable 0x01230BCC)

struct CSoundNameMap {                    // std::map<CCharString, long> (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CSoundNameMap& operator=(const CSoundNameMap& other);   // 0x00432B38
};


// def vtable 0x01242C34: slot 0 0x004E4552 (implicit inline dtor); real overrides from the def vtable scan
class CClockDef : public CDefClassBase { public:
    CSoundNameMap Sound;                // +0x28
    long HandType;                      // +0x34
    CClockDef& operator=(const CClockDef& other);
    virtual void Transfer(CPersistContext& context);         // slot 18 0x004E448D
    virtual void Copy(const CDefClassBase* other);           // slot 19 0x004E444B
    virtual unsigned long GetSizeofClass();                  // slot 20 0x004E3828
};

CClockDef& CClockDef::operator=(const CClockDef& other)
{
    CDefClassBase::operator=(other);
    Sound = other.Sound;
    HandType = other.HandType;
    return *this;
}