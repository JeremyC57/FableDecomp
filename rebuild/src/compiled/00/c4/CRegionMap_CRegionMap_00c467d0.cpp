// CRegionMap::CRegionMap(float radius, float cellSize) -- build the ambient region map: the node allocator (+0x00) and the region list over it (+0x04, out of line), the cell size (+0x34) and radius (+0x38), then walk every grid cell within radius / cellSize with a stack collector that interns the cells' regions into the list
// Leaf: ??0CRegionMap@@QAE@MM@Z
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C467D0, 136 bytes, __thiscall `ret 8`, returns this, EH-framed (state 0 = the region list).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439 "0xC467D0 CGuiVarTransferFloat = CRegionMap ctor"): its caller 0x00C37050
//   builds a stack object with it (0x00C370CF), hands that object to the landed CRegionSnapshot constructor 0x00C46400
//   (`const CRegionMap&`) and destroys it with the landed CRegionMap::~CRegionMap 0x00C46860 (0x00C37112); the second caller
//   0x00C36D16 is the same pattern. The layout is the one that destructor and agent439's GetCellSize 0x00C465E0 (+0x34) read.
//   Descriptive names (no PDB covers libpk).
// Label correction: the landed row was CGuiVarTransferFloat::CGuiVarTransferFloat(float, CGuiFloat) (propagated), which modelled the
//   allocator's vtable as the class's own, the list as a "transfer base" and the CGridCircle walker temporary as a `Bind` call
//   on a by-value wrapper argument. VC7.1 builds the walker temporary in the dead `cellSize` argument slot by itself (`lea ecx,[esp+34h]`).
// Naming: the region-list type is named descriptively here; agent439's 0x00C46860 calls it PKCTRList<SRegionNode>, but its vtable
//   0x012B7DF8 holds none of the libpk PKCTRList.h asserts (see NOTES.md).
// Callees: CRegionList::CRegionList 0x00C46CE0 (with the allocator, i.e. `this`), CGridCircle::CGridCircle 0x00C46900 (radius /
//   cellSize, &collector); the inline allocator and collector constructors store vtables 0x012B2F60 and 0x012B7D64 (the abstract
//   CCellVisitor base's own store is dead and dropped).
struct C2DVectorI {
    long X;
    long Y;
};

class CCellVisitor {                            // the grid walker's cell callback (agent439's 0x00C46900 model)
public:
    virtual void Visit(const C2DVectorI& cell) = 0;     // slot 0
};

class CGridCircle {
public:
    CGridCircle(float radius, CCellVisitor* visitor);   // 0x00C46900 (visits every cell within the radius)
};

class CNodeAllocator {                          // vtable 0x012B2F60 (agent439: a member, not a base)
public:
    virtual void Free(void* node);              // slot 0 (0x00C19640)
    virtual void* Alloc();                      // slot 1 (0x00C19650, `new 14h`)
};

class CRegionList {                             // the region node list, vtable 0x012B7DF8 (0x30 bytes)
public:
    CRegionList(CNodeAllocator* allocator);     // 0x00C46CE0
    ~CRegionList();                             // 0x00C46D30 (the EH state-0 unwind)
    void*         m_vptr;                       // +0x00
    void*         m_pImpl;                      // +0x04 (-> m_impl)
    unsigned char m_impl[0x28];                 // +0x08
};

class CRegionCollector : public CCellVisitor {  // the 12-byte stack collector, vtable 0x012B7D64
public:
    CRegionCollector(CRegionList* regions, float cellSize) : m_pRegions(regions), m_cellSize(cellSize) { }
    virtual void Visit(const C2DVectorI& cell); // slot 0 (0x00C466F0: interns the four corners of each cell)
    CRegionList* m_pRegions;
    float        m_cellSize;
};

class CRegionMap {
public:
    CNodeAllocator m_allocator;                 // +0x00
    CRegionList    m_regions;                   // +0x04
    float          m_cellSize;                  // +0x34
    float          m_radius;                    // +0x38
    CRegionMap(float radius, float cellSize);
};

CRegionMap::CRegionMap(float radius, float cellSize)
    : m_regions(&m_allocator)
{
    m_cellSize = cellSize;
    m_radius = radius;
    CRegionCollector collector(&m_regions, cellSize);
    CGridCircle(radius / cellSize, &collector);
}
