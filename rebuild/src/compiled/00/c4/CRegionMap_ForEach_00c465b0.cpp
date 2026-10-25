// CRegionMap::ForEach(CRegionVisitor& visitor) const -- run a visitor over every region node: forward to the region list's (+0x04) slot-10 ForEach
// Leaf: ?ForEach@CRegionMap@@QBEXAAVCRegionVisitor@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C465B0, 9 bytes: `mov eax,[ecx+4] ; add ecx,4 ; jmp [eax+28h]` (the visitor
//   argument stays in place for the sibling call).
// Identity (sweep1, 2026-10-25; FINDINGS chunk 439): `this` is agent439's libpk CRegionMap (allocator +0x00, the
//   region list +0x04 (agent439's PKCTRList<SRegionNode>; see NOTES.md), cell size +0x34, radius +0x38; ctor 0x00C467D0, dtor 0x00C46860). The landed
//   CRegionSnapshot constructor 0x00C46400 calls GetCellSize 0x00C465E0, GetCount 0x00C465D0 and ForEach 0x00C465B0 on its
//   `const CRegionMap&` (rebuild/oracles/reloc_bindings.tsv binds ?GetCellSize@CRegionMap@@QBEMXZ -> 0x00C465E0 and
//   ?ForEach@CRegionMap@@QBEXAAVCRegionVisitor@@@Z -> 0x00C465B0, the symbols defined here). Descriptive names (no PDB).
// Label correction: the landed row was NScript::CIsThingAlive::operator() (propagated manifest label) over a placeholder
//   `C::Op` with a slot-10 call on an invented member.
class CNodeAllocator {                          // vtable 0x012B2F60 (agent439's 0x00C46860 model)
public:
    virtual void Free(void* node);              // slot 0 (0x00C19640)
    virtual void* Alloc();                      // slot 1 (0x00C19650)
};

class CRegionVisitor;                           // the region visitor (CRegionCopier 0x012B7D58 derives from it)

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

struct SRegionNode {                            // 16 bytes (MarkRegion 0x00C465F0)
    long  m_visited;                            // +0x00
    long  m_x;                                  // +0x04
    long  m_y;                                  // +0x08
    float m_length;                             // +0x0c
};

class CRegionMap {                              // libpk ambient region map (agent439: ctor 0x00C467D0, dtor 0x00C46860)
public:
    CNodeAllocator         m_allocator;         // +0x00
    CRegionList            m_regions;           // +0x04
    float                  m_cellSize;          // +0x34
    float                  m_radius;            // +0x38
    void ForEach(CRegionVisitor& visitor) const;
};

void CRegionMap::ForEach(CRegionVisitor& visitor) const
{
    m_regions.ForEach(visitor);
}
