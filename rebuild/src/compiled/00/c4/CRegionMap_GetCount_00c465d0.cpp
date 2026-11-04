// CRegionMap::GetCount() const -- the number of regions in the map: forward to the region list's (+0x04) slot-15 GetCount
// Leaf: ?GetCount@CRegionMap@@QBEJXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C465D0, 8 bytes: `add ecx,4 ; mov eax,[ecx] ; jmp [eax+3Ch]` (no-argument virtual call on the
//   member list; the sibling ForEach 0x00C465B0, which forwards an argument, loads the vptr from [ecx+4] first).
// Identity (two sources): the landed CRegionSnapshot ctor 0x00C46400 calls it on its `const CRegionMap&` between GetCellSize
//   0x00C465E0 and ForEach 0x00C465B0 (the landed CRegionMap rows, whose model this is: allocator +0x00, region list +0x04, cell size
//   +0x34); and 0x00C36CF0 builds a stack CRegionMap with the same ctor 0x00C467D0 / dtor 0x00C46860, calls this and returns its
//   result. That caller's model names the class `CAliveHelper_00c36cf0` and this `Check` -- one address, one name: the right spelling
//   is CRegionMap::GetCount (NAME_FIXES.tsv). The catalog label `CIsThingAlive::operator()` is propagated.
// Library: libpk (no PDB / RTTI): descriptive names.
class CNodeAllocator {                          // vtable 0x012B2F60
public:
    virtual void Free(void* node);              // slot 0 (0x00C19640)
    virtual void* Alloc();                      // slot 1 (0x00C19650)
};

class CRegionVisitor;

class CRegionList {                             // the region node list, vtable 0x012B7DF8 (slots declared, not pure: a member cannot be abstract)
public:
    virtual long Compare(const void* a, const void* b) const;       // slot 0
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual ~CRegionList() { }                  // slot 4
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void Remove(void* item);            // slot 8
    virtual void Add(void* item);               // slot 9
    virtual void ForEach(CRegionVisitor& visitor) const;       // slot 10 (+0x28)
    virtual void* GetHead();                    // slot 11
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual long GetCount() const;              // slot 15 (+0x3c)
    void*         m_pImpl;                      // +0x04 (-> m_impl)
    unsigned char m_impl[0x28];                 // +0x08 the list implementation (destructor 0x00C19250)
};

class CRegionMap {                              // libpk ambient region map (ctor 0x00C467D0, dtor 0x00C46860)
public:
    CNodeAllocator m_allocator;                 // +0x00
    CRegionList    m_regions;                   // +0x04
    float          m_cellSize;                  // +0x34
    float          m_radius;                    // +0x38
    long GetCount() const;
};

long CRegionMap::GetCount() const
{
    return m_regions.GetCount();
}