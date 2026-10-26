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

class CDefClassBase { public:
    unsigned char _body[0x25];          // vptr, resource links, PDefManager +0x1c, GlobalIndex +0x20, flag bits +0x24
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};

struct CSoundNameMap {                    // std::map<CCharString, long> (retail STLport _Rb_tree, 12 bytes)
    void* m_pHeader;
    unsigned long m_nodeCount;
    unsigned char m_keyCompare;
    CSoundNameMap& operator=(const CSoundNameMap& other);   // 0x00432B38
};


class CClockDef : public CDefClassBase { public:
    CSoundNameMap Sound;                // +0x28
    long HandType;                      // +0x34
    CClockDef& operator=(const CClockDef& other);
};

CClockDef& CClockDef::operator=(const CClockDef& other)
{
    CDefClassBase::operator=(other);
    Sound = other.Sound;
    HandType = other.HandType;
    return *this;
}