// CHeightMap::`scalar deleting destructor'(unsigned int) -- destructor inlined: own vtable, WorldSeed cleared, the Cells vector's storage freed, then the base destructor; frees `this` when bit 0 of the flags is set
// Leaf: ??_GCHeightMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008F6DE0, length 61 bytes.
// Callees: free @ 0x00BFEA14 (the vector storage), the base destructor @ 0x0099A300 (out of line;
//          it only stores the root vtable 0x01231710), operator delete @ 0x00BFE9BC.
// Vtable 0x01288BEC = CHeightMap (rtti-vtables.tsv; this body is its slot 0).
// Layout (PDB CHeightMap): World +4, Map +8, WorldSeed (unsigned long*) +0xc, Cells
// (vector<CHeightMapCell>, 12 bytes retail) +0x10, then MapUpdateAreas and the size fields.
// Shape: `??_G` with the destructor inlined (`__forceinline virtual ~CHeightMap`): the vtable store,
// the body's `WorldSeed = 0` (an immediate store), then the vector member's inline destructor frees
// its block under a null test, then the base destructor. The `new` factory (inline constructor)
// pulls the vtable and the deleting destructor into the TU.

extern "C" void __cdecl free(void* block);                                // 0x00BFEA14

struct CBaseClass {
    virtual ~CBaseClass();                                                // 0x0099A300
};

struct CWorld;
struct CMap;

struct CHeightMapCell {
    unsigned char _cell[0x10];
};

struct CHeightMapCellVector {                                             // retail vector<CHeightMapCell> (12 bytes)
    CHeightMapCell* m_first;
    CHeightMapCell* m_last;
    CHeightMapCell* m_end;
    CHeightMapCellVector() : m_first(0), m_last(0), m_end(0) {}
    ~CHeightMapCellVector()
    {
        if (m_first)
            free(m_first);
    }
};

struct CHeightMap : CBaseClass {                                          // vtable 0x01288BEC
    CWorld*              World;                                           // +0x04
    CMap*                Map;                                             // +0x08
    unsigned long*       WorldSeed;                                       // +0x0c
    CHeightMapCellVector Cells;                                           // +0x10
    CHeightMap(CWorld* world, CMap* map) : World(world), Map(map), WorldSeed(0) {}
    __forceinline virtual ~CHeightMap()
    {
        WorldSeed = 0;
    }
};

CHeightMap* NewHeightMap(CWorld* world, CMap* map)
{
    return new CHeightMap(world, map);
}
