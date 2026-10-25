// CRegionMap::~CRegionMap() -- free the region map: pop every region node off its region list (+0x04) and delete it, then destroy the list (its implementation member out of line, then the abstract list base)
// Leaf: ??1CRegionMap@@QAE@XZ
// Flags: /EHsc
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C46860, 120 bytes, __fastcall `ret`, EH-framed (handler 0x0121559D: state 0
//   the list member, state 1 the list's base).
// Library: libpk (no PDB/RTTI): descriptive names. Callers: the level setter 0x00C37112 and 0x00C36D3A (both on a
//   stack map), and two unwind funclets (0x01214513, 0x0121457F). Constructor 0x00C467D0 (landed as
//   `CGuiVarTransferFloat`: it is this map's constructor). The manifest's "Manager_UnwindCleanupObjects" is propagated.
// Layout: +0x00 the node allocator (vtable 0x012B2F60: slot 0 Free 0x00C19640, slot 1 Alloc 0x00C19650 `new 14h`; a
//   member, not a base -- a base gives ~CRegionMap its own vptr store), +0x04 the PKCTRList of 16-byte region nodes
//   (vtable 0x012B7DF8; implementation pointer +4 -> +8, implementation destructor 0x00C19250; base vtable
//   0x012B1AB0, all `_purecall`), +0x34 cell size, +0x38 radius.
// Shape: the loop is `while ((node = m_regions.PopFirst()) != 0) delete node;` with the list's inline PopFirst
//   (GetHead, Remove when non-null): VC7.1 then keeps the single GetHead call at the loop top (`jmp` back); a plain
//   `while ((node = GetFirst()) != 0) { Remove(node); delete node; }` is rotated (132 bytes). The out-of-line list
//   destructor 0x00C46D30 is inlined here.
void operator delete(void* p) throw();

class CNodeAllocator {                      // vtable 0x012B2F60
public:
    virtual void Free(void* node);          // slot 0 (0x00C19640)
    virtual void* Alloc();                  // slot 1 (0x00C19650)
};

class CPKListImpl {
public:
    CPKListImpl(CNodeAllocator* allocator); // 0x00C194F0
    ~CPKListImpl();                         // 0x00C19250
    unsigned char m_body[0x28];
};

class CPKList {                             // vtable 0x012B1AB0
public:
    virtual long Compare(const void* a, const void* b) const = 0;   // slot 0
    virtual void v01() = 0;
    virtual void v02() = 0;
    virtual void v03() = 0;
    virtual ~CPKList() { }                  // slot 4
    virtual void v05() = 0;
    virtual void v06() = 0;
    virtual void v07() = 0;
    virtual void Remove(void* item) = 0;    // slot 8
    virtual void Add(void* item) = 0;       // slot 9
    virtual void v10() = 0;
    virtual void* GetHead() = 0;            // slot 11
};

template <class T>
class PKCTRList : public CPKList {          // vtable 0x012B7DF8 for the region nodes
public:
    PKCTRList(CNodeAllocator* allocator);   // 0x00C46CE0
    virtual ~PKCTRList() { }                // out of line at 0x00C46D30
    virtual long Compare(const void* a, const void* b) const;
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void Remove(void* item);
    virtual void Add(void* item);
    virtual void v10();
    virtual void* GetHead();
    T* GetFirst() { return (T*)GetHead(); }
    T* PopFirst()
    {
        T* item = GetFirst();
        if (item)
            Remove(item);
        return item;
    }
    CPKListImpl* m_pImpl;                   // +0x04 (-> m_impl)
    CPKListImpl  m_impl;                    // +0x08
};

struct SRegionNode {                        // 16 bytes (MarkRegion 0x00C465F0)
    long  m_visited;                        // +0x00
    long  m_x;                              // +0x04
    long  m_y;                              // +0x08
    float m_length;                         // +0x0c
};

class CRegionMap {
public:
    CNodeAllocator         m_allocator;     // +0x00
    PKCTRList<SRegionNode> m_regions;       // +0x04
    float                  m_cellSize;      // +0x34
    float                  m_radius;        // +0x38
    ~CRegionMap();
};

CRegionMap::~CRegionMap()
{
    SRegionNode* node;
    while ((node = m_regions.PopFirst()) != 0)
        delete node;
}