// NUISystem::CUIStateDef::CUIStateDef(const CUIStateDef& other) -- copy constructor: the inlined CParentDefClassBase copy (CDefClassBase copy, InstantiationName, SubDefInfoMap vector + Dirty), own vtable, then the graphic index, state-change flag/type, position, zoom, colour, update time, the ChildrenNotAffected vector and the linear-change flag
// Leaf: ??0CUIStateDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006250B0, length 157 bytes (padded to 160), `ret 4`.
// Callees: CDefClassBase::CDefClassBase(const CDefClassBase&) @ 0x0046644C, the SubDefInfo vector copy ctor
// @ 0x0046EECF, vector<unsigned long> copy ctor @ 0x00535470.
// Vtables: 0x01230CBC (CParentDefClassBase, the inlined base's store) then 0x0125871C (NUISystem::CUIStateDef, see the
// landed dtor 0x00624DC0).
// Layout (retail; def classes are byte packed, CParentDefClassBase 0x39 bytes (the CVectorMap is 16 bytes default packed) then 4-aligned members, i.e. PDB - 4):
// InstantiationName +0x25, SubDefInfoMap +0x29 (Dirty +0x36), GraphicIndex +0x3c, StateChangeFlag +0x40,
// StateChangeType +0x44, Position +0x48, Zoom +0x50, Colour (CRGBFloatColour) +0x58, UpdateTime +0x68,
// ChildrenNotAffected +0x6c (12-byte vector), LinearChange +0x78.
#pragma pack(push, 1)
struct CDefClassBase {
    virtual void v00();
    unsigned char _body[0x21];                     // +0x04..+0x25 (CResource, PDefManager, GlobalIndex, flags)
    CDefClassBase(const CDefClassBase& other);     // 0x0046644C
};
#pragma pack(pop)
struct CDefString {
    long TablePos;
};
struct CSubDefInfoVector {                         // retail std::vector (12 bytes)
    void* m_first;
    void* m_last;
    void* m_end;
    CSubDefInfoVector(const CSubDefInfoVector& other);   // 0x0046EECF
};
struct CKeyPairCompareLess {
};
struct CSubDefInfoMap {                            // CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, ...>
    CSubDefInfoVector   Vector;                    // +0x00
    CKeyPairCompareLess Compare;                   // +0x0c
    bool                Dirty;                     // +0x0d
    CSubDefInfoMap(const CSubDefInfoMap& other)
        : Vector(other.Vector), Compare(other.Compare), Dirty(other.Dirty)
    {
    }
};
#pragma pack(push, 1)
struct CParentDefClassBase : CDefClassBase {       // vtable 0x01230CBC
    CDefString     InstantiationName;              // +0x25
    CSubDefInfoMap SubDefInfoMap;                  // +0x29
    virtual void v00();
    CParentDefClassBase(const CParentDefClassBase& other)
        : CDefClassBase(other), InstantiationName(other.InstantiationName), SubDefInfoMap(other.SubDefInfoMap)
    {
    }
};
#pragma pack(pop)
struct C2DVector { float X, Y; };
struct CRGBFloatColour { float R, G, B, A; };
struct CULongVector {                              // retail std::vector<unsigned long> (12 bytes)
    unsigned long* m_first;
    unsigned long* m_last;
    unsigned long* m_end;
    CULongVector(const CULongVector& other);       // 0x00535470
};
struct CUIStateDef : CParentDefClassBase {         // vtable 0x0125871C (NUISystem::CUIStateDef)
    unsigned long   GraphicIndex;                  // +0x3c
    unsigned long   StateChangeFlag;               // +0x40
    long            StateChangeType;               // +0x44 EStateChangeType
    C2DVector       Position;                      // +0x48
    C2DVector       Zoom;                          // +0x50
    CRGBFloatColour Colour;                        // +0x58
    float           UpdateTime;                    // +0x68
    CULongVector    ChildrenNotAffected;           // +0x6c
    bool            LinearChange;                  // +0x78
    virtual void v00();
    CUIStateDef(const CUIStateDef& other);
};
CUIStateDef::CUIStateDef(const CUIStateDef& other)
    : CParentDefClassBase(other), GraphicIndex(other.GraphicIndex), StateChangeFlag(other.StateChangeFlag),
      StateChangeType(other.StateChangeType), Position(other.Position), Zoom(other.Zoom), Colour(other.Colour),
      UpdateTime(other.UpdateTime), ChildrenNotAffected(other.ChildrenNotAffected), LinearChange(other.LinearChange)
{
}