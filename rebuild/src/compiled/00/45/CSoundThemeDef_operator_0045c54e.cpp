// CSoundThemeDef::operator=(const CSoundThemeDef&) -- memberwise copy: CParentDefClassBase part out of line, six dwords, the atmos-group multimap by its operator=, four flag bytes; returns *this
#pragma optimize("s", on)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0045C54E, length 93 bytes.
// Pragma: #pragma optimize("s", on) -- the def module is size-optimised (every copy goes through eax).
// Callees: CParentDefClassBase::operator= @ 0x00431ED0, multimap<CCharString,long>::operator= @ 0x00432B38
// (manifest "CTCCarryable_assign", propagated).
// Identity: the catalog label "CParentDefClassBase::operator=" is propagated. CSoundThemeDef's vtable
// (0x01233824) slot 19 (Copy) is the 5-byte `jmp 0x0045C54E` right before this body, and the PDB layout
// of CSoundThemeDef (EditColour, AtmosIndex, MusicSet, Gain, Range, Granularity, AtmosGroupMap, four
// bools) sits exactly at the copied offsets once the retail 4-byte-smaller containers are applied
// (PDB +0x40.. -> retail +0x3c..; the PDB multimap is 16 bytes, retail 12).
// The engine header CParentDefClassBase.h is the PDB layout (0x3d bytes, 16-byte SubDefInfoMap); the
// retail base is 0x39 bytes (12-byte vector map), so it is modelled locally: a packed polymorphic base
// whose operator= is out of line. Members follow at the natural 4-byte boundary +0x3c.
#pragma pack(push, 1)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _defClassBase[0x35];      // +0x04 CDefClassBase part, InstantiationName (+0x25), SubDefInfoMap (+0x29)
    CParentDefClassBase& operator=(const CParentDefClassBase& other);   // 0x00431ED0
};
#pragma pack(pop)

struct CRGBColour {                         // B, G, R, A bytes; 4-byte aligned in retail (the PDB puts
    unsigned long m_packed;                 // EditColour at +0x40, not right after the 0x3d-byte base)
};

struct CAtmosGroupMultimap {                // retail multimap<CCharString,long> (12 bytes)
    void* m_allocator;
    void* m_head;
    unsigned long m_size;
    CAtmosGroupMultimap& operator=(const CAtmosGroupMultimap& other);   // 0x00432B38
};

struct CSoundThemeDef : CParentDefClassBase {   // vtable 0x01233824
    CRGBColour          EditColour;             // +0x3c
    long                AtmosIndex;             // +0x40
    long                MusicSet;               // +0x44
    float               Gain;                   // +0x48
    float               Range;                  // +0x4c
    float               Granularity;            // +0x50
    CAtmosGroupMultimap AtmosGroupMap;          // +0x54
    bool                PlayOnlyWhenInside;     // +0x60
    bool                PlayQuietlyWhenInside;  // +0x61
    bool                PlayOnlyWhenOutside;    // +0x62
    bool                PlayQuietlyWhenOutside; // +0x63
    CSoundThemeDef& operator=(const CSoundThemeDef& other);
};

CSoundThemeDef& CSoundThemeDef::operator=(const CSoundThemeDef& other)
{
    CParentDefClassBase::operator=(other);
    EditColour = other.EditColour;
    AtmosIndex = other.AtmosIndex;
    MusicSet = other.MusicSet;
    Gain = other.Gain;
    Range = other.Range;
    Granularity = other.Granularity;
    AtmosGroupMap = other.AtmosGroupMap;
    PlayOnlyWhenInside = other.PlayOnlyWhenInside;
    PlayQuietlyWhenInside = other.PlayQuietlyWhenInside;
    PlayOnlyWhenOutside = other.PlayOnlyWhenOutside;
    PlayQuietlyWhenOutside = other.PlayQuietlyWhenOutside;
    return *this;
}