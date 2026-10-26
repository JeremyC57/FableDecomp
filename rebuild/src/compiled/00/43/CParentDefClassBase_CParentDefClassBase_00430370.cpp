// CParentDefClassBase::CParentDefClassBase() -- inlined CDefClassBase part (no manager, status bits clear), own vtable, an unset instantiation name (-1) and an empty, clean sub-def map
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00430370, length 51 bytes.
// Leaf: ??0CParentDefClassBase
// Callees: CResource::CResource @ 0x009FBEC0. Vtable 0x01230CBC = CParentDefClassBase (rtti-vtables.tsv).
// Layout: the Ego_r.pdb members (InstantiationName CDefString +0x25, SubDefInfoMap +0x29) on top of the
// packed CDefClassBase (0x25 bytes). The PDB CVectorMap is 20 bytes (16-byte vector, Compare +0x10,
// Dirty +0x11); the retail one is 16 (12-byte vector: Compare +0xc, Dirty +0xd = this+0x36), which is
// why retail CParentDefClassBase is 0x39 bytes against the header's 0x3d -- modelled locally.
// Shape: the whole CDefClassBase constructor (0x0042D9E0) is inline here, so its vtable store is dead
// and only CResource's out-of-line constructor is called; the CDefString and CVectorMap member
// constructors are inline (-1 and three null pointers + the Dirty byte, sharing the eax zero).
class CDefinitionManager;

class CResource { public:
    virtual void v00();
    unsigned char _resource[0x18];          // +0x04 resource list links, size, last used frame
    CResource();                            // 0x009FBEC0
};

class CDefPointeeBase : public CResource { public:        // vtable 0x01230BA0
    virtual void v00();
    CDefPointeeBase() { }
};

#pragma pack(push, 1)                       // def classes are byte packed
class CDefClassBase : public CDefPointeeBase { public:    // vtable 0x01230BCC
    CDefinitionManager* PDefManager;        // +0x1c
    unsigned long       GlobalIndex;        // +0x20
    unsigned char       Setup : 1;          // +0x24 bit 0
    unsigned char       Template : 1;       // +0x24 bit 1
    unsigned char       DefaultValsApplied : 1;     // +0x24 bit 2
    virtual void v00();
    CDefClassBase()                         // out of line at 0x0042D9E0, inline here
        : PDefManager(0)
    {
        Setup = 0;
        Template = 0;
        DefaultValsApplied = 0;
    }
};

class CDefString { public:                         // a def-name string-table position
    long TablePos;
    CDefString() : TablePos(-1) { }
};
#pragma pack(pop)

struct CSubDefInfoVectorMap {               // retail CVectorMap<unsigned long, CDefClassBase::CSubDefInfo> (16 bytes)
    void*         m_first;                  // +0x00 sorted vector of (key, info) pairs
    void*         m_last;                   // +0x04
    void*         m_end;                    // +0x08
    unsigned char Compare;                  // +0x0c CKeyPairCompareLess (empty)
    bool          Dirty;                    // +0x0d
    CSubDefInfoVectorMap() : m_first(0), m_last(0), m_end(0), Dirty(false) { }
};

#pragma pack(push, 1)
class CParentDefClassBase : public CDefClassBase { public:    // vtable 0x01230CBC
    CDefString           InstantiationName; // +0x25
    CSubDefInfoVectorMap SubDefInfoMap;     // +0x29
    virtual void v00();
    CParentDefClassBase();
};
#pragma pack(pop)

CParentDefClassBase::CParentDefClassBase()
{
}