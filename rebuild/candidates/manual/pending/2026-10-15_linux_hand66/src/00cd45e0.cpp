// SThreeStrings_00cd45e0::SThreeStrings_00cd45e0() -- default-constructs three consecutive CCharString members (+0x0, +0x4, +0x8)
// Leaf: ??0SThreeStrings_00cd45e0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CD45E0, 28 bytes (+4 int3 padding).
// Callee: 0x0099E4B0 CCharString::CCharString().
//
// The manifest label (a CPersistContextReadSection constructor) is propagated and wrong: the PDB
// CPersistContextReadSection starts with a 44-byte CPersistContext, while this body only
// default-constructs three CCharString members at +0x0/+0x4/+0x8 and returns `this`. The PDB has
// several records of that shape (WaypointData {locMarker, locTextOverheard, locTextRequested},
// CThingInfo {TypeName, SubTypeName, ScriptName, long Player}, CActivity {Name, Description,
// UnitOfMeasurement, ...}); the script-module neighbourhood (QuestData constructor at 0x00CD45A0)
// does not single one out and the constructor has no direct callers, so the record keeps a
// descriptive name.
struct CCharString {
    struct SBlock* m_pBlock;
    CCharString();                         // 0x0099E4B0
};

struct SThreeStrings_00cd45e0 {
    CCharString m_first;                   // +0x0
    CCharString m_second;                  // +0x4
    CCharString m_third;                   // +0x8
    SThreeStrings_00cd45e0();
};

SThreeStrings_00cd45e0::SThreeStrings_00cd45e0()
{
}
