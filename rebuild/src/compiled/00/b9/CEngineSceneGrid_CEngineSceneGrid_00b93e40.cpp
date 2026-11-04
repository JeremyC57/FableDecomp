// CEngineSceneGrid::~CEngineSceneGrid() -- scene grid destructor: restore the CEngineSceneGrid vtable, delete every grid cell in GridCells (by index, re-reading the size each pass), then the GridCells array's own destructor (free the buffer) and the CBaseClassNonCopyable base
// Leaf: ??1CEngineSceneGrid@@UAE@XZ
// Plan phase 3 (2026-11-04), CArray wave: the local CArray (an empty public std::vector<T>) and its STLport stand-ins replaced by the shared engine/CArray.h (Lionhead's array: a public STLport 4.6 vector<T>, 12 bytes, no fields of its own). STLport algorithm / construct helpers and placement new come from the header's model (<new>).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B93E40, 98 bytes, thiscall, tail `jmp ??1CBaseClassNonCopyable@@MAE@XZ` (0x0099A300).
// Identity (proven): stores vtable 0x012A3E58 first (RTTI .?AVCEngineSceneGrid@@ : CBaseClassNonCopyable : CBaseClass); sole
//   caller is that vtable's slot-0 scalar deleting destructor 0x00B97A60 (call at 0x00B97A63); Ego_r.pdb ??1CEngineSceneGrid@@UAE@XZ
//   and ??1CEngineSceneGridCell@@QAE@XZ (the per-cell callee 0x00B93660, non-virtual). The catalog label
//   CLocalDetailGeneratorTheme::~CLocalDetailGeneratorTheme is propagated from a masked twin.
// Callees: ~CEngineSceneGridCell 0x00B93660, operator delete 0x00BFE9BC, free 0x00BFEA14 (STLport allocator), ~CBaseClassNonCopyable 0x0099A300.
// Layout: Ego_r CEngineSceneGrid (no engine header): Level +0x04, LevelUp +0x08, LevelDown +0x0C, Size +0x10, GridCells +0x14
//   (CArray<CEngineSceneGridCell*>, Fable's CArray over the 12-byte STLport 4.6 vector: _M_start +0x14, _M_finish +0x18).
// Shape: `for (unsigned long i = 0; i < GridCells.size(); ++i) delete GridCells[i];` -- unsigned size (`jbe` / `jb`), the
//   size re-read every pass, operator[] re-loads _M_start; `delete` of the non-virtual cell is the null test + ??1 + operator
//   delete; then the STLport ~vector (pointer elements: no destroy loop) is `if (_M_start) free`.
#include "engine/CBaseClassNonCopyable.h"
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
#include "engine/CArray.h"
class CEngineSceneGridCell {
public:
    ~CEngineSceneGridCell();                               // 0x00B93660 (Ego_r ??1CEngineSceneGridCell@@QAE@XZ)
};
class CEngineSceneGrid : public CBaseClassNonCopyable {
public:
    unsigned long                 Level;                   // +0x04
    CEngineSceneGrid*             LevelUp;                 // +0x08
    CEngineSceneGrid*             LevelDown;               // +0x0c
    unsigned long                 Size;                    // +0x10
    CArray<CEngineSceneGridCell*> GridCells;               // +0x14
    virtual ~CEngineSceneGrid();
};
CEngineSceneGrid::~CEngineSceneGrid()
{
    for (unsigned long i = 0; i < GridCells.size(); ++i)
        delete GridCells[i];
}