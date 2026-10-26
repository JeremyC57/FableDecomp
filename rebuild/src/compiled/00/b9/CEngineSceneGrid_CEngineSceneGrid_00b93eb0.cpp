// CEngineSceneGrid::CEngineSceneGrid(unsigned long level) -- CBase, the grid level at +4, own vtable, no neighbour levels, the side length 1 << level and a cell-pointer array resized to Size * Size nulls
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B93EB0, length 80 bytes, `ret 4`.
// Callees: CBase::CBase @ 0x0099A2F0, vector<CEngineSceneGridCell*>::resize(unsigned int n, CEngineSceneGridCell* const& value)
// @ 0x00B97A80 (STLport: _M_start +0, _M_finish +4; shrinks with memmove, grows through insert @ 0x00B97250).
// Vtable 0x012A3E58 = CEngineSceneGrid (rtti-vtables.tsv); the chunk label "CV_SingingStonesScript::CV_SingingStonesScript"
// is propagated. Layout (Ego_r.pdb, 36 bytes): ulong Level +4, CEngineSceneGrid* LevelUp +8, LevelDown +0xc, ulong Size +0x10,
// CArray<CEngineSceneGridCell*> GridCells +0x14 (16 bytes in the PDB; only its three STLport vector words are touched here).
// Shape: `Size` is UNSIGNED (a signed side keeps the shl result in eax and the cell vector's `lea` in edx); the member is read
// back for `Size * Size` without a reload (`mov ecx,edx ; imul ecx,edx`); the value temporary 0 for resize homed in the dead
// `level` argument slot (`lea ecx,[esp+0Ch]` pushed, `mov [esp+14h],edi` after the other push); the vector's three words are
// zeroed by the STLport-shaped inline constructor chain (vector(const allocator& = allocator()) -> _Vector_base(a) ->
// _M_end_of_storage(a, 0) as an allocator-derived proxy), which is what places the count push between the [eax+4] and [eax+8]
// stores -- a flat three-member constructor pushes the count before all three; `Size` stored last, after `mov ecx,eax`.
struct CBase {
    virtual void v00();
    CBase();                                       // 0x0099A2F0
};

class CEngineSceneGridCell;
struct CGridCellAllocator { };                      // STLport allocator<CEngineSceneGridCell*> (empty)
struct CGridCellStorageProxy : CGridCellAllocator { // STLport _STLP_alloc_proxy: allocator base + the end-of-storage pointer
    CEngineSceneGridCell** _M_data;
    CGridCellStorageProxy(const CGridCellAllocator& a, CEngineSceneGridCell** p) : CGridCellAllocator(a), _M_data(p) { }
};
struct CGridCellVectorBase {                        // STLport _Vector_base
    CEngineSceneGridCell** _M_start;                // +0x00
    CEngineSceneGridCell** _M_finish;               // +0x04
    CGridCellStorageProxy  _M_end_of_storage;       // +0x08
    CGridCellVectorBase(const CGridCellAllocator& a) : _M_start(0), _M_finish(0), _M_end_of_storage(a, 0) { }
};
struct CGridCellArray : CGridCellVectorBase {       // CArray<CEngineSceneGridCell*>
    CGridCellArray(const CGridCellAllocator& a = CGridCellAllocator()) : CGridCellVectorBase(a) { }
    void resize(unsigned int n, CEngineSceneGridCell* const& value);   // 0x00B97A80
};

class CEngineSceneGrid : public CBase { public:                   // vtable 0x012A3E58
    unsigned long     Level;                        // +0x04
    CEngineSceneGrid* LevelUp;                      // +0x08
    CEngineSceneGrid* LevelDown;                    // +0x0c
    unsigned long     Size;                         // +0x10
    CGridCellArray    GridCells;                    // +0x14
    virtual void v00();
    CEngineSceneGrid(unsigned long level);
};

CEngineSceneGrid::CEngineSceneGrid(unsigned long level)
    : CBase(), Level(level), LevelUp(0), LevelDown(0), GridCells()
{
    Size = 1 << level;
    GridCells.resize(Size * Size, 0);
}