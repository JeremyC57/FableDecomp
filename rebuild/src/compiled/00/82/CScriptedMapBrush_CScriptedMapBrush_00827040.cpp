// CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrush& other) -- copy constructor: CBaseObject base, the cell vector, the three thing sets/map, the game-area box, the brush name, creation time and pattern
// Leaf: ??0CScriptedMapBrush
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00827040, length 128 bytes (123 + int3 padding).
// Callees: CBaseObject::CBaseObject(const CBaseObject&) @ 0x0099A390, the Cells vector copy ctor @ 0x00824DD0,
// the set<unsigned __int64> copy ctor @ 0x00826000 (ThingsToDelete, ThingsToAdd), the ThingsToMove map
// copy ctor @ 0x008267C0, CCharString copy ctor @ 0x0099EC30.
// Vtable: 0x01272518 (CScriptedMapBrush, rtti-vtables.tsv). Callers: 0x00820FB0, 0x00827315 .. 0x0082780C.
// Shape: memberwise copy in declaration order; the C2DBoxI game area copies as four direct dword moves
// through its inline memberwise copy ctor (the implicit copy re-bases both pointers with `lea`).
// Layout: engine/CScriptedMapBrush.h is the PDB (debug-STL) layout, 0x64 bytes with 16-byte containers;
// retail's release STLport containers are 12 bytes, so every member after Cells sits 4/8/12/16 bytes lower
// (retail: Cells +0x08, ThingsToDelete +0x14, ThingsToMove +0x20, ThingsToAdd +0x2c, GameArea +0x38,
// BrushName +0x48, CreationTimeSeconds +0x4c, CreationPattern +0x50; sizeof 0x54). Modelled locally with
// the header's member names.
class CBaseObject { public:
    void* IntelligentPointer;                         // +0x04
protected:
    CBaseObject(const CBaseObject& other);               // 0x0099A390
    virtual ~CBaseObject();                              // 0x0099A430
};

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);            // 0x0099EC30
};

struct CScriptedMapCellVector {                       // vector<CScriptedMapBrush::CScriptedMapCell>
    void* m_start; void* m_finish; void* m_end_of_storage;
    CScriptedMapCellVector(const CScriptedMapCellVector& other);   // 0x00824DD0
};
struct CThingIDSet {                                  // set<unsigned __int64>
    void* m_header; void* m_node; unsigned long m_count;
    CThingIDSet(const CThingIDSet& other);            // 0x00826000
};
struct CThingPosMap {                                 // map<unsigned __int64, CThingPosAndOrientation>
    void* m_header; void* m_node; unsigned long m_count;
    CThingPosMap(const CThingPosMap& other);          // 0x008267C0
};
class C2DBoxI { public:
    long TLX, TLY, BRX, BRY;
    C2DBoxI(const C2DBoxI& o) : TLX(o.TLX), TLY(o.TLY), BRX(o.BRX), BRY(o.BRY) {}
};

class CScriptedMapBrush : public CBaseObject { public:             // vtable 0x01272518
    CScriptedMapCellVector Cells;                     // +0x08
    CThingIDSet            ThingsToDelete;            // +0x14
    CThingPosMap           ThingsToMove;              // +0x20
    CThingIDSet            ThingsToAdd;               // +0x2c
    C2DBoxI                GameArea;                  // +0x38
    CCharString            BrushName;                 // +0x48
    float                  CreationTimeSeconds;       // +0x4c
    long                   CreationPattern;           // +0x50
    virtual void v00();
    CScriptedMapBrush(const CScriptedMapBrush& other);
};

CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrush& other)
    : CBaseObject(other), Cells(other.Cells), ThingsToDelete(other.ThingsToDelete), ThingsToMove(other.ThingsToMove),
      ThingsToAdd(other.ThingsToAdd), GameArea(other.GameArea), BrushName(other.BrushName),
      CreationTimeSeconds(other.CreationTimeSeconds), CreationPattern(other.CreationPattern)
{
}